//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <exception>
#include <fstream>
#include <algorithm>
#include <FreeImage.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "common/image.h"
#include "common/meshsurface.h"
#include "common/meshdescriptor.h"
#include "engine/lifeengine.h"
#include "engine/engine.h"
#include "engine/imaterialproxy.h"
#include "engine/materialproxyvar.h"
#include "engine/script.h"
#include "physics/iphysicsmodel.h"
#include "studiorender/istudiorender.h"
#include "studiorender/itexture.h"
#include "studiorender/imesh.h"
#include "studiorender/studiovertexelement.h"
#include "studiorender/studiorendersampler.h"
#include "studiorender/igpuprogram.h"

#include "global.h"
#include "mdldoc.h"
#include "lmtdoc.h"
#include "consolesystem.h"
#include "resourcesystem.h"
#include "level.h"
#include "fontfreetype.h"
#include "material.h"
#include "shaderparameter.h"
#include "materialproxyfactory.h"
#include "materialsystem.h"

#define PHY_ID			"LPHY"
#define PHY_VERSION		1

enum PHY_LUMPS
{
	PL_VERTECES,
	PL_INDECES,
	PL_MAX_LUMPS
};

struct PHYHeader
{
	char				strId[ 4 ]; // Always 'LPHY'
	le::UInt32_t		version;
};

struct PHYLump
{
	le::UInt32_t		offset;
	le::UInt32_t		length;
};

// ------------------------------------------------------------------------------------ //
// Загрузить изображение
// ------------------------------------------------------------------------------------ //
void LE_LoadImage( const char* Path, le::Image& Image, bool& IsError, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels )
{
	IsError = false;
	FREE_IMAGE_FORMAT		imageFormat = FIF_UNKNOWN;
	imageFormat = FreeImage_GetFileType( Path, 0 );

	if ( imageFormat == FIF_UNKNOWN )
		imageFormat = FreeImage_GetFIFFromFilename( Path );

	FIBITMAP* bitmap = FreeImage_Load( imageFormat, Path, 0 );
	if ( !bitmap )
	{
		IsError = true;
		return;
	}

	if ( IsFlipVertical )				FreeImage_FlipVertical( bitmap );
	if ( IsSwitchRedAndBlueChannels )
	{
		auto		red = FreeImage_GetChannel( bitmap, FREE_IMAGE_COLOR_CHANNEL::FICC_RED );
		auto		blue = FreeImage_GetChannel( bitmap, FREE_IMAGE_COLOR_CHANNEL::FICC_BLUE );

		FreeImage_SetChannel( bitmap, blue, FREE_IMAGE_COLOR_CHANNEL::FICC_RED );
		FreeImage_SetChannel( bitmap, red, FREE_IMAGE_COLOR_CHANNEL::FICC_BLUE );

		FreeImage_Unload( red );
		FreeImage_Unload( blue );
	}

	le::UInt8_t* tempData = FreeImage_GetBits( bitmap );
	Image.width = FreeImage_GetWidth( bitmap );
	Image.height = FreeImage_GetHeight( bitmap );
	Image.depth = FreeImage_GetBPP( bitmap );
	Image.pitch = FreeImage_GetPitch( bitmap );

	Image.data = ( le::UInt8_t* ) malloc( Image.pitch * Image.height );
	memcpy( Image.data, tempData, Image.pitch * Image.height );

	Image.rMask = 0x00ff0000;
	Image.gMask = 0x0000ff00;
	Image.bMask = 0x000000ff;
	Image.aMask = ( Image.depth == 24 ) ? 0 : 0xff000000;

	FreeImage_Unload( bitmap );
	return;
}

// ------------------------------------------------------------------------------------ //
// Загрузить текстуру
// ------------------------------------------------------------------------------------ //
le::ITexture* LE_LoadTexture( const char* Path, le::IFactory* StudioRenderFactory )
{
	bool				isError = false;
	le::Image			image;

	LE_LoadImage( Path, image, isError, false, true );
	if ( isError )			return nullptr;

	le::ITexture* texture = ( le::ITexture* ) StudioRenderFactory->Create( TEXTURE_INTERFACE_VERSION );
	if ( !texture )			return nullptr;

	texture->Initialize( le::TT_2D, image.aMask > 0 ? le::IF_RGBA_8UNORM : le::IF_RGB_8UNORM, image.width, image.height );
	texture->Bind();
	texture->Append( image.data );
	texture->GenerateMipmaps();

	le::StudioRenderSampler			sampler;
	sampler.minFilter = le::SF_LINEAR_MIPMAP_LINEAR;
	sampler.magFilter = le::SF_LINEAR;
	texture->SetSampler( sampler );

	texture->Unbind();

	delete[] image.data;
	return texture;
}

