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

#include "engine/lifeengine.h"
#include "engine/iengine.h"
#include "engine/ifont.h"
#include "engine/imaterial.h"
#include "engine/iscript.h"
#include "engine/ilevel.h"
#include "studiorender/istudiorender.h"
#include "studiorender/igpuprogram.h"
#include "studiorender/itexture.h"
#include "studiorender/imesh.h"
#include "physics/iphysicsmodel.h"
#include "audio/iaudiosystem.h"
#include "audio/isoundbuffer.h"
#include "audio/istreamsoundinternal.h"

#include "global.h"
#include "materialsystem.h"
#include "scriptsystem.h"
#include "consolesystem.h"
#include "resourcesystem.h"

<<<<<<< HEAD
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
le::IMesh* LE_LoadMesh ( const char* Path, le::IResourceSystem* ResourceSystem, le::IFactory* StudioRenderFactory )
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
=======
// Parsers
#include "parsergpuprogram_shader.h"
#include "parserfont_freetype.h"
#include "parserimage_freeimage.h"
#include "parserlevel_bsp.h"
#include "parsermaterial_lmt.h"
#include "parsermesh_mdl.h"
#include "parserphysicsmodel_phy.h"
#include "parserscript_c.h"
#include "parsertexture_freeimage.h"
#include "parsersoundbuffer_ogg.h"
>>>>>>> remotes/origin/dev

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
		g_consoleSystem->PrintInfo( "Loading image [%s]", Path );

		std::string			format = GetFormatFile( Path );
		if ( format.empty() )						throw std::runtime_error( "In image format not found" );

		auto				parser = loaderImages.find( format );
		if ( parser == loaderImages.end() )			throw std::runtime_error( "Loader for format image not found" );

		Image				image;
		IParserImage*		parserImage = parser->second();

		for ( UInt32_t index = 0, count = paths.size(); !image.data && index < count; ++index )
			parserImage->Read( std::string( paths[ index ] + "/" + Path ).c_str(), image, IsError, IsFlipVertical, IsSwitchRedAndBlueChannels );

		parserImage->Release();
		if ( IsError )								throw std::runtime_error( "Fail loading image" );
		
		g_consoleSystem->PrintInfo( "Loaded image [%s]", Path );
		return image;
	}
	catch ( std::exception& Exception )
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
		IParserTexture*		parserTexture = parser->second();

		for ( UInt32_t index = 0, count = paths.size(); !texture && index < count; ++index )
			texture = parserTexture->Read( std::string( paths[ index ] + "/" + Path ).c_str(), studioRenderFactory );

		parserTexture->Release();
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
		IParserMaterial*	parserMaterial = parser->second();

		for ( UInt32_t index = 0, count = paths.size(); !material && index < count; ++index )
			material = parserMaterial->Read( std::string( paths[ index ] + "/" + Path ).c_str(), this, g_materialSystem, engineFactory );

		parserMaterial->Release();
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
		IParserMesh*	parserMesh = parser->second();

		for ( UInt32_t index = 0, count = paths.size(); !mesh && index < count; ++index )
			mesh = parserMesh->Read( std::string( paths[ index ] + "/" + Path ).c_str(), this, studioRenderFactory );

		parserMesh->Release();
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
		IParserLevel*	parserLevel = parser->second();

		for ( UInt32_t index = 0, count = paths.size(); !level && index < count; ++index )
			level = parserLevel->Read( std::string( paths[ index ] + "/" + Path ).c_str(), GameFactory );	

		parserLevel->Release();
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
		IParserFont*	parserFont = parser->second();

		for ( UInt32_t index = 0, count = paths.size(); !font && index < count; ++index )
			font = parserFont->Read( std::string( paths[ index ] + "/" + Path ).c_str() );

		parserFont->Release();
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
	UnloadSoundBuffers();
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
		IStudioRender*			studioRender = Engine->GetStudioRender();
		if ( !studioRender )	throw std::runtime_error( "Resource system requared studiorender" );

		studioRenderFactory = studioRender->GetFactory();
		scriptSystemFactory = g_scriptSystem->GetFactory();
		audioSystemFactory = Engine->GetAudioSystem()->GetFactory();
		engineFactory = Engine->GetFactory();

		if ( !ParserFontFreeType::InitializeFreeType() )
			throw std::runtime_error( "Failed initialize freetype library" );

		RegisterLoader( []() -> IParserImage* { return new ParserImageFreeImage(); } );
		RegisterLoader( []() -> IParserFont* { return new ParserFontFreeType(); } );
		RegisterLoader( []() -> IParserGPUProgram* { return new ParserGPUProgramShader(); } );
		RegisterLoader( []() -> IParserLevel* { return new ParserLevelBSP(); } );
		RegisterLoader( []() -> IParserMaterial* { return new ParserMaterialLMT(); } );
		RegisterLoader( []() -> IParserMesh* { return new ParserMeshMDL(); } );
		RegisterLoader( []() -> IParserPhysicsModel* { return new ParserPhysicsModelPHY(); } );
		RegisterLoader( []() -> IParserScript* { return new ParserScriptC(); } );
		RegisterLoader( []() -> IParserTexture* { return new ParserTextureFreeImage(); } );
		RegisterLoader( []() -> IParserSoundBuffer* { return new ParserSoundBufferOGG(); } );
	}
	catch ( std::exception& Exception )
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
		IParserScript*		parserScript = parser->second();

		for ( UInt32_t index = 0, count = paths.size(); !script && index < count; ++index )
			script = parserScript->Read( std::string( paths[ index ] + "/" + Path ).c_str(), CountFunctions, Functions, CountVars, Vars, scriptSystemFactory );

		parserScript->Release();
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
// Unload sound buffer
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadSoundBuffer( const char* Name )
{
	LIFEENGINE_ASSERT( Name );

	auto				it = soundBuffers.find( Name );
	if ( it == soundBuffers.end() || it->second->GetCountReferences() > 1 )		return;

	it->second->Release();
	soundBuffers.erase( it );

	g_consoleSystem->PrintInfo( "Unloaded sound buffer [%s]", Name );
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
		IParserPhysicsModel*		parserPhysicsModel = parser->second();

		for ( UInt32_t index = 0, count = paths.size(); !physicsModel && index < count; ++index )
			physicsModel = parserPhysicsModel->Read( std::string( paths[ index ] + "/" + Path ).c_str(), g_physicsSystemFactory );

		parserPhysicsModel->Release();
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
		IParserGPUProgram*		parserGPUProgram = parser->second();

		for ( UInt32_t index = 0, count = paths.size(); !gpuProgram && index < count; ++index )		
			gpuProgram = parserGPUProgram->Read( paths[ index ].c_str(), std::string( paths[ index ] + "/" + Path ).c_str(), CountDefines, Defines, studioRenderFactory );

		parserGPUProgram->Release();
		if ( !gpuProgram )							throw std::runtime_error( "Fail loading gpu program" );
		
		gpuProgram->IncrementReference();
		gpuPrograms[ Name ][ Flags ] = gpuProgram;
		g_consoleSystem->PrintInfo( "Loaded gpu program [%s] with flags [%i]", Name, Flags );

		return gpuProgram;
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( "Gpu program [%s] with flags [%i] not loaded: %s", Path, Flags, Exception.what() );
		return nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Load sound buffer
// ------------------------------------------------------------------------------------ //
le::ISoundBuffer* le::ResourceSystem::LoadSoundBuffer( const char* Name, const char* Path )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Path );

	try
	{
		if ( soundBuffers.find( Name ) != soundBuffers.end() )		return soundBuffers[ Name ];
		if ( loaderSoundBuffers.empty() )							throw std::runtime_error( "No sound buffer loaders" );

		g_consoleSystem->PrintInfo( "Loading sound buffer [%s] with name [%s]", Path, Name );

		std::string			format = GetFormatFile( Path );
		if ( format.empty() )								throw std::runtime_error( "In sound buffer format not found" );

		auto				parser = loaderSoundBuffers.find( format );
		if ( parser == loaderSoundBuffers.end() )			throw std::runtime_error( "Loader for format sound buffer not found" );

		le::ISoundBuffer*		soundBuffer = nullptr;
		IParserSoundBuffer*		parserSoundBuffer = parser->second();

		for ( UInt32_t index = 0, count = paths.size(); !soundBuffer && index < count; ++index )
		{				
			if ( !parserSoundBuffer->Open( std::string( paths[ index ] + "/" + Path ).c_str() ) )
				continue;

			UInt32_t					sizeData = parserSoundBuffer->GetSampleCount();
			Byte_t*						data = ( Byte_t* ) malloc( sizeData );		
			if ( parserSoundBuffer->Read( data, sizeData ) == 0 )
			{
				free( data );
				continue;
			}

			soundBuffer = ( le::ISoundBuffer* ) audioSystemFactory->Create( SOUNDBUFFER_INTERFACE_VERSION );
			if ( !soundBuffer )
			{
				free( data );
				parserSoundBuffer->Release();
				
				throw std::runtime_error( "Interface le::ISoundBuffer [" SOUNDBUFFER_INTERFACE_VERSION "] not founded in audio system" );
			}
			
			soundBuffer->Create();
			soundBuffer->Append( parserSoundBuffer->GetSampleFormat(), data, sizeData, parserSoundBuffer->GetSampleRate() );
			
			free( data );
		}

		parserSoundBuffer->Release();
		if ( !soundBuffer )		throw std::runtime_error( "Fail loading sound buffer" );		
		
		soundBuffer->IncrementReference();
		soundBuffers.insert( std::make_pair( Name, soundBuffer ) );	
		g_consoleSystem->PrintInfo( "Loaded sound buffer [%s]", Name );
		
		return soundBuffer;
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( "Sound buffer [%s] not loaded: %s", Path, Exception.what() );
		return nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Open stream sound
// ------------------------------------------------------------------------------------ //
le::IStreamSound* le::ResourceSystem::OpenStreamSound( const char* Path )
{
	LIFEENGINE_ASSERT( Path );

	try
	{
		if ( loaderSoundBuffers.empty() )		throw std::runtime_error( "No sound buffer loaders" );

		g_consoleSystem->PrintInfo( "Opening stream sound [%s]", Path );

		std::string			format = GetFormatFile( Path );
		if ( format.empty() )								throw std::runtime_error( "In sound buffer format not found" );

		auto				parser = loaderSoundBuffers.find( format );
		if ( parser == loaderSoundBuffers.end() )			throw std::runtime_error( "Loader for format sound buffer not found" );

		IStreamSoundInternal*		streamSound = nullptr;
		IParserSoundBuffer*			parserSoundBuffer = parser->second();

		for ( UInt32_t index = 0, count = paths.size(); !streamSound && index < count; ++index )
		{		
			if ( !parserSoundBuffer->Open( std::string( paths[ index ] + "/" + Path ).c_str() ) )
				continue;

			streamSound = ( IStreamSoundInternal* ) audioSystemFactory->Create( STREAMSOUND_INTERFACE_VERSION );
			if ( !streamSound )
			{
				parserSoundBuffer->Release();
				throw std::runtime_error( "interface le::IStreamSound [" STREAMSOUND_INTERFACE_VERSION "] not founded in audio system" );
			}

			streamSound->Create();
			streamSound->Open( parserSoundBuffer );
		}

		if ( !streamSound )
		{
			parserSoundBuffer->Release();
			throw std::runtime_error( "Fail opening stream sound" );
		}

		g_consoleSystem->PrintInfo( "Opened stream sound" );
		return streamSound;
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( "Stream sound [%s] not opened: %s", Path, Exception.what() );
		return nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Open stream sound in allrady created IStreamSound
// ------------------------------------------------------------------------------------ //
bool le::ResourceSystem::OpenStreamSound( const char* Path, IStreamSound* StreamSound )
{
	LIFEENGINE_ASSERT( Path );
	LIFEENGINE_ASSERT( StreamSound );

	try
	{
		if ( loaderSoundBuffers.empty() )		throw std::runtime_error( "No sound buffer loaders" );

		g_consoleSystem->PrintInfo( "Opening stream sound [%s]", Path );

		std::string			format = GetFormatFile( Path );
		if ( format.empty() )								throw std::runtime_error( "In sound buffer format not found" );

		auto				parser = loaderSoundBuffers.find( format );
		if ( parser == loaderSoundBuffers.end() )			throw std::runtime_error( "Loader for format sound buffer not found" );

		bool					isSuccess = false;
		IParserSoundBuffer*		parserSoundBuffer = parser->second();
		
		for ( UInt32_t index = 0, count = paths.size(); !isSuccess && index < count; ++index )
		{		
			if ( !parserSoundBuffer->Open( std::string( paths[ index ] + "/" + Path ).c_str() ) )
				continue;

			static_cast< IStreamSoundInternal* >( StreamSound )->Open( parserSoundBuffer );
			isSuccess = true;
		}

		if ( !isSuccess )
		{
			parserSoundBuffer->Release();
			throw std::runtime_error( "Fail opening stream sound" );
		}

		g_consoleSystem->PrintInfo( "Opened stream sound" );
		return isSuccess;
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( "Stream sound [%s] not opened: %s", Path, Exception.what() );
		return false;
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
// Unload sound buffers
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadSoundBuffers()
{
	if ( soundBuffers.empty() ) return;

	for ( auto it = soundBuffers.begin(), itEnd = soundBuffers.end(); it != itEnd; )
		if ( it->second->GetCountReferences() <= 1 )
		{
			it->second->Release();

			g_consoleSystem->PrintInfo( "Unloaded sound buffer [%s]", it->first.c_str() );
			it = soundBuffers.erase( it );
			itEnd = soundBuffers.end();
		}
		else
			++it;
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
// Get sound buffer
// ------------------------------------------------------------------------------------ //
le::ISoundBuffer* le::ResourceSystem::GetSoundBuffer( const char* Name ) const
{
	LIFEENGINE_ASSERT( Name );

	auto	it = soundBuffers.find( Name );
	if ( it != soundBuffers.end() )		return it->second;

	return nullptr;
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

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( CreateParserImageFn_t CreateParserImage )
{
	if ( !CreateParserImage )		return;

	IParserImage*		parserImage = CreateParserImage();
	const char**		extensions = parserImage->GetFileExtensions();
	UInt32_t			countExtensions = parserImage->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser image not registered, supported extensions not founded" );
		parserImage->Release();
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderImages[ extensions[ index ] ] = CreateParserImage;

	g_consoleSystem->PrintInfo( "Parser image registered. Name: %s, Version: %s, Author: %s", parserImage->GetName(), parserImage->GetVersion(), parserImage->GetAuthor() );
	parserImage->Release();
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( CreateParserTextureFn_t CreateParserTexture )
{
	if ( !CreateParserTexture )		return;

	IParserTexture*		parserTexture = CreateParserTexture();
	const char**		extensions = parserTexture->GetFileExtensions();
	UInt32_t			countExtensions = parserTexture->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser texture not registered, supported extensions not founded" );
		parserTexture->Release();
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderTextures[ extensions[ index ] ] = CreateParserTexture;

	g_consoleSystem->PrintInfo( "Parser texture registered. Name: %s, Version: %s, Author: %s", parserTexture->GetName(), parserTexture->GetVersion(), parserTexture->GetAuthor() );
	parserTexture->Release();
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( CreateParserMaterialFn_t CreateParserMaterial )
{
	if ( !CreateParserMaterial )		return;

	IParserMaterial*	parserMaterial = CreateParserMaterial();
	const char**		extensions = parserMaterial->GetFileExtensions();
	UInt32_t			countExtensions = parserMaterial->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser material not registered, supported extensions not founded" );
		parserMaterial->Release();
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderMaterials[ extensions[ index ] ] = CreateParserMaterial;

	g_consoleSystem->PrintInfo( "Parser material registered. Name: %s, Version: %s, Author: %s", parserMaterial->GetName(), parserMaterial->GetVersion(), parserMaterial->GetAuthor() );
	parserMaterial->Release();
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( CreateParserMeshFn_t CreateParserMesh )
{
	if ( !CreateParserMesh )		return;

	IParserMesh*		parserMesh = CreateParserMesh();
	const char**		extensions = parserMesh->GetFileExtensions();
	UInt32_t			countExtensions = parserMesh->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser mesh not registered, supported extensions not founded" );
		parserMesh->Release();
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderMeshes[ extensions[ index ] ] = CreateParserMesh;

	g_consoleSystem->PrintInfo( "Parser mesh registered. Name: %s, Version: %s, Author: %s", parserMesh->GetName(), parserMesh->GetVersion(), parserMesh->GetAuthor() );
	parserMesh->Release();
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( CreateParserLevelFn_t CreateParserLevel )
{
	if ( !CreateParserLevel )		return;

	IParserLevel*		parserLevel = CreateParserLevel();
	const char**		extensions = parserLevel->GetFileExtensions();
	UInt32_t			countExtensions = parserLevel->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser level not registered, supported extensions not founded" );
		parserLevel->Release();
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderLevels[ extensions[ index ] ] = CreateParserLevel;

	g_consoleSystem->PrintInfo( "Parser level registered. Name: %s, Version: %s, Author: %s", parserLevel->GetName(), parserLevel->GetVersion(), parserLevel->GetAuthor() );
	parserLevel->Release();
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( CreateParserFontFn_t CreateParserFont )
{
	if ( !CreateParserFont )		return;

	IParserFont*		parseFont = CreateParserFont();
	const char**		extensions = parseFont->GetFileExtensions();
	UInt32_t			countExtensions = parseFont->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser font not registered, supported extensions not founded" );
		parseFont->Release();
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderFonts[ extensions[ index ] ] = CreateParserFont;

	g_consoleSystem->PrintInfo( "Parser font registered. Name: %s, Version: %s, Author: %s", parseFont->GetName(), parseFont->GetVersion(), parseFont->GetAuthor() );
	parseFont->Release();
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( CreateParserScriptFn_t CreateParserScript )
{
	if ( !CreateParserScript )		return;

	IParserScript*		parserScript = CreateParserScript();
	const char**		extensions = parserScript->GetFileExtensions();
	UInt32_t			countExtensions = parserScript->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser script not registered, supported extensions not founded" );
		parserScript->Release();
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderScripts[ extensions[ index ] ] = CreateParserScript;

	g_consoleSystem->PrintInfo( "Parser script registered. Name: %s, Version: %s, Author: %s", parserScript->GetName(), parserScript->GetVersion(), parserScript->GetAuthor() );
	parserScript->Release();
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( CreateParserPhysicsModelFn_t CreateParserPhysicsModel )
{
	if ( !CreateParserPhysicsModel )		return;

	IParserPhysicsModel*		parserPhysicsModel = CreateParserPhysicsModel();
	const char**				extensions = parserPhysicsModel->GetFileExtensions();
	UInt32_t					countExtensions = parserPhysicsModel->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser physics model not registered, supported extensions not founded" );
		parserPhysicsModel->Release();
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderPhysicsModels[ extensions[ index ] ] = CreateParserPhysicsModel;

	g_consoleSystem->PrintInfo( "Parser physics model registered. Name: %s, Version: %s, Author: %s", parserPhysicsModel->GetName(), parserPhysicsModel->GetVersion(), parserPhysicsModel->GetAuthor() );
	parserPhysicsModel->Release();
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( CreateParserGPUProgramFn_t CreateParserGPUProgram )
{
	if ( !CreateParserGPUProgram )		return;

	IParserGPUProgram*		parserGPUProgram = CreateParserGPUProgram();
	const char**			extensions = parserGPUProgram->GetFileExtensions();
	UInt32_t				countExtensions = parserGPUProgram->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser GPU program not registered, supported extensions not founded" );
		parserGPUProgram->Release();
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderGPUProgram[ extensions[ index ] ] = CreateParserGPUProgram;

	g_consoleSystem->PrintInfo( "Parser GPU program registered. Name: %s, Version: %s, Author: %s", parserGPUProgram->GetName(), parserGPUProgram->GetVersion(), parserGPUProgram->GetAuthor() );
	parserGPUProgram->Release();
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( CreateParserSoundBufferFn_t CreateParserSoundBuffer )
{
	if ( !CreateParserSoundBuffer )		return;

	IParserSoundBuffer*		parserSoundBuffer = CreateParserSoundBuffer();
	const char**			extensions = parserSoundBuffer->GetFileExtensions();
	UInt32_t				countExtensions = parserSoundBuffer->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser sound buffer not registered, supported extensions not founded" );
		parserSoundBuffer->Release();
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderSoundBuffers[ extensions[ index ] ] = CreateParserSoundBuffer;

	g_consoleSystem->PrintInfo( "Parser sound buffer registered. Name: %s, Version: %s, Author: %s", parserSoundBuffer->GetName(), parserSoundBuffer->GetVersion(), parserSoundBuffer->GetAuthor() );
	parserSoundBuffer->Release();
}