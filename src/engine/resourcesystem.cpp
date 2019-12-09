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
#include <FreeImage/FreeImage.h>

#include "common/image.h"
#include "engine/lifeengine.h"
#include "engine/engine.h"
#include "studiorender/istudiorender.h"
#include "studiorender/itexture.h"

#include "global.h"
#include "consolesystem.h"
#include "resourcesystem.h"

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
// Зарегестрировать парсер картинок
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterParser_Image( const char* Format, LoadImageFn_t LoadImage )
{
	LIFEENGINE_ASSERT( Format );
	LIFEENGINE_ASSERT( LoadImage );

	g_consoleSystem->PrintInfo( "Parser image for format [%s] registered", Format );
	parserImages[ Format ] = LoadImage;
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать парсер текстур
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterParser_Texture( const char* Format, LoadTextureFn_t LoadTexture )
{
	LIFEENGINE_ASSERT( Format );
	LIFEENGINE_ASSERT( LoadTexture );

	g_consoleSystem->PrintInfo( "Parser texture for format [%s] registered", Format );
	parserTextures[ Format ] = LoadTexture;
}

// ------------------------------------------------------------------------------------ //
// Разрегестрировать парсер картинок
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterParser_Image( const char* Format )
{
	LIFEENGINE_ASSERT( Format );

	auto		it = parserImages.find( Format );
	if ( it == parserImages.end() ) return;

	parserImages.erase( it );
	g_consoleSystem->PrintInfo( "Parser image for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Разрегестрировать парсер текстур
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterParser_Texture( const char* Format )
{
	LIFEENGINE_ASSERT( Format );

	auto		it = parserTextures.find( Format );
	if ( it == parserTextures.end() ) return;

	parserTextures.erase( it );
	g_consoleSystem->PrintInfo( "Parser texture for format [%s] unregistered", Format );
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
		if ( parserImages.empty() )					throw std::exception( "No image parsers" );
		std::string			path = gameDir + "/" + Path;

		const char* format = strchr( Path, '.' );
		if ( !format )							throw std::exception( "In image format not found" );

		auto				parser = parserImages.find( format + 1 );
		if ( parser == parserImages.end() )			throw std::exception( "Parser for format image not found" );

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
le::ITexture* le::ResourceSystem::LoadTexture( const char* Name, const char* Group, const char* Path )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Group );
	LIFEENGINE_ASSERT( Path );

	try
	{
		if ( !studioRenderFactory )					throw std::exception( "Resource system not initialized" );

		auto				group = textures[ Group ];
		if ( group.find( Name ) != group.end() )	return group[ Name ];
		if ( parserTextures.empty() )				throw std::exception( "No texture parsers" );

		std::string			path = gameDir + "/" + Path;

		g_consoleSystem->PrintInfo( "Loading texture [%s] with name [%s] in group [%s]", Path, Name, Group );

		const char* format = strchr( Path, '.' );
		if ( !format )								throw std::exception( "In texture format not found" );

		auto				parser = parserTextures.find( format + 1 );
		if ( parser == parserTextures.end() )		throw std::exception( "Parser for format texture not found" );

		ITexture*			texture = parser->second( path.c_str(), studioRenderFactory );
		if ( !texture )								throw std::exception( "Fail loading texture" );

		group.insert( std::make_pair( Path, texture ) );
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
void le::ResourceSystem::UnloadTexture( const char* Name, const char* Group )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Group );

	if ( !studioRenderFactory )
	{
		g_consoleSystem->PrintError( "Resource system not initialized" );
		return;
	}

	auto				group = textures[ Group ];
	auto				it = group.find( Name );

	if ( it == group.end() )	return;

	studioRenderFactory->Delete( it->second );
	group.erase( it );

	g_consoleSystem->PrintInfo( "Unloaded texture [%s] in group [%s]", Name, Group );
}

// ------------------------------------------------------------------------------------ //
// Выгрузить группу текстур
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadTextures( const char* Group )
{
	if ( !studioRenderFactory )
	{
		g_consoleSystem->PrintError( "Resource system not initialized" );
		return;
	}

	auto		group = textures[ Group ];
	if ( group.empty() )		return;

	for ( auto it = group.begin(), itEnd = group.end(); it != itEnd; ++it )
		studioRenderFactory->Delete( it->second );

	if ( !group.empty() )		g_consoleSystem->PrintInfo( "Unloaded all textures in group [%s]", Group );
	group.clear();	
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все текстуры
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadTextures()
{
	if ( !studioRenderFactory )
	{
		g_consoleSystem->PrintError( "Resource system not initialized" );
		return;
	}

	for ( auto itGroup = textures.begin(), itEndGroup = textures.end(); itGroup != itEndGroup; ++itGroup )
		for ( auto itTexture = itGroup->second.begin(), itEndTexture = itGroup->second.end(); itTexture != itEndTexture; ++itTexture )
			studioRenderFactory->Delete( itTexture->second );

	if ( !textures.empty() )		g_consoleSystem->PrintInfo( "Unloaded all textures" );
	textures.clear();	
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все ресурсы
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadAll()
{
	UnloadTextures();
}

// ------------------------------------------------------------------------------------ //
//  Получить текстуру
// ------------------------------------------------------------------------------------ //
le::ITexture* le::ResourceSystem::GetTexture( const char* Name, const char* Group ) const
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Group );

	auto				group = textures.at( Group );
	auto				it = group.find( Name );
	if ( it != group.end() )		return it->second;

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

		studioRenderFactory = studioRender->GetFactory();

		RegisterParser_Image( "png", LE_LoadImage );
		RegisterParser_Image( "jpg", LE_LoadImage );
		RegisterParser_Texture( "png", LE_LoadTexture );
		RegisterParser_Texture( "jpg", LE_LoadTexture );
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
	studioRenderFactory( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ResourceSystem::~ResourceSystem()
{
	UnloadAll();
}
