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
#include <FreeImage/FreeImage.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "common/image.h"
#include "common/meshsurface.h"
#include "common/meshdescriptor.h"
#include "engine/lifeengine.h"
#include "engine/engine.h"
#include "materialsystem/imaterialsystem.h"
#include "materialsystem/imaterial.h"
#include "materialsystem/imaterialvar.h"
#include "studiorender/istudiorender.h"
#include "studiorender/itexture.h"
#include "studiorender/imesh.h"
#include "studiorender/studiovertexelement.h"

#include "global.h"
#include "consolesystem.h"
#include "resourcesystem.h"

struct Vertex
{
	le::Vector3D_t			position;
	le::Vector3D_t			normal;
	le::Vector2D_t			texCoords;
	le::Vector3D_t			tangent;
	le::Vector3D_t			bitangent;
};

// ------------------------------------------------------------------------------------ //
// Преобразовать JSON массив в 2D вектор 
// ------------------------------------------------------------------------------------ //
inline le::Vector2D_t JsonArrayToVec2( rapidjson::Value::Array& Array )
{
	try
	{
		if ( Array.Size() < 2 ) throw;

		for ( size_t index = 0, count = Array.Size(); index < count; ++index )
			if ( !Array[ index ].IsNumber() )
				throw;
	}
	catch ( ... )
	{
		return le::Vector2D_t( 0.f, 0.f );
	}

	return le::Vector2D_t( Array[ 0 ].GetFloat(), Array[ 1 ].GetFloat() );
}

// ------------------------------------------------------------------------------------ //
// Преобразовать JSON массив в 3D вектор 
// ------------------------------------------------------------------------------------ //
inline le::Vector3D_t JsonArrayToVec3( rapidjson::Value::Array& Array )
{
	try
	{
		if ( Array.Size() < 3 ) throw;

		for ( size_t index = 0, count = Array.Size(); index < count; ++index )
			if ( !Array[ index ].IsNumber() )
				throw;
	}
	catch ( ... )
	{
		return le::Vector3D_t( 0.f, 0.f, 0.f );
	}

	return le::Vector3D_t( Array[ 0 ].GetFloat(), Array[ 1 ].GetFloat(), Array[ 2 ].GetFloat() );
}

// ------------------------------------------------------------------------------------ //
// Преобразовать JSON массив в 4D вектор 
// ------------------------------------------------------------------------------------ //
inline le::Vector4D_t JsonArrayToVec4( rapidjson::Value::Array& Array )
{
	try
	{
		if ( Array.Size() < 4 ) throw;

		for ( size_t index = 0, count = Array.Size(); index < count; ++index )
			if ( !Array[ index ].IsNumber() )
				throw;
	}
	catch ( ... )
	{
		return le::Vector4D_t( 0.f, 0.f, 0.f, 0.f );
	}

	return le::Vector4D_t( Array[ 0 ].GetFloat(), Array[ 1 ].GetFloat(), Array[ 2 ].GetFloat(), Array[ 3 ].GetFloat() );
}

// ------------------------------------------------------------------------------------ //
// Преобразовать JSON массив в матрицу 4х4
// ------------------------------------------------------------------------------------ //
inline le::Matrix4x4_t JsonArrayToMatrix( rapidjson::Value::Array& Array )
{
	try
	{
		if ( Array.Size() < 16 ) throw;

		for ( size_t index = 0, count = Array.Size(); index < count; ++index )
			if ( !Array[ index ].IsNumber() )
				throw;
	}
	catch ( ... )
	{
		return le::Matrix4x4_t( 1.f );
	}

	return le::Matrix4x4_t( Array[ 0 ].GetFloat(), Array[ 1 ].GetFloat(), Array[ 2 ].GetFloat(), Array[ 3 ].GetFloat(),
							Array[ 4 ].GetFloat(), Array[ 5 ].GetFloat(), Array[ 6 ].GetFloat(), Array[ 7 ].GetFloat(),
							Array[ 8 ].GetFloat(), Array[ 9 ].GetFloat(), Array[ 10 ].GetFloat(), Array[ 11 ].GetFloat(),
							Array[ 12 ].GetFloat(), Array[ 13 ].GetFloat(), Array[ 14 ].GetFloat(), Array[ 15 ].GetFloat() );
}

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

	le::ITexture*		texture = ( le::ITexture* ) StudioRenderFactory->Create( TEXTURE_INTERFACE_VERSION );
	if ( !texture )			return nullptr;

	texture->Initialize( le::TT_2D, image.aMask > 0 ? le::IF_RGBA : le::IF_RGB, image.width, image.height );
	texture->Bind();
	texture->Append( image.data );
	texture->GenerateMipmaps();
	texture->Unbind();

	delete[] image.data;
	return texture;
}