// ------------------------------------------------------------------------------------ //
// Загрузить материал
// ------------------------------------------------------------------------------------ //
le::IMaterial* LE_LoadMaterial( const char* Path, le::IResourceSystem* ResourceSystem, le::IMaterialSystem* MaterialSystem, le::IFactory* EngineFactory )
{
	LMTDoc			lmtDoc;
	if ( !lmtDoc.Load( Path ) )				return nullptr;

	// Create material and set surface name and shader
	le::Material*		material = new le::Material();
	material->SetSurfaceName( lmtDoc.GetSurface().c_str() );
	material->SetShader( lmtDoc.GetShader().c_str() );

	// Set cullface type
	switch ( lmtDoc.GetCullfaceType() )
	{
	case LMTDoc::CT_BACK:		material->SetCullFaceType( le::CT_BACK );	break;
	case LMTDoc::CT_FRONT:		material->SetCullFaceType( le::CT_FRONT );	break;
	}

	// Set settings material
	material->EnableBlend( lmtDoc.IsEnabledBlend() );
	material->EnableCullFace( lmtDoc.IsEnabledCullFace() );
	material->EnableDepthTest( lmtDoc.IsEnabledDepthTest() );
	material->EnableDepthWrite( lmtDoc.IsEnabledDepthWrite() );

	// Getting shader parameters
	const std::vector< LMTParameter >&			shaderParameters = lmtDoc.GetParameters();
	if ( !shaderParameters.empty() )
		for ( le::UInt32_t index = 0, count = shaderParameters.size(); index < count; ++index )
		{
			const LMTParameter&			parameter = shaderParameters[ index ];
			le::ShaderParameter*		shaderParameter = new le::ShaderParameter();

			le::SHADER_PARAMETER_TYPE			shaderParameterType;
			switch ( parameter.GetType() )
			{
			case LMTParameter::PT_INT:			shaderParameterType = le::SPT_INT;			break;
			case LMTParameter::PT_FLOAT:		shaderParameterType = le::SPT_FLOAT;		break;
			case LMTParameter::PT_BOOL:			shaderParameterType = le::SPT_SHADER_FLAG;	break;
			case LMTParameter::PT_TEXTURE:		shaderParameterType = le::SPT_TEXTURE;		break;
			case LMTParameter::PT_COLOR:		shaderParameterType = le::SPT_COLOR;		break;
			case LMTParameter::PT_VECTOR_2D:	shaderParameterType = le::SPT_VECTOR_2D;	break;
			case LMTParameter::PT_VECTOR_3D:	shaderParameterType = le::SPT_VECTOR_3D;	break;
			case LMTParameter::PT_VECTOR_4D:	shaderParameterType = le::SPT_VECTOR_4D;	break;
			default:							delete shaderParameter;						continue;
			}

			shaderParameter->SetName( parameter.GetName().c_str() );

			switch ( shaderParameterType )
			{
			case le::SPT_INT:			shaderParameter->SetValueInt( parameter.GetValueInt() );			break;
			case le::SPT_FLOAT:			shaderParameter->SetValueFloat( parameter.GetValueFloat() );		break;
			case le::SPT_SHADER_FLAG:	shaderParameter->SetValueShaderFlag( parameter.GetValueBool() );	break;
			case le::SPT_TEXTURE:
			{
				std::string				path = parameter.GetValueTexture();
				le::ITexture*			texture = ResourceSystem->LoadTexture( path.c_str(), path.c_str() );
				if ( !texture )
				{
					delete shaderParameter;
					continue;
				}

				shaderParameter->SetValueTexture( texture );
				break;
			}
			case le::SPT_COLOR:
			{
				LMTColor			color = parameter.GetValueColor();
				shaderParameter->SetValueColor( le::Color_t( color.r, color.g, color.b, color.a ) );
				break;
			}
			case le::SPT_VECTOR_2D:
			{
				LMTVector2D		vec2d = parameter.GetValueVector2D();
				shaderParameter->SetValueVector2D( le::Vector2D_t( vec2d.x, vec2d.y ) );
				break;
			}
			case le::SPT_VECTOR_3D:
			{
				LMTVector3D		vec3d = parameter.GetValueVector3D();
				shaderParameter->SetValueVector3D( le::Vector3D_t( vec3d.x, vec3d.y, vec3d.z ) );
				break;
			}
			case le::SPT_VECTOR_4D:
			{
				LMTVector4D		vec4d = parameter.GetValueVector4D();
				shaderParameter->SetValueVector4D( le::Vector4D_t( vec4d.x, vec4d.y, vec4d.z, vec4d.w ) );
				break;
			}
			}

			material->AddParameter( shaderParameter );
		}

	// Getting proxes
	const std::vector< LMTProxy >&			proxes = lmtDoc.GetProxes();
	if ( !proxes.empty() )
		for ( le::UInt32_t index = 0, count = proxes.size(); index < count; ++index )
		{
			const LMTProxy&				lmtProxy = proxes[ index ];
			auto&						lmtProxyParameters = lmtProxy.GetParameters();
			le::IMaterialProxy*			materialProxy = static_cast< le::MaterialProxyFactory* >( MaterialSystem->GetMaterialProxyFactory() )->Create( lmtProxy.GetName().c_str() );
			if ( !materialProxy )		continue;

			for ( le::UInt32_t indexParameter = 0, countParameters = lmtProxyParameters.size(); indexParameter < countParameters; ++indexParameter )
			{
				const LMTProxyParameter&		lmtProxyParameter = lmtProxyParameters[ indexParameter ];
				le::MaterialProxyVar*			proxyParameter = new le::MaterialProxyVar();

				le::MATERIAL_PROXY_VAR_TYPE		proxyParameterType = le::MPVT_NONE;
				switch ( lmtProxyParameter.GetType() )
				{
				case LMTProxyParameter::PT_BOOL:				proxyParameterType = le::MPVT_BOOL;					break;
				case LMTProxyParameter::PT_INT:					proxyParameterType = le::MPVT_INT;					break;
				case LMTProxyParameter::PT_FLOAT:				proxyParameterType = le::MPVT_FLOAT;				break;
				case LMTProxyParameter::PT_SHADER_PARAMETER:	proxyParameterType = le::MPVT_SHADER_PARAMETER;		break;
				case LMTProxyParameter::PT_VECTOR_2D:			proxyParameterType = le::MPVT_VECTOR_2D;			break;
				case LMTProxyParameter::PT_VECTOR_3D:			proxyParameterType = le::MPVT_VECTOR_3D;			break;
				case LMTProxyParameter::PT_VECTOR_4D:			proxyParameterType = le::MPVT_VECTOR_4D;			break;
				}

				proxyParameter->SetName( lmtProxyParameter.GetName().c_str() );

				switch ( proxyParameterType )
				{
				case le::MPVT_BOOL:					proxyParameter->SetValueBool( lmtProxyParameter.GetValueBool() );		break;
				case le::MPVT_INT:					proxyParameter->SetValueInt( lmtProxyParameter.GetValueInt() );			break;
				case le::MPVT_FLOAT:				proxyParameter->SetValueFloat( lmtProxyParameter.GetValueFloat() );		break;
				case le::MPVT_SHADER_PARAMETER:
				{
					std::string					nameShaderParameter = lmtProxyParameter.GetValueShaderParameter();
					le::IShaderParameter*		parameter = material->FindParameter( nameShaderParameter.c_str() );
					if ( !parameter )
					{
						delete proxyParameter;
						continue;
					}

					proxyParameter->SetValueShaderParameter( parameter );
					break;
				}
				case le::MPVT_VECTOR_2D:
				{
					LMTVector2D			vec2d = lmtProxyParameter.GetValueVector2D();
					proxyParameter->SetValueVector2D( le::Vector2D_t( vec2d.x, vec2d.y ) );
					break;
				}
				case le::MPVT_VECTOR_3D:
				{
					LMTVector3D			vec3d = lmtProxyParameter.GetValueVector3D();
					proxyParameter->SetValueVector3D( le::Vector3D_t( vec3d.x, vec3d.y, vec3d.z ) );
					break;
				}
				case le::MPVT_VECTOR_4D:
				{
					LMTVector4D			vec4d = lmtProxyParameter.GetValueVector4D();
					proxyParameter->SetValueVector4D( le::Vector4D_t( vec4d.x, vec4d.y, vec4d.z, vec4d.w ) );
					break;
				}
				}

				materialProxy->SetVar( proxyParameter );
			}

			material->AddProxy( materialProxy );
		}

	return material;
}

