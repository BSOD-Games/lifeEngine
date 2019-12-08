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

#include "common/image.h"
#include "engine/lifeengine.h"
#include "global.h"
#include "consolesystem.h"
#include "resourcesystem.h"

// ------------------------------------------------------------------------------------ //
// Зарегестрировать парсер картинок
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterParser( const char* Format, LoadImageFn_t LoadImage )
{
	LIFEENGINE_ASSERT( Format );

	g_consoleSystem->PrintInfo( "Parser for format [%s] registered", Format );
	parserImages[ Format ] = LoadImage;
}

// ------------------------------------------------------------------------------------ //
// Загрузить картинку
// ------------------------------------------------------------------------------------ //
le::Image* le::ResourceSystem::LoadImage( const char* Name, const char* Group, const char* Path, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Group );
	LIFEENGINE_ASSERT( Path );

	try
	{
		auto				group = images[ Group ];
		if ( group.find( Name ) != group.end() )	return &group[ Name ];
		if ( parserImages.empty() )					throw std::exception( "No image parsers" );

		g_consoleSystem->PrintInfo( "Loading image [%s] with name [%s] in group [%s]", Path, Name, Group );

		const char* format = strchr( Path, '.' );
		if ( !format )							throw std::exception( "In image format not found" );

		auto				parser = parserImages.find( format + 1 );
		if ( parser == parserImages.end() )			throw std::exception( "Parser for format image not found" );

		bool				isError = false;
		Image				image;
		
		parser->second( Path, image, isError, IsFlipVertical, IsSwitchRedAndBlueChannels );
		if ( isError )								throw std::exception( "Fail loading image" );

		group.insert( std::make_pair( Path, image ) );
		g_consoleSystem->PrintInfo( "Loaded image", Path, Name, Group );
		
		return &group[ Path ];
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( Exception.what() );
		return nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Удалить картинку
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::DeleteImage( const char* Name, const char* Group )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Group );

	auto				group = images[ Group ];
	auto				it = group.find( Name );
	
	if ( it == group.end() )	return;
	if ( it->second.data )		delete[] it->second.data;		// Не уверен, что так правильно удалять данные (в случае, если память была выделена в другой DLL)

	group.erase( it );
	g_consoleSystem->PrintInfo( "Deleted image [%s] in group [%s]", Name, Group );
}

// ------------------------------------------------------------------------------------ //
// Удалить все картинки
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::DeleteImages( const char* Group )
{
	auto				group = images[ Group ];
	if ( group.empty() )		return;

	for ( auto it = group.begin(), itEnd = group.end(); it != itEnd; ++it )
		if ( it->second.data )		delete[] it->second.data;

	group.clear();
	g_consoleSystem->PrintInfo( "Deleted all images in group [%s]", Group );
}

// ------------------------------------------------------------------------------------ //
// Удалить все картинки
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::DeleteImages()
{
	for ( auto itGroup = images.begin(), itEndGroup = images.end(); itGroup != itEndGroup; ++itGroup )
		for ( auto itImage = itGroup->second.begin(), itEndImage = itGroup->second.end(); itImage != itEndImage; ++itImage )
			if ( itImage->second.data )		delete[] itImage->second.data;

	images.clear();
	g_consoleSystem->PrintInfo( "Deleted all images" );
}

// ------------------------------------------------------------------------------------ //
// Удалить все ресурсы
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::DeleteAll()
{
	DeleteImages();
}

// ------------------------------------------------------------------------------------ //
// Получить картинку
// ------------------------------------------------------------------------------------ //
le::Image* le::ResourceSystem::GetImage( const char* Name, const char* Group )
{
	LIFEENGINE_ASSERT( Name );
	LIFEENGINE_ASSERT( Group );

	auto				group = images[ Group ];
	auto				it = group.find( Name );
	if ( it != group.end() )		return &it->second;

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ResourceSystem::ResourceSystem()
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ResourceSystem::~ResourceSystem()
{
	DeleteAll();
}