// ------------------------------------------------------------------------------------ //
// Загрузить материал
// ------------------------------------------------------------------------------------ //
le::IMaterial* LE_LoadMaterial( const char* Path, le::IResourceSystem* ResourceSystem, le::IFactory* MaterialSystemFactory )
{
	std::ifstream		file( Path );
	if ( !file.is_open() )						return nullptr;

	std::string					stringBuffer;
	std::getline( file, stringBuffer, '\0' );

	rapidjson::Document			document;
	document.Parse( stringBuffer.c_str() );
	if ( document.HasParseError() )				return nullptr;

	std::unordered_map< std::string, rapidjson::Value >		materialVars;
	std::unordered_map< std::string, rapidjson::Value >		shaderParams;

	// Считываем все параметры материала в память
	for ( auto it = document.MemberBegin(), itEnd = document.MemberEnd(); it != itEnd; ++it )
	{
		// Название поверхности
		if ( strcmp( it->name.GetString(), "surface" ) == 0 && it->value.IsString() )
			materialVars[ "surface" ] = it->value;

		// Название шейдера
		else if ( strcmp( it->name.GetString(), "shader" ) == 0 && it->value.IsString() )
			materialVars[ "shader" ] = it->value;

		// Параметры шейдера
		else if ( it->value.IsObject() && strcmp( it->name.GetString(), "shader_parameters" ) == 0 )
			for ( auto itObject = it->value.MemberBegin(), itObjectEnd = it->value.MemberEnd(); itObject != itObjectEnd; ++itObject )
				shaderParams[ itObject->name.GetString() ] = itObject->value;
	}

	le::IMaterial*			material = ( le::IMaterial* ) MaterialSystemFactory->Create( MATERIAL_INTERFACE_VERSION );
	if ( !material )							return nullptr;

	// Записываем материалу параметры
	for ( auto it = materialVars.begin(), itEnd = materialVars.end(); it != itEnd; ++it )
	{
		// Название поверхности
		if ( it->first == "surface" && it->second.IsString() )
			material->SetSurfaceName( it->second.GetString() );

		// Название шейдера
		else if ( it->first == "shader" && it->second.IsString() )
			material->SetShader( it->second.GetString() );
	}

	// Записываем параметры шейдера
	for ( auto it = shaderParams.begin(), itEnd = shaderParams.end(); it != itEnd; ++it )
	{
		le::IMaterialVar*		materialVar = material->FindVar( it->first.c_str() );
		if ( !materialVar ) continue;

		if ( it->second.IsString() )
		{
			le::ITexture*		texture = ResourceSystem->LoadTexture( it->second.GetString(), it->second.GetString() );
			if ( !texture )	continue;

			materialVar->SetValueTexture( texture );
		}

		else if ( it->second.IsArray() )
		{
			rapidjson::Value::Array				array = it->second.GetArray();

			if ( array.Size() == 2 )			materialVar->SetValueVector2D( JsonArrayToVec2( array ) );
			else if ( array.Size() == 3 )		materialVar->SetValueVector3D( JsonArrayToVec3( array ) );
			else if ( array.Size() == 4 )		materialVar->SetValueVector4D( JsonArrayToVec4( array ) );
			else if ( array.Size() == 16 )		materialVar->SetValueMatrix( JsonArrayToMatrix( array ) );
		}

		else if ( it->second.IsBool() )			materialVar->SetValueShaderFlag( it->second.GetBool() );
		else if ( it->second.IsDouble() )		materialVar->SetValueFloat( it->second.GetDouble() );		
		else if ( it->second.IsFloat() )		materialVar->SetValueFloat( it->second.GetFloat() );		
		else if ( it->second.IsInt() )			materialVar->SetValueInt( it->second.GetInt() );
	}

	return material;
}