// ------------------------------------------------------------------------------------ //
// Загрузить меш
// ------------------------------------------------------------------------------------ //
le::IMesh* LE_LoadMesh( const char* Path, le::IResourceSystem* ResourceSystem, le::IFactory* StudioRenderFactory )
{
	MDLDoc				mdlDoc;
	if ( !mdlDoc.Load( Path ) )		return nullptr;

	// Getting all data
	auto&				verteces = mdlDoc.GetVerteces();
	auto&				vertexIndeces = mdlDoc.GetVertexIndeces();
	auto&				materialPaths = mdlDoc.GetMaterials();
	auto&				surfaces = mdlDoc.GetSurfaces();
	MDLVector3D			minXYZ = mdlDoc.GetMinXYZ();
	MDLVector3D			maxXYZ = mdlDoc.GetMaxXYZ();

	// Loading materials
	std::vector< le::IMaterial* >		materials;
	for ( le::UInt32_t index = 0, count = materialPaths.size(); index < count; ++index )
	{
		le::IMaterial*			material = ResourceSystem->LoadMaterial( materialPaths[ index ].c_str(), materialPaths[ index ].c_str() );
		if ( !material ) continue;

		materials.push_back( material );
	}

	// Convert MDLSurface to le::MeshSurface
	std::vector< le::MeshSurface >			meshSurfaces;
	for ( le::UInt32_t index = 0, count = surfaces.size(); index < count; ++index )
	{
		const MDLSurface&		mdlSurface = surfaces[ index ];
		le::MeshSurface			surface;
		
		surface.startVertexIndex = 0;
		surface.lightmapID = 0;
		surface.materialID = mdlSurface.materialId;
		surface.startIndex = mdlSurface.startVertexIndex;
		surface.countIndeces = mdlSurface.countVertexIndeces;
		meshSurfaces.push_back( surface );
	}
		
	le::IMesh* mesh = ( le::IMesh* ) StudioRenderFactory->Create( MESH_INTERFACE_VERSION );
	if ( !mesh )				return nullptr;

	// Create descriptor format verteces
	std::vector< le::StudioVertexElement >			vertexElements =
	{
		{ 3, le::VET_FLOAT },
		{ 3, le::VET_FLOAT },
		{ 2, le::VET_FLOAT },
		{ 3, le::VET_FLOAT },
		{ 3, le::VET_FLOAT }
	};

	// Creating mesh descriptor and loading to gpu
	le::MeshDescriptor				meshDescriptor;
	meshDescriptor.countIndeces = vertexIndeces.size();
	meshDescriptor.countMaterials = materials.size();
	meshDescriptor.countLightmaps = 0;
	meshDescriptor.countSurfaces = meshSurfaces.size();
	meshDescriptor.sizeVerteces = verteces.size() * sizeof( MDLVertex );

	meshDescriptor.indeces = ( le::UInt32_t* ) vertexIndeces.data();
	meshDescriptor.materials = materials.data();
	meshDescriptor.lightmaps = nullptr;
	meshDescriptor.surfaces = meshSurfaces.data();
	meshDescriptor.verteces = verteces.data();

	meshDescriptor.min = le::Vector3D_t( minXYZ.x, minXYZ.y, minXYZ.z );
	meshDescriptor.max = le::Vector3D_t( maxXYZ.x, maxXYZ.y, maxXYZ.z );
	meshDescriptor.primitiveType = le::PT_TRIANGLES;
	meshDescriptor.countVertexElements = vertexElements.size();
	meshDescriptor.vertexElements = vertexElements.data();

	// Loading mesh to gpu
	mesh->Create( meshDescriptor );
	if ( !mesh->IsCreated() )
	{
		StudioRenderFactory->Delete( mesh );
		return nullptr;
	}

	return mesh;
}

// ------------------------------------------------------------------------------------ //
// Load font
// ------------------------------------------------------------------------------------ //
le::IFont* LE_LoadFont( const char* Path )
{
	le::FontFreeType*			fontFreeType = new le::FontFreeType();
	if ( !fontFreeType->Load( Path ) )
	{
		delete fontFreeType;
		return nullptr;
	}

	return fontFreeType;
}

// ------------------------------------------------------------------------------------ //
// Загрузить уровень
// ------------------------------------------------------------------------------------ //
le::ILevel* LE_LoadLevel( const char* Path, le::IFactory* GameFactory )
{
	le::Level* level = new le::Level();
	if ( !level->Load( Path, GameFactory ) )
	{
		delete level;
		return nullptr;
	}

	return level;
};

// ------------------------------------------------------------------------------------ //
// Loading script
// ------------------------------------------------------------------------------------ //
le::IScript* LE_LoadScript( const char* Path, le::UInt32_t CountFunctions, le::ScriptDescriptor::Symbol* Functions, le::UInt32_t CountVars, le::ScriptDescriptor::Symbol* Vars, le::IFactory* ScriptSystemFactory )
{
	std::ifstream			file( Path );
	if ( !file.is_open() )	return nullptr;

	std::string			code;
	std::getline( file, code, '\0' );
	if ( code.empty() )		return nullptr;

	le::IScript*		script = ( le::IScript* ) ScriptSystemFactory->Create( SCRIPT_INTERFACE_VERSION );
	if ( !script )			return nullptr;

	le::ScriptDescriptor		scriptDescriptor;
	scriptDescriptor.code = code.c_str();
	scriptDescriptor.countFunctions = CountFunctions;
	scriptDescriptor.functions = Functions;
	scriptDescriptor.countVars = CountVars;
	scriptDescriptor.vars = Vars;

	if ( !script->Load( scriptDescriptor ) )
	{
		script->Release();
		return nullptr;
	}

	return script;
}

// ------------------------------------------------------------------------------------ //
// Loading collider
// ------------------------------------------------------------------------------------ //
le::IPhysicsModel* LE_LoadPhysicsModel( const char* Path, le::IFactory* PhysicsSystemFactory )
{
	std::ifstream				file( Path, std::ios::binary );
	if ( !file.is_open() )		return nullptr;

	// Read header file
	PHYHeader			phyHeader;
	file.read( ( char* ) &phyHeader, sizeof( PHYHeader ) );
	if ( strncmp( phyHeader.strId, PHY_ID, 4 ) != 0 || phyHeader.version != PHY_VERSION )
		return nullptr;

	// Read all lumps
	PHYLump				phyLumps[ PL_MAX_LUMPS ];
	file.read( ( char* ) &phyLumps[ 0 ], sizeof( PHYLump ) * PL_MAX_LUMPS );

	// Read verteces
	std::vector< le::Vector3D_t >		verteces;
	le::UInt32_t						countVerteces;

	file.seekg( phyLumps[ PL_VERTECES ].offset, std::ios::beg );
	file.read( ( char* ) &countVerteces, sizeof( le::UInt32_t ) );

	verteces.resize( countVerteces );
	file.read( ( char* ) verteces.data(), countVerteces * sizeof( le::Vector3D_t ) );

	// Read indeces
	std::vector< le::UInt32_t >		indeces;
	le::UInt32_t					countIndeces;

	file.seekg( phyLumps[ PL_INDECES ].offset, std::ios::beg );
	file.read( ( char* ) &countIndeces, sizeof( le::UInt32_t ) );

	indeces.resize( countIndeces );
	file.read( ( char* ) indeces.data(), countIndeces * sizeof( le::UInt32_t ) );

	if ( verteces.empty() ) return nullptr;

	// Creating physics models
	le::IPhysicsModel*		physicsModel = ( le::IPhysicsModel* ) PhysicsSystemFactory->Create( PHYSICSMODEL_INTERFACE_VERSION );
	if ( !physicsModel )	return nullptr;

	physicsModel->InitializeMesh( verteces.data(), verteces.size(), indeces.data(), indeces.size() );
	return physicsModel;
}