// ------------------------------------------------------------------------------------ //
// Загрузить меш
// ------------------------------------------------------------------------------------ //
le::IMesh* LE_LoadMesh( const char* Path, le::IResourceSystem* ResourceSystem, le::IFactory* StudioRenderFactory )
{
	std::ifstream				file( Path, std::ios::binary );
	if ( !file.is_open() )		return nullptr;

	// Читаем заголовок файла
	char						strId[ 3 ];
	le::UInt16_t				version = 0;

	const char					loader_strId[ 3 ] = { 'L', 'M', 'D' };
	const le::UInt16_t			loader_version = 2;

	file.read( strId, 3 );
	file.read( ( char* ) &version, sizeof( le::UInt16_t ) );
	if ( strncmp( strId, loader_strId, 3 ) != 0 || version != loader_version )		return nullptr;

	// Читаем все материалы модели
	le::UInt32_t						sizeString = 0;
	le::UInt32_t						sizeArrayMaterials = 0;
	std::string*						routeMaterial = nullptr;
	std::vector< std::string >			arrayRouteMaterials;
	std::vector< le::IMaterial* >		arrayMaterials;

	file.read( ( char* ) &sizeArrayMaterials, sizeof( le::UInt32_t ) );
	arrayRouteMaterials.resize( sizeArrayMaterials );

	for ( le::UInt32_t index = 0; index < sizeArrayMaterials; ++index )
	{
		routeMaterial = &arrayRouteMaterials[ index ];

		file.read( ( char* ) &sizeString, sizeof( le::UInt32_t ) );
		routeMaterial->resize( sizeString );
		file.read( &( *routeMaterial )[ 0 ], sizeString );
	}

	// Загружаем материалы
	for ( le::UInt32_t index = 0; index < sizeArrayMaterials; ++index )
	{
		le::IMaterial*			material = ResourceSystem->LoadMaterial( arrayRouteMaterials[ index ].c_str(), arrayRouteMaterials[ index ].c_str() );
		if ( !material ) continue;

		arrayMaterials.push_back( material );
	}

	// Читаем все вершины модели
	le::UInt32_t						sizeArrayVerteces;
	std::vector< Vertex >				arrayVerteces;

	file.read( ( char* ) &sizeArrayVerteces, sizeof( le::UInt32_t ) );
	arrayVerteces.resize( sizeArrayVerteces );
	
	if ( sizeArrayVerteces > 0 )
		file.read( ( char* ) &arrayVerteces[ 0 ], sizeArrayVerteces * sizeof( Vertex ) );
	else
		return nullptr;

	// Читаем все индексы модели
	le::UInt32_t						sizeArrayIndices;
	std::vector< le::UInt32_t >			arrayIndices;

	file.read( ( char* ) &sizeArrayIndices, sizeof( le::UInt32_t ) );
	arrayIndices.resize( sizeArrayIndices );

	if ( sizeArrayIndices > 0 )
		file.read( ( char* ) &arrayIndices[ 0 ], sizeArrayIndices * sizeof( le::UInt32_t ) );
	else
		return nullptr;

	// Читаем все сетки модели
	le::UInt32_t						sizeArraySurfaces;
	std::vector< le::MeshSurface >		arraySurfaces;

	file.read( ( char* ) &sizeArraySurfaces, sizeof( le::UInt32_t ) );
	if ( sizeArraySurfaces == 0 )		return nullptr;

	le::MeshSurface						surface;
	for ( le::UInt32_t index = 0; index < sizeArraySurfaces; ++index )
	{
		file.read( ( char* ) &surface.materialID, sizeof( le::UInt32_t ) );
		file.read( ( char* ) &surface.startIndex, sizeof( le::UInt32_t ) );
		file.read( ( char* ) &surface.countIndeces, sizeof( le::UInt32_t ) );
		arraySurfaces.push_back( surface );
	}

	if ( arraySurfaces.empty() )				return nullptr;

	// Создаем сам меш
	le::IMesh*				mesh = ( le::IMesh* ) StudioRenderFactory->Create( MESH_INTERFACE_VERSION );
	if ( !mesh )				return nullptr;

	// Создаем описание для формата вершин
	std::vector< le::StudioVertexElement >			vertexElements =
	{
		{ 3, le::VET_FLOAT },
		{ 3, le::VET_FLOAT },
		{ 2, le::VET_FLOAT },
		{ 3, le::VET_FLOAT },
		{ 3, le::VET_FLOAT }
	};

	// Создаем описание меша для загрузки его в модуль рендера
	le::MeshDescriptor				meshDescriptor;
	meshDescriptor.countIndeces = arrayIndices.size();
	meshDescriptor.countMaterials = arrayMaterials.size();
	meshDescriptor.countSurfaces = arraySurfaces.size();
	meshDescriptor.sizeVerteces = arrayVerteces.size() * sizeof( Vertex );

	meshDescriptor.indeces = arrayIndices.data();
	meshDescriptor.materials = arrayMaterials.data();
	meshDescriptor.surfaces = arraySurfaces.data();
	meshDescriptor.verteces = arrayVerteces.data();

	meshDescriptor.primitiveType = le::PT_TRIANGLES;
	meshDescriptor.countVertexElements = vertexElements.size();
	meshDescriptor.vertexElements = vertexElements.data();

	// Загружаем меш в GPU
	mesh->Create( meshDescriptor );
	if ( !mesh->IsCreated() )
	{
		StudioRenderFactory->Delete( mesh );
		return nullptr;
	}

	return mesh;
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
// Загрузить картинку
// ------------------------------------------------------------------------------------ //
le::Image le::ResourceSystem::LoadImage( const char* Path, bool& IsError, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels )
{
	LIFEENGINE_ASSERT( Path );
	IsError = false;

	try
	{
		if ( loaderImages.empty() )					throw std::exception( "No image loaders" );
		std::string			path = gameDir + "/" + Path;

		const char* format = strchr( Path, '.' );
		if ( !format )								throw std::exception( "In image format not found" );

		auto				parser = loaderImages.find( format + 1 );
		if ( parser == loaderImages.end() )			throw std::exception( "Loader for format image not found" );

		Image				image;	
		parser->second( path.c_str(), image, IsError, IsFlipVertical, IsSwitchRedAndBlueChannels );
		if ( IsError )								throw std::exception( "Fail loading image" );		
		
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
		if ( !studioRenderFactory )							throw std::exception( "Resource system not initialized" );

		if ( textures.find( Name ) != textures.end() )		return textures[ Name ];
		if ( loaderTextures.empty() )						throw std::exception( "No texture loaders" );

		std::string			path = gameDir + "/" + Path;

		g_consoleSystem->PrintInfo( "Loading texture [%s] with name [%s]", Path, Name );

		const char* format = strchr( Path, '.' );
		if ( !format )								throw std::exception( "In texture format not found" );

		auto				parser = loaderTextures.find( format + 1 );
		if ( parser == loaderTextures.end() )		throw std::exception( "Loader for format texture not found" );

		ITexture*			texture = parser->second( path.c_str(), studioRenderFactory );
		if ( !texture )								throw std::exception( "Fail loading texture" );

		textures.insert( std::make_pair( Name, texture ) );
		g_consoleSystem->PrintInfo( "Loaded texture [%s]", Name );

		return texture;
	}
	catch ( std::exception& Exception )
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
		if ( !materialSystemFactory )						throw std::exception( "Resource system not initialized" );

		if ( materials.find( Name ) != materials.end() )	return materials[ Name ];
		if ( loaderMaterials.empty() )						throw std::exception( "No material loaders" );

		std::string			path = gameDir + "/" + Path;

		g_consoleSystem->PrintInfo( "Loading material [%s] with name [%s]", Path, Name );

		const char* format = strchr( Path, '.' );
		if ( !format )								throw std::exception( "In material format not found" );

		auto				parser = loaderMaterials.find( format + 1 );
		if ( parser == loaderMaterials.end() )		throw std::exception( "Loader for format material not found" );

		IMaterial*			material = parser->second( path.c_str(), this, materialSystemFactory );
		if ( !material )							throw std::exception( "Fail loading material" );

		materials.insert( std::make_pair( Name, material ) );
		g_consoleSystem->PrintInfo( "Loaded material [%s]", Name );

		return material;
	}
	catch ( std::exception& Exception )
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
		if ( !studioRenderFactory )							throw std::exception( "Resource system not initialized" );

		if ( meshes.find( Name ) != meshes.end() )			return meshes[ Name ];
		if ( loaderMeshes.empty() )							throw std::exception( "No mesh loaders" );

		std::string			path = gameDir + "/" + Path;

		g_consoleSystem->PrintInfo( "Loading mesh [%s] with name [%s]", Path, Name );

		const char* format = strchr( Path, '.' );
		if ( !format )								throw std::exception( "In mesh format not found" );

		auto				parser = loaderMeshes.find( format + 1 );
		if ( parser == loaderMeshes.end() )		throw std::exception( "Loader for format material not found" );

		IMesh*				mesh = parser->second( path.c_str(), this, studioRenderFactory );
		if ( !mesh )							throw std::exception( "Fail loading mesh" );

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

	if ( !studioRenderFactory )
	{
		g_consoleSystem->PrintError( "Resource system not initialized" );
		return;
	}

	auto				it = textures.find( Name );
	if ( it == textures.end() )	return;

	studioRenderFactory->Delete( it->second );
	textures.erase( it );

	g_consoleSystem->PrintInfo( "Unloaded texture [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Выгрузить материал
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadMaterial( const char* Name )
{
	LIFEENGINE_ASSERT( Name );

	if ( !materialSystemFactory )
	{
		g_consoleSystem->PrintError( "Resource system not initialized" );
		return;
	}

	auto				it = materials.find( Name );
	if ( it == materials.end() )	return;

	materialSystemFactory->Delete( it->second );
	materials.erase( it );

	g_consoleSystem->PrintInfo( "Unloaded material [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Выгрузить меш
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadMesh( const char* Name )
{
	LIFEENGINE_ASSERT( Name );

	if ( !studioRenderFactory )
	{
		g_consoleSystem->PrintError( "Resource system not initialized" );
		return;
	}

	auto				it = meshes.find( Name );
	if ( it == meshes.end() )	return;

	studioRenderFactory->Delete( it->second );
	meshes.erase( it );

	g_consoleSystem->PrintInfo( "Unloaded mesh [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все материалы
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadMaterials()
{
	if ( materials.empty() ) return;

	if ( !materialSystemFactory )
	{
		g_consoleSystem->PrintError( "Resource system not initialized" );
		return;
	}

	for ( auto it = materials.begin(), itEnd = materials.end(); it != itEnd; ++it )
			materialSystemFactory->Delete( it->second );

	g_consoleSystem->PrintInfo( "Unloaded all materials" );
	materials.clear();
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все мешы
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadMeshes()
{
	if ( meshes.empty() ) return;

	if ( !studioRenderFactory )
	{
		g_consoleSystem->PrintError( "Resource system not initialized" );
		return;
	}

	for ( auto it = meshes.begin(), itEnd = meshes.end(); it != itEnd; ++it )
		studioRenderFactory->Delete( it->second );

	g_consoleSystem->PrintInfo( "Unloaded all meshes" );
	meshes.clear();
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

	for ( auto it = textures.begin(), itEnd = textures.end(); it != itEnd; ++it )
			studioRenderFactory->Delete( it->second );

	g_consoleSystem->PrintInfo( "Unloaded all textures" );
	textures.clear();	
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все ресурсы
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadAll()
{
	UnloadMeshes();	
	UnloadMaterials();
	UnloadTextures();
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
// Инициализировать систему ресурсов
// ------------------------------------------------------------------------------------ //
bool le::ResourceSystem::Initialize( IEngine* Engine )
{
	try
	{
		IStudioRender*			studioRender = Engine->GetStudioRender();
		if ( !studioRender )	throw std::exception( "Resource system requared studiorender" );

		IMaterialSystem*		materialSystem = Engine->GetMaterialSystem();
		if ( !materialSystem )	throw std::exception( "Resource system requared material system" );

		studioRenderFactory = studioRender->GetFactory();
		materialSystemFactory = materialSystem->GetFactory();

		RegisterLoader_Image( "png", LE_LoadImage );
		RegisterLoader_Image( "jpg", LE_LoadImage );
		RegisterLoader_Image( "tga", LE_LoadImage );

		RegisterLoader_Texture( "png", LE_LoadTexture );
		RegisterLoader_Texture( "jpg", LE_LoadTexture );
		RegisterLoader_Texture( "tga", LE_LoadTexture );

		RegisterLoader_Material( "lmt", LE_LoadMaterial );

		RegisterLoader_Mesh( "lmd", LE_LoadMesh );
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( Exception.what() );
		return false;
	}
	return true;
}

// ------------------------------------------------------------------------------------ //
// Задать каталог игры
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::SetGameDir( const char* GameDir )
{
	gameDir = GameDir;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ResourceSystem::ResourceSystem() :
	studioRenderFactory( nullptr ),
	materialSystemFactory( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ResourceSystem::~ResourceSystem()
{
	UnloadAll();
}