// ------------------------------------------------------------------------------------ //
// Loading gpu program
// ------------------------------------------------------------------------------------ //
le::IGPUProgram* LE_LoadGPUProgram( const char* Directory, const char* Path, le::UInt32_t CountDefines, const char** Defines, le::IFactory* StudioRenderFactory )
{
	std::ifstream				file( Path );
	if ( !file.is_open() )		return nullptr;

	std::string					stringBuffer;
	std::getline( file, stringBuffer, '\0' );

	rapidjson::Document			document;
	document.Parse( stringBuffer.c_str() );
	if ( document.HasParseError() )				return nullptr;

	std::string				vertexShader;
	std::string				pixelShader;

	// Reading all path to shaders
	for ( auto itRoot = document.MemberBegin(), itRootEnd = document.MemberEnd(); itRoot != itRootEnd; ++itRoot )
	{
		// Vertex shader
		if ( strcmp( itRoot->name.GetString(), "vertex" ) == 0 && itRoot->value.IsString() )
			vertexShader = itRoot->value.GetString();

		// Pixel shader
		else if ( strcmp( itRoot->name.GetString(), "pixel" ) == 0 && itRoot->value.IsString() )
			pixelShader = itRoot->value.GetString();
	}

	// Loading vertex shader
	file.close();
	file.open( std::string( Directory ) + "/" + vertexShader );
	if ( !file.is_open() )			return nullptr;

	vertexShader.clear();
	std::getline( file, vertexShader, '\0' );

	// Loading pixel shader
	file.close();
	file.open( std::string( Directory ) + "/" + pixelShader );
	if ( !file.is_open() )			return nullptr;

	pixelShader.clear();
	std::getline( file, pixelShader, '\0' );

	// Creating gpu program
	le::IGPUProgram*			gpuProgram = ( le::IGPUProgram* ) StudioRenderFactory->Create( GPUPROGRAM_INTERFACE_VERSION );
	if ( !gpuProgram )			return nullptr;

	le::GPUProgramDescriptor		gpuProgramDescriptor;
	gpuProgramDescriptor.vertexShaderSource = vertexShader.c_str();
	gpuProgramDescriptor.fragmentShaderSource = pixelShader.c_str();
	gpuProgramDescriptor.geometryShaderSource = nullptr;

	if ( !gpuProgram->Compile( gpuProgramDescriptor, CountDefines, Defines ) )
		return nullptr;

	return gpuProgram;
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать загрузчик картинок
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_Image( const char* Format, LoadImageFn_t LoadImage )
{
	LIFEENGINE_ASSERT( Format );
	LIFEENGINE_ASSERT( LoadImage );

	g_consoleSystem->PrintInfo( "Loader image for format [%s] registered", Format );
	loaderImages[ Format ] = LoadImage;
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать загрузчик текстур
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_Texture( const char* Format, LoadTextureFn_t LoadTexture )
{
	LIFEENGINE_ASSERT( Format );
	LIFEENGINE_ASSERT( LoadTexture );

	g_consoleSystem->PrintInfo( "Loader texture for format [%s] registered", Format );
	loaderTextures[ Format ] = LoadTexture;
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать загрузчик материалов
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_Material( const char* Format, LoadMaterialFn_t LoadMaterial )
{
	LIFEENGINE_ASSERT( Format );
	LIFEENGINE_ASSERT( LoadMaterial );

	g_consoleSystem->PrintInfo( "Loader material for format [%s] registered", Format );
	loaderMaterials[ Format ] = LoadMaterial;
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать загрузчик мешей
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_Mesh( const char* Format, LoadMeshFn_t LoadMesh )
{
	LIFEENGINE_ASSERT( Format );
	LIFEENGINE_ASSERT( LoadMesh );

	g_consoleSystem->PrintInfo( "Loader mesh for format [%s] registered", Format );
	loaderMeshes[ Format ] = LoadMesh;
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать загрузчик уровней
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_Level( const char* Format, LoadLevelFn_t LoadLevel )
{
	LIFEENGINE_ASSERT( Format );
	LIFEENGINE_ASSERT( LoadLevel );

	g_consoleSystem->PrintInfo( "Loader level for format [%s] registered", Format );
	loaderLevels[ Format ] = LoadLevel;
}

// ------------------------------------------------------------------------------------ //
// Register loader fonts
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_Font( const char* Format, LoadFontFn_t LoadFont )
{
	LIFEENGINE_ASSERT( Format );
	LIFEENGINE_ASSERT( LoadFont );

	g_consoleSystem->PrintInfo( "Loader font for format [%s] registered", Format );
	loaderFonts[ Format ] = LoadFont;
}

// ------------------------------------------------------------------------------------ //
// Разрегестрировать загрузчик картинок
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_Image( const char* Format )
{
	LIFEENGINE_ASSERT( Format );

	auto		it = loaderImages.find( Format );
	if ( it == loaderImages.end() ) return;

	loaderImages.erase( it );
	g_consoleSystem->PrintInfo( "Loader image for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Разрегестрировать загрузчик текстур
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_Texture( const char* Format )
{
	LIFEENGINE_ASSERT( Format );

	auto		it = loaderTextures.find( Format );
	if ( it == loaderTextures.end() ) return;

	loaderTextures.erase( it );
	g_consoleSystem->PrintInfo( "Loader texture for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Разрегестрировать загрузчик материалов
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_Material( const char* Format )
{
	LIFEENGINE_ASSERT( Format );

	auto		it = loaderMaterials.find( Format );
	if ( it == loaderMaterials.end() ) return;

	loaderMaterials.erase( it );
	g_consoleSystem->PrintInfo( "Loader material for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Разрегестрировать загрузчик мешей
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_Mesh( const char* Format )
{
	LIFEENGINE_ASSERT( Format );

	auto		it = loaderMeshes.find( Format );
	if ( it == loaderMeshes.end() ) return;

	loaderMeshes.erase( it );
	g_consoleSystem->PrintInfo( "Loader mesh for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Разрегестрировать загрузчик уровней
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_Level( const char* Format )
{
	LIFEENGINE_ASSERT( Format );

	auto		it = loaderLevels.find( Format );
	if ( it == loaderLevels.end() ) return;

	loaderLevels.erase( it );
	g_consoleSystem->PrintInfo( "Loader level for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Unregister loader font
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_Font( const char* Format )
{
	LIFEENGINE_ASSERT( Format );

	auto		it = loaderFonts.find( Format );
	if ( it == loaderFonts.end() ) return;

	loaderFonts.erase( it );
	g_consoleSystem->PrintInfo( "Loader font for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Загрузить картинку
// ------------------------------------------------------------------------------------ //
le::Image le::ResourceSystem::LoadImage( const char* Path, bool& IsError, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels )
{
	LIFEENGINE_ASSERT( Path );
	IsError = true;

	try
	{
		if ( loaderImages.empty() )					throw std::runtime_error( "No image loaders" );

		std::string			format = GetFormatFile( Path );
		if ( format.empty() )						throw std::runtime_error( "In image format not found" );

		auto				parser = loaderImages.find( format );
		if ( parser == loaderImages.end() )			throw std::runtime_error( "Loader for format image not found" );

		Image				image;
		for ( UInt32_t index = 0, count = paths.size(); index < count; ++index )
		{
			parser->second( std::string( paths[ index ] + "/" + Path ).c_str(), image, IsError, IsFlipVertical, IsSwitchRedAndBlueChannels );
			if ( image.data )
			{
				g_consoleSystem->PrintInfo( "Image founded in [%s]", paths[ index ].c_str() );
				break;
			}
		}

		if ( IsError )								throw std::runtime_error( "Fail loading image" );
		return image;
	}
	catch ( std::exception & Exception )
	{
		g_consoleSystem->PrintError( "Image [%s] not loaded: %s", Path, Exception.what() );
		return Image();
	}
}

// ------------------------------------------------------------------------------------ //
// Загрузить текстуру
// ------------------------------------------------------------------------------------ //
le::ITexture* le::ResourceSystem::LoadTexture( const char* Name, const char* Path )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Path );

	try
	{
		if ( !studioRenderFactory )							throw std::runtime_error( "Resource system not initialized" );

		if ( textures.find( Name ) != textures.end() )		return textures[ Name ];
		if ( loaderTextures.empty() )						throw std::runtime_error( "No texture loaders" );

		g_consoleSystem->PrintInfo( "Loading texture [%s] with name [%s]", Path, Name );

		std::string			format = GetFormatFile( Path );
		if ( format.empty() )						throw std::runtime_error( "In texture format not found" );

		auto				parser = loaderTextures.find( format );
		if ( parser == loaderTextures.end() )		throw std::runtime_error( "Loader for format texture not found" );

		ITexture*			texture = nullptr;
		for ( UInt32_t index = 0, count = paths.size(); index < count; ++index )
		{
			texture = parser->second( std::string( paths[ index ] + "/" + Path ).c_str(), studioRenderFactory );
			if ( texture )
			{
				g_consoleSystem->PrintInfo( "Texture founded in [%s]", paths[ index ].c_str() );
				break;
			}
		}

		if ( !texture )								throw std::runtime_error( "Fail loading texture" );
		texture->IncrementReference();
		textures.insert( std::make_pair( Name, texture ) );
		g_consoleSystem->PrintInfo( "Loaded texture [%s]", Name );

		return texture;
	}
	catch ( std::exception & Exception )
	{
		g_consoleSystem->PrintError( "Texture [%s] not loaded: %s", Path, Exception.what() );
		return nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Загрузить материал
// ------------------------------------------------------------------------------------ //
le::IMaterial* le::ResourceSystem::LoadMaterial( const char* Name, const char* Path )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Path );

	try
	{
		if ( !studioRenderFactory )							throw std::runtime_error( "Resource system not initialized" );

		if ( materials.find( Name ) != materials.end() )	return materials[ Name ];
		if ( loaderMaterials.empty() )						throw std::runtime_error( "No material loaders" );

		g_consoleSystem->PrintInfo( "Loading material [%s] with name [%s]", Path, Name );

		std::string			format = GetFormatFile( Path );
		if ( format.empty() )						throw std::runtime_error( "In material format not found" );

		auto				parser = loaderMaterials.find( format );
		if ( parser == loaderMaterials.end() )		throw std::runtime_error( "Loader for format material not found" );

		IMaterial*			material = nullptr;
		for ( UInt32_t index = 0, count = paths.size(); index < count; ++index )
		{
			material = parser->second( std::string( paths[ index ] + "/" + Path ).c_str(), this, g_materialSystem, engineFactory );
			if ( material )
			{
				g_consoleSystem->PrintInfo( "Material founded in [%s]", paths[ index ].c_str() );
				break;
			}
		}

		if ( !material )	throw std::runtime_error( "Fail loading material" );
		material->IncrementReference();
		materials.insert( std::make_pair( Name, material ) );
		g_consoleSystem->PrintInfo( "Loaded material [%s]", Name );

		return material;
	}
	catch ( std::exception & Exception )
	{
		g_consoleSystem->PrintError( "Material [%s] not loaded: %s", Path, Exception.what() );
		return nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Загрузить меш
// ------------------------------------------------------------------------------------ //
le::IMesh* le::ResourceSystem::LoadMesh( const char* Name, const char* Path )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Path );

	try
	{
		if ( !studioRenderFactory )							throw std::runtime_error( "Resource system not initialized" );

		if ( meshes.find( Name ) != meshes.end() )			return meshes[ Name ];
		if ( loaderMeshes.empty() )							throw std::runtime_error( "No mesh loaders" );

		g_consoleSystem->PrintInfo( "Loading mesh [%s] with name [%s]", Path, Name );

		std::string			format = GetFormatFile( Path );
		if ( format.empty() )						throw std::runtime_error( "In mesh format not found" );

		auto				parser = loaderMeshes.find( format );
		if ( parser == loaderMeshes.end() )		throw std::runtime_error( "Loader for format mesh not found" );

		IMesh*			mesh = nullptr;
		for ( UInt32_t index = 0; index < paths.size(); ++index )
		{
			mesh = parser->second( std::string( paths[ index ] + "/" + Path ).c_str(), this, studioRenderFactory );
			if ( mesh )
			{
				g_consoleSystem->PrintInfo( "Mesh founded in [%s]", paths[ index ].c_str() );
				break;
			}
		}

		if ( !mesh )							throw std::runtime_error( "Fail loading mesh" );
		mesh->IncrementReference();
		meshes.insert( std::make_pair( Name, mesh ) );
		g_consoleSystem->PrintInfo( "Loaded mesh [%s]", Name );

		return mesh;
	}
	catch ( std::exception & Exception )
	{
		g_consoleSystem->PrintError( "Mesh [%s] not loaded: %s", Path, Exception.what() );
		return nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Загрузить уровень
// ------------------------------------------------------------------------------------ //
le::ILevel* le::ResourceSystem::LoadLevel( const char* Name, const char* Path, IFactory* GameFactory )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Path );

	try
	{
		if ( !studioRenderFactory )							throw std::runtime_error( "Resource system not initialized" );

		if ( levels.find( Name ) != levels.end() )			return levels[ Name ];
		if ( loaderLevels.empty() )							throw std::runtime_error( "No level loaders" );

		g_consoleSystem->PrintInfo( "Loading level [%s] with name [%s]", Path, Name );

		std::string			format = GetFormatFile( Path );
		if ( format.empty() )						throw std::runtime_error( "In level format not found" );

		auto				parser = loaderLevels.find( format );
		if ( parser == loaderLevels.end() )		throw std::runtime_error( "Loader for format level not found" );

		ILevel*			level = nullptr;
		for ( UInt32_t index = 0, count = paths.size(); index < count; ++index )
		{
			level = parser->second( std::string( paths[ index ] + "/" + Path ).c_str(), GameFactory );
			if ( level )
			{
				g_consoleSystem->PrintInfo( "Level founded in [%s]", paths[ index ].c_str() );
				break;
			}
		}

		if ( !level )							throw std::runtime_error( "Fail loading level" );
		level->IncrementReference();
		levels.insert( std::make_pair( Name, level ) );
		g_consoleSystem->PrintInfo( "Loaded level [%s]", Name );

		return level;
	}
	catch ( std::exception & Exception )
	{
		g_consoleSystem->PrintError( "Level [%s] not loaded: %s", Path, Exception.what() );
		return nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Load font
// ------------------------------------------------------------------------------------ //
le::IFont* le::ResourceSystem::LoadFont( const char* Name, const char* Path )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Path );

	try
	{
		if ( fonts.find( Name ) != fonts.end() )			return fonts[ Name ];
		if ( loaderFonts.empty() )							throw std::runtime_error( "No font loaders" );

		g_consoleSystem->PrintInfo( "Loading font [%s] with name [%s]", Path, Name );

		std::string			format = GetFormatFile( Path );
		if ( format.empty() )						throw std::runtime_error( "In font format not found" );

		auto				parser = loaderFonts.find( format );
		if ( parser == loaderFonts.end() )		throw std::runtime_error( "Loader for format font not found" );

		IFont*			font = nullptr;
		for ( UInt32_t index = 0, count = paths.size(); index < count; ++index )
		{
			font = parser->second( std::string( paths[ index ] + "/" + Path ).c_str() );
			if ( font )
			{
				g_consoleSystem->PrintInfo( "Font founded in [%s]", paths[ index ].c_str() );
				break;
			}
		}

		if ( !font )							throw std::runtime_error( "Fail loading font" );
		font->IncrementReference();
		fonts.insert( std::make_pair( Name, font ) );
		g_consoleSystem->PrintInfo( "Loaded font [%s]", Name );

		return font;
	}
	catch ( std::exception & Exception )
	{
		g_consoleSystem->PrintError( "Font [%s] not loaded: %s", Path, Exception.what() );
		return nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Выгрузить картинку
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadImage( Image& Image )
{
	if ( !Image.data ) return;

	delete[] Image.data;
	Image.data = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Выгрузить текстуру
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadTexture( const char* Name )
{
	LIFEENGINE_ASSERT( Name );

	auto				it = textures.find( Name );
	if ( it == textures.end() || it->second->GetCountReferences() > 1 )	return;

	it->second->Release();
	textures.erase( it );

	g_consoleSystem->PrintInfo( "Unloaded texture [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Выгрузить материал
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadMaterial( const char* Name )
{
	LIFEENGINE_ASSERT( Name );

	auto				it = materials.find( Name );
	if ( it == materials.end() || it->second->GetCountReferences() > 1 )	return;

	it->second->Release();
	materials.erase( it );

	g_consoleSystem->PrintInfo( "Unloaded material [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Выгрузить меш
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadMesh( const char* Name )
{
	LIFEENGINE_ASSERT( Name );

	auto				it = meshes.find( Name );
	if ( it == meshes.end() || it->second->GetCountReferences() > 1 )	return;

	it->second->Release();
	meshes.erase( it );

	g_consoleSystem->PrintInfo( "Unloaded mesh [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Выгрузить уровень
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadLevel( const char* Name )
{
	LIFEENGINE_ASSERT( Name );

	auto				it = levels.find( Name );
	if ( it == levels.end() || it->second->GetCountReferences() > 1 )	return;

	it->second->Release();
	levels.erase( it );

	g_consoleSystem->PrintInfo( "Unloaded level [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Unload font
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadFont( const char* Name )
{
	LIFEENGINE_ASSERT( Name );

	auto				it = fonts.find( Name );
	if ( it == fonts.end() || it->second->GetCountReferences() > 1 )	return;

	it->second->Release();
	fonts.erase( it );

	g_consoleSystem->PrintInfo( "Unloaded font [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все материалы
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadMaterials()
{
	if ( materials.empty() ) return;

	for ( auto it = materials.begin(), itEnd = materials.end(); it != itEnd; )
		if ( it->second->GetCountReferences() <= 1 )
		{
			it->second->Release();

			g_consoleSystem->PrintInfo( "Unloaded material [%s]", it->first.c_str() );
			it = materials.erase( it );
			itEnd = materials.end();
		}
		else
			++it;
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все мешы
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadMeshes()
{
	if ( meshes.empty() ) return;

	for ( auto it = meshes.begin(), itEnd = meshes.end(); it != itEnd; )
		if ( it->second->GetCountReferences() <= 1 )
		{
			it->second->Release();

			g_consoleSystem->PrintInfo( "Unloaded mesh [%s]", it->first.c_str() );
			it = meshes.erase( it );
			itEnd = meshes.end();
		}
		else
			++it;
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все уровени
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadLevels()
{
	if ( levels.empty() ) return;

	for ( auto it = levels.begin(), itEnd = levels.end(); it != itEnd; )
		if ( it->second->GetCountReferences() <= 1 )
		{
			it->second->Release();

			g_consoleSystem->PrintInfo( "Unloaded level [%s]", it->first.c_str() );
			it = levels.erase( it );
			itEnd = levels.end();
		}
		else
			++it;
}

// ------------------------------------------------------------------------------------ //
// Unload all fonts
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadFonts()
{
	if ( fonts.empty() ) return;

	for ( auto it = fonts.begin(), itEnd = fonts.end(); it != itEnd; )
		if ( it->second->GetCountReferences() <= 1 )
		{
			it->second->Release();

			g_consoleSystem->PrintInfo( "Unloaded font [%s]", it->first.c_str() );
			it = fonts.erase( it );
			itEnd = fonts.end();
		}
		else
			++it;
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все текстуры
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadTextures()
{
	if ( textures.empty() ) return;

	if ( !studioRenderFactory )
	{
		g_consoleSystem->PrintError( "Resource system not initialized" );
		return;
	}

	for ( auto it = textures.begin(), itEnd = textures.end(); it != itEnd; )
		if ( it->second->GetCountReferences() <= 1 )
		{
			it->second->Release();

			g_consoleSystem->PrintInfo( "Unloaded texture [%s]", it->first.c_str() );
			it = textures.erase( it );
			itEnd = textures.end();
		}
		else
			++it;
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все ресурсы
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadAll()
{
	UnloadScripts();
	UnloadLevels();
	UnloadMeshes();
	UnloadFonts();
	UnloadMaterials();
	UnloadTextures();
	UnloadPhysicsModels();
	UnloadGPUPrograms();
}

// ------------------------------------------------------------------------------------ //
//  Получить текстуру
// ------------------------------------------------------------------------------------ //
le::ITexture* le::ResourceSystem::GetTexture( const char* Name ) const
{
	LIFEENGINE_ASSERT( Name );

	auto	it = textures.find( Name );
	if ( it != textures.end() )		return it->second;

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Получить материал
// ------------------------------------------------------------------------------------ //
le::IMaterial* le::ResourceSystem::GetMaterial( const char* Name ) const
{
	LIFEENGINE_ASSERT( Name );

	auto	it = materials.find( Name );
	if ( it != materials.end() )		return it->second;

	return nullptr;
}
// ------------------------------------------------------------------------------------ //
// Получить меш
// ------------------------------------------------------------------------------------ //
le::IMesh* le::ResourceSystem::GetMesh( const char* Name ) const
{
	LIFEENGINE_ASSERT( Name );

	auto	it = meshes.find( Name );
	if ( it != meshes.end() )		return it->second;

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Получить уровень
// ------------------------------------------------------------------------------------ //
le::ILevel* le::ResourceSystem::GetLevel( const char* Name ) const
{
	LIFEENGINE_ASSERT( Name );

	auto	it = levels.find( Name );
	if ( it != levels.end() )		return it->second;

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Get font
// ------------------------------------------------------------------------------------ //
le::IFont* le::ResourceSystem::GetFont( const char* Name ) const
{
	LIFEENGINE_ASSERT( Name );

	auto	it = fonts.find( Name );
	if ( it != fonts.end() )		return it->second;

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Инициализировать систему ресурсов
// ------------------------------------------------------------------------------------ //
bool le::ResourceSystem::Initialize( IEngine* Engine )
{
	try
	{
		IStudioRender* studioRender = Engine->GetStudioRender();
		if ( !studioRender )	throw std::runtime_error( "Resource system requared studiorender" );

		studioRenderFactory = studioRender->GetFactory();
		scriptSystemFactory = g_scriptSystem->GetFactory();
		engineFactory = Engine->GetFactory();

		RegisterLoader_Image( "png", LE_LoadImage );
		RegisterLoader_Image( "jpg", LE_LoadImage );
		RegisterLoader_Image( "tga", LE_LoadImage );
		RegisterLoader_Texture( "png", LE_LoadTexture );
		RegisterLoader_Texture( "jpg", LE_LoadTexture );
		RegisterLoader_Texture( "tga", LE_LoadTexture );
		RegisterLoader_Material( "lmt", LE_LoadMaterial );
		RegisterLoader_Mesh( "mdl", LE_LoadMesh );
		RegisterLoader_Level( "bsp", LE_LoadLevel );
		RegisterLoader_Font( "ttf", LE_LoadFont );
		RegisterLoader_Script( "c", LE_LoadScript );
		RegisterLoader_PhysicsModel( "phy", LE_LoadPhysicsModel );
		RegisterLoader_GPUProgram( "shader", LE_LoadGPUProgram );
	}
	catch ( std::exception & Exception )
	{
		g_consoleSystem->PrintError( Exception.what() );
		return false;
	}
	return true;
}

// ------------------------------------------------------------------------------------ //
// Add path to resources
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::AddPath( const char* Path )
{
	if ( !Path ) return;

	g_consoleSystem->PrintInfo( "Resources path [%s] added to resource system", Path );
	paths.push_back( Path );
}

// ------------------------------------------------------------------------------------ //
// Remove path to resources
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RemovePath( le::UInt32_t Index )
{
	LIFEENGINE_ASSERT( Index < paths.size() );

	g_consoleSystem->PrintInfo( "Resources path [%s] removed from resource system", paths[ Index ].c_str() );
	paths.erase( paths.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Clear paths
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::ClearPaths()
{
	if ( paths.empty() ) return;

	for ( UInt32_t index = 0, count = paths.size(); index < count; ++index )
		g_consoleSystem->PrintInfo( "Resources path [%s] removed from resource system", paths[ index ].c_str() );
	paths.clear();
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ResourceSystem::ResourceSystem() :
	studioRenderFactory( nullptr ),
	engineFactory( nullptr ),
	scriptSystemFactory( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ResourceSystem::~ResourceSystem()
{
	UnloadAll();
}

// ------------------------------------------------------------------------------------ //
// Get format file
// ------------------------------------------------------------------------------------ //
std::string le::ResourceSystem::GetFormatFile( const std::string& Route )
{
	UInt32_t		position = Route.find_last_of( '.' );
	if ( position == std::string::npos )
		return "";

	// Если расширение есть, то переводим в малый регистр и возвращаем
	std::string		format = Route.substr( position + 1, std::string::npos );
	for ( UInt32_t index = 0, count = format.size(); index < count; ++index )
		format[ index ] = tolower( format[ index ] );

	return format;
}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_Script( const char* Format, LoadScriptFn_t LoadScript )
{
	LIFEENGINE_ASSERT( Format );
	LIFEENGINE_ASSERT( LoadScript );

	g_consoleSystem->PrintInfo( "Loader script for format [%s] registered", Format );
	loaderScripts[ Format ] = LoadScript;
}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_Script( const char* Format )
{
	LIFEENGINE_ASSERT( Format );

	auto		it = loaderScripts.find( Format );
	if ( it == loaderScripts.end() ) return;

	loaderScripts.erase( it );
	g_consoleSystem->PrintInfo( "Loader script for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::IScript* le::ResourceSystem::LoadScript( const char* Name, const char* Path, UInt32_t CountFunctions, ScriptDescriptor::Symbol* Functions, UInt32_t CountVars, ScriptDescriptor::Symbol* Vars )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Path );

	try
	{
		if ( !scriptSystemFactory )							throw std::runtime_error( "Resource system not initialized" );

		if ( scripts.find( Name ) != scripts.end() )		return scripts[ Name ];
		if ( loaderScripts.empty() )						throw std::runtime_error( "No script loaders" );

		g_consoleSystem->PrintInfo( "Loading script [%s] with name [%s]", Path, Name );

		std::string			format = GetFormatFile( Path );
		if ( format.empty() )						throw std::runtime_error( "In script format not found" );

		auto				parser = loaderScripts.find( format );
		if ( parser == loaderScripts.end() )		throw std::runtime_error( "Loader for format script not found" );

		IScript*			script = nullptr;
		for ( UInt32_t index = 0, count = paths.size(); index < count; ++index )
		{
			script = parser->second( std::string( paths[ index ] + "/" + Path ).c_str(), CountFunctions, Functions, CountVars, Vars, scriptSystemFactory );
			if ( script )
			{
				g_consoleSystem->PrintInfo( "Script founded in [%s]", paths[ index ].c_str() );
				break;
			}
		}

		if ( !script )							throw std::runtime_error( "Fail loading script" );
		script->IncrementReference();
		scripts.insert( std::make_pair( Name, script ) );
		g_consoleSystem->PrintInfo( "Loaded script [%s]", Name );

		return script;
	}
	catch ( std::exception & Exception )
	{
		g_consoleSystem->PrintError( "Script [%s] not loaded: %s", Path, Exception.what() );
		return nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadScript( const char* Name )
{
	LIFEENGINE_ASSERT( Name );

	auto				it = scripts.find( Name );
	if ( it == scripts.end() || it->second->GetCountReferences() > 1 )	return;

	it->second->Release();
	scripts.erase( it );

	g_consoleSystem->PrintInfo( "Unloaded script [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadScripts()
{
	if ( scripts.empty() ) return;

	for ( auto it = scripts.begin(), itEnd = scripts.end(); it != itEnd; )
		if ( it->second->GetCountReferences() <= 1 )
		{
			it->second->Release();

			g_consoleSystem->PrintInfo( "Unloaded script [%s]", it->first.c_str() );
			it = scripts.erase( it );
			itEnd = scripts.end();
		}
		else
			++it;
}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::IScript* le::ResourceSystem::GetScript( const char* Name ) const
{
	LIFEENGINE_ASSERT( Name );

	auto	it = scripts.find( Name );
	if ( it != scripts.end() )		return it->second;

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Register loader for physics model
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_PhysicsModel( const char *Format, LoadPhysicsModelFn_t LoadPhysicsModel )
{
	LIFEENGINE_ASSERT( Format );
	LIFEENGINE_ASSERT( LoadPhysicsModel );

	g_consoleSystem->PrintInfo( "Loader physics model for format [%s] registered", Format );
	loaderPhysicsModels[ Format ] = LoadPhysicsModel;
}

// ------------------------------------------------------------------------------------ //
// Rigister loader gpu program
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_GPUProgram( const char* Format, LoadGPUProgramFn_t LoadGPUProgram )
{
	LIFEENGINE_ASSERT( Format );
	LIFEENGINE_ASSERT( LoadGPUProgram );

	g_consoleSystem->PrintInfo( "Loader gpu program for format [%s] registered", Format );
	loaderGPUProgram[ Format ] = LoadGPUProgram;
}

// ------------------------------------------------------------------------------------ //
// Unregister loader for physics model
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_PhysicsModel( const char *Format )
{
	LIFEENGINE_ASSERT( Format );

	auto		it = loaderPhysicsModels.find( Format );
	if ( it == loaderPhysicsModels.end() ) return;

	loaderPhysicsModels.erase( it );
	g_consoleSystem->PrintInfo( "Loader physics model for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Unregister loader gpu program
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_GPUProgram( const char* Format )
{
	LIFEENGINE_ASSERT( Format );

	auto		it = loaderGPUProgram.find( Format );
	if ( it == loaderGPUProgram.end() ) return;

	loaderGPUProgram.erase( it );
	g_consoleSystem->PrintInfo( "Loader gpu program for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Unload physics model
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadPhysicsModel( const char *Name )
{
	LIFEENGINE_ASSERT( Name );

	auto				it = physicsModels.find( Name );
	if ( it == physicsModels.end() || it->second->GetCountReferences() > 1 )	return;

	it->second->Release();
	physicsModels.erase( it );

	g_consoleSystem->PrintInfo( "Unloaded physics model [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Unload gpu program
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadGPUProgram( const char* Name, UInt32_t Flags )
{
	LIFEENGINE_ASSERT( Name );

	auto				itShaders = gpuPrograms.find( Name );
	if ( itShaders == gpuPrograms.end() )	return;

	auto				itShader = itShaders->second.find( Flags );
	if ( itShader == itShaders->second.end() || itShader->second->GetCountReferences() > 1 )
		return;

	itShader->second->Release();
	itShaders->second.erase( itShader );

	if ( itShaders->second.empty() )
		gpuPrograms.erase( itShaders );

	g_consoleSystem->PrintInfo( "Unloaded gpu program [%s] with flags [%i]", Name, Flags );
}

// ------------------------------------------------------------------------------------ //
// Unload gpu programs
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadGPUProgram( const char* Name )
{
	LIFEENGINE_ASSERT( Name );

	auto				itShaders = gpuPrograms.find( Name );
	if ( itShaders == gpuPrograms.end() )	return;

	for ( auto itShader = itShaders->second.begin(), itShaderEnd = itShaders->second.end(); itShader != itShaderEnd; )
		if ( itShader->second->GetCountReferences() <= 1 )
		{
			itShader->second->Release();

			g_consoleSystem->PrintInfo( "Unloaded gpu program [%s] with flags [%i]", Name, itShader->first );
			itShader = itShaders->second.erase( itShader );
			itShaderEnd = itShaders->second.end();
		}
		else
			++itShader;
}

// ------------------------------------------------------------------------------------ //
// Load physics model
// ------------------------------------------------------------------------------------ //
le::IPhysicsModel* le::ResourceSystem::LoadPhysicsModel( const char *Name, const char *Path )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Path );

	try
	{
		if ( physicsModels.find( Name ) != physicsModels.end() )	return physicsModels[ Name ];
		if ( loaderPhysicsModels.empty() )							throw std::runtime_error( "No physics model loaders" );

		g_consoleSystem->PrintInfo( "Loading physics model [%s] with name [%s]", Path, Name );

		std::string			format = GetFormatFile( Path );
		if ( format.empty() )								throw std::runtime_error( "In collider format not found" );

		auto				parser = loaderPhysicsModels.find( format );
		if ( parser == loaderPhysicsModels.end() )			throw std::runtime_error( "Loader for format physics model not found" );

		le::IPhysicsModel*			physicsModel = nullptr;
		for ( UInt32_t index = 0, count = paths.size(); index < count; ++index )
		{
			physicsModel = parser->second( std::string( paths[ index ] + "/" + Path ).c_str(), g_physicsSystemFactory );
			if ( physicsModel )
			{
				g_consoleSystem->PrintInfo( "Physics model founded in [%s]", paths[ index ].c_str() );
				break;
			}
		}

		if ( !physicsModel )		throw std::runtime_error( "Fail loading physics model" );
		physicsModel->IncrementReference();
		physicsModels.insert( std::make_pair( Name, physicsModel ) );
		g_consoleSystem->PrintInfo( "Loaded collider [%s]", Name );

		return physicsModel;
	}
	catch ( std::exception & Exception )
	{
		g_consoleSystem->PrintError( "Physics model [%s] not loaded: %s", Path, Exception.what() );
		return nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Load gpu program
// ------------------------------------------------------------------------------------ //
le::IGPUProgram* le::ResourceSystem::LoadGPUProgram( const char* Name, const char* Path, UInt32_t Flags, UInt32_t CountDefines, const char** Defines )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Path );

	try
	{
		if ( !studioRenderFactory )							throw std::runtime_error( "Resource system not initialized" );

		// Find loaded gpu program
		{
			auto			itShaders = gpuPrograms.find( Name );
			if ( itShaders != gpuPrograms.end() )
			{
				auto		itShader = itShaders->second.find( Flags );
				if ( itShader != itShaders->second.end() )
					return itShader->second;
			}
		}

		if ( loaderGPUProgram.empty() )						throw std::runtime_error( "No gpu program loaders" );

		g_consoleSystem->PrintInfo( "Loading gpu program [%s] with name [%s] and flags [%i]", Path, Name, Flags );

		std::string			format = GetFormatFile( Path );
		if ( format.empty() )						throw std::runtime_error( "In gpu program format not found" );

		auto				parser = loaderGPUProgram.find( format );
		if ( parser == loaderGPUProgram.end() )		throw std::runtime_error( "Loader for format gpu program not found" );

		IGPUProgram*			gpuProgram = nullptr;
		for ( UInt32_t index = 0, count = paths.size(); index < count; ++index )
		{
			gpuProgram = parser->second( paths[ index ].c_str(), std::string( paths[ index ] + "/" + Path ).c_str(), CountDefines, Defines, studioRenderFactory );
			if ( gpuProgram )
			{
				g_consoleSystem->PrintInfo( "GPU program founded in [%s]", paths[ index ].c_str() );
				break;
			}
		}

		if ( !gpuProgram )							throw std::runtime_error( "Fail loading gpu program" );
		gpuProgram->IncrementReference();
		gpuPrograms[ Name ][ Flags ] = gpuProgram;
		g_consoleSystem->PrintInfo( "Loaded gpu program [%s] with flags [%i]", Name, Flags );

		return gpuProgram;
	}
	catch ( std::exception & Exception )
	{
		g_consoleSystem->PrintError( "Gpu program [%s] with flags [%i] not loaded: %s", Path, Flags, Exception.what() );
		return nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Unload physics models
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadPhysicsModels()
{
	if ( physicsModels.empty() ) return;

	for ( auto it = physicsModels.begin(), itEnd = physicsModels.end(); it != itEnd; )
		if ( it->second->GetCountReferences() <= 1 )
		{
			it->second->Release();

			g_consoleSystem->PrintInfo( "Unloaded physics model [%s]", it->first.c_str() );
			it = physicsModels.erase( it );
			itEnd = physicsModels.end();
		}
		else
			++it;
}

// ------------------------------------------------------------------------------------ //
// Unload all gpu programs
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadGPUPrograms()
{
	if ( gpuPrograms.empty() ) return;

	for ( auto it = gpuPrograms.begin(), itEnd = gpuPrograms.end(); it != itEnd; )
	{
		for ( auto itShader = it->second.begin(), itShaderEnd = it->second.end(); itShader != itShaderEnd; )
			if ( itShader->second->GetCountReferences() <= 1 )
			{
				itShader->second->Release();

				g_consoleSystem->PrintInfo( "Unloaded gpu program [%s] with flags [%i]", it->first.c_str(), itShader->first );
				itShader = it->second.erase( itShader );
				itShaderEnd = it->second.end();
			}
			else
				++itShader;

		if ( it->second.empty() )
		{
			it = gpuPrograms.erase( it );
			itEnd = gpuPrograms.end();
		}
		else
			++it;
	}
}

// ------------------------------------------------------------------------------------ //
// Get physics model
// ------------------------------------------------------------------------------------ //
le::IPhysicsModel* le::ResourceSystem::GetPhysicsModel( const char* Name ) const
{
	LIFEENGINE_ASSERT( Name );

	auto	it = physicsModels.find( Name );
	if ( it != physicsModels.end() )	return it->second;

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Get gpu program
// ------------------------------------------------------------------------------------ //
le::IGPUProgram* le::ResourceSystem::GetGPUProgram( const char* Name, UInt32_t Flags ) const
{
	LIFEENGINE_ASSERT( Name );

	auto		itShaders = gpuPrograms.find( Name );
	if ( itShaders == gpuPrograms.end() )		return nullptr;

	auto		itShader = itShaders->second.find( Flags );
	if ( itShader == itShaders->second.end() )	return nullptr;

	return itShader->second;
}

// ------------------------------------------------------------------------------------ //
// Get count paths
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ResourceSystem::GetCountPaths() const
{
	return paths.size();
}

// ------------------------------------------------------------------------------------ //
// Get path
// ------------------------------------------------------------------------------------ //
const char* le::ResourceSystem::GetPath( UInt32_t Index ) const
{
	if ( Index >= paths.size() )		return "";
	return paths[ Index ].c_str();
}