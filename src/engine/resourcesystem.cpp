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

#include "global.h"
#include "materialsystem.h"
#include "scriptsystem.h"
#include "consolesystem.h"
#include "resourcesystem.h"

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
			parser->second->Read( std::string( paths[ index ] + "/" + Path ).c_str(), image, IsError, IsFlipVertical, IsSwitchRedAndBlueChannels );
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
			texture = parser->second->Read( std::string( paths[ index ] + "/" + Path ).c_str(), studioRenderFactory );
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
			material = parser->second->Read( std::string( paths[ index ] + "/" + Path ).c_str(), this, g_materialSystem, engineFactory );
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
			mesh = parser->second->Read( std::string( paths[ index ] + "/" + Path ).c_str(), this, studioRenderFactory );
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
			level = parser->second->Read( std::string( paths[ index ] + "/" + Path ).c_str(), GameFactory );
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
			font = parser->second->Read( std::string( paths[ index ] + "/" + Path ).c_str() );
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

		if ( !parserFontFreeType.InitializeFreeType() )
			throw std::runtime_error( "Failed initialize freetype library" );

		RegisterLoader( &parserFontFreeType );
		RegisterLoader( &parserGPUProgramShader );
		RegisterLoader( &parserImageFreeImage );
		RegisterLoader( &parserLevelBSP );
		RegisterLoader( &parserMaterialLMT );
		RegisterLoader( &parserMeshMDL );
		RegisterLoader( &parserPhysicsModelPHY );
		RegisterLoader( &parserScriptC );
		RegisterLoader( &parserTextureFreeImage );
		RegisterLoader( &parserSoundBufferOGG );
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
		for ( UInt32_t index = 0, count = paths.size(); index < count; ++index )
		{
			script = parser->second->Read( std::string( paths[ index ] + "/" + Path ).c_str(), CountFunctions, Functions, CountVars, Vars, scriptSystemFactory );
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
		for ( UInt32_t index = 0, count = paths.size(); index < count; ++index )
		{
			physicsModel = parser->second->Read( std::string( paths[ index ] + "/" + Path ).c_str(), g_physicsSystemFactory );
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
			gpuProgram = parser->second->Read( paths[ index ].c_str(), std::string( paths[ index ] + "/" + Path ).c_str(), CountDefines, Defines, studioRenderFactory );
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
		for ( UInt32_t index = 0, count = paths.size(); index < count; ++index )
		{			
			soundBuffer = parser->second->Read( std::string( paths[ index ] + "/" + Path ).c_str(), audioSystemFactory );
			if ( soundBuffer )
			{
				g_consoleSystem->PrintInfo( "Sound buffer founded in [%s]", paths[ index ].c_str() );
				break;
			}
		}

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
void le::ResourceSystem::RegisterLoader( IParserImage* ParserImage )
{
	if ( !ParserImage )		return;

	const char**		extensions = ParserImage->GetFileExtensions();
	UInt32_t			countExtensions = ParserImage->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser image not registered, supported extensions not founded" );
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderImages[ extensions[ index ] ] = ParserImage;

	g_consoleSystem->PrintInfo( "Parser image registered. Name: %s, Version: %s, Author: %s", ParserImage->GetName(), ParserImage->GetVersion(), ParserImage->GetAuthor() );

}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( IParserTexture* ParserTexture )
{
	if ( !ParserTexture )		return;

	const char**		extensions = ParserTexture->GetFileExtensions();
	UInt32_t			countExtensions = ParserTexture->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser texture not registered, supported extensions not founded" );
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderTextures[ extensions[ index ] ] = ParserTexture;

	g_consoleSystem->PrintInfo( "Parser texture registered. Name: %s, Version: %s, Author: %s", ParserTexture->GetName(), ParserTexture->GetVersion(), ParserTexture->GetAuthor() );
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( IParserMaterial* ParserMaterial )
{
	if ( !ParserMaterial )		return;

	const char**		extensions = ParserMaterial->GetFileExtensions();
	UInt32_t			countExtensions = ParserMaterial->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser material not registered, supported extensions not founded" );
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderMaterials[ extensions[ index ] ] = ParserMaterial;

	g_consoleSystem->PrintInfo( "Parser material registered. Name: %s, Version: %s, Author: %s", ParserMaterial->GetName(), ParserMaterial->GetVersion(), ParserMaterial->GetAuthor() );
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( IParserMesh* ParserMesh )
{
	if ( !ParserMesh )		return;

	const char**		extensions = ParserMesh->GetFileExtensions();
	UInt32_t			countExtensions = ParserMesh->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser mesh not registered, supported extensions not founded" );
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderMeshes[ extensions[ index ] ] = ParserMesh;

	g_consoleSystem->PrintInfo( "Parser mesh registered. Name: %s, Version: %s, Author: %s", ParserMesh->GetName(), ParserMesh->GetVersion(), ParserMesh->GetAuthor() );
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( IParserLevel* ParserLevel )
{
	if ( !ParserLevel )		return;

	const char**		extensions = ParserLevel->GetFileExtensions();
	UInt32_t			countExtensions = ParserLevel->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser level not registered, supported extensions not founded" );
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderLevels[ extensions[ index ] ] = ParserLevel;

	g_consoleSystem->PrintInfo( "Parser level registered. Name: %s, Version: %s, Author: %s", ParserLevel->GetName(), ParserLevel->GetVersion(), ParserLevel->GetAuthor() );
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( IParserFont* ParserFont )
{
	if ( !ParserFont )		return;

	const char**		extensions = ParserFont->GetFileExtensions();
	UInt32_t			countExtensions = ParserFont->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser font not registered, supported extensions not founded" );
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderFonts[ extensions[ index ] ] = ParserFont;

	g_consoleSystem->PrintInfo( "Parser font registered. Name: %s, Version: %s, Author: %s", ParserFont->GetName(), ParserFont->GetVersion(), ParserFont->GetAuthor() );
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( IParserScript* ParserScript )
{
	if ( !ParserScript )		return;

	const char**		extensions = ParserScript->GetFileExtensions();
	UInt32_t			countExtensions = ParserScript->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser script not registered, supported extensions not founded" );
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderScripts[ extensions[ index ] ] = ParserScript;

	g_consoleSystem->PrintInfo( "Parser script registered. Name: %s, Version: %s, Author: %s", ParserScript->GetName(), ParserScript->GetVersion(), ParserScript->GetAuthor() );
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( IParserPhysicsModel* ParserPhysicsModel )
{
	if ( !ParserPhysicsModel )		return;

	const char**		extensions = ParserPhysicsModel->GetFileExtensions();
	UInt32_t			countExtensions = ParserPhysicsModel->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser physics model not registered, supported extensions not founded" );
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderPhysicsModels[ extensions[ index ] ] = ParserPhysicsModel;

	g_consoleSystem->PrintInfo( "Parser physics model registered. Name: %s, Version: %s, Author: %s", ParserPhysicsModel->GetName(), ParserPhysicsModel->GetVersion(), ParserPhysicsModel->GetAuthor() );
}

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( IParserGPUProgram* ParserGPUProgram )
{
	if ( !ParserGPUProgram )		return;

	const char**		extensions = ParserGPUProgram->GetFileExtensions();
	UInt32_t			countExtensions = ParserGPUProgram->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser GPU program not registered, supported extensions not founded" );
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderGPUProgram[ extensions[ index ] ] = ParserGPUProgram;

	g_consoleSystem->PrintInfo( "Parser GPU program registered. Name: %s, Version: %s, Author: %s", ParserGPUProgram->GetName(), ParserGPUProgram->GetVersion(), ParserGPUProgram->GetAuthor() );
}

#include "engine/iparsersoundbuffer.h"

// ------------------------------------------------------------------------------------ //
// Register loader
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader( IParserSoundBuffer* ParserSoundBuffer )
{
	if ( !ParserSoundBuffer )		return;

	const char**		extensions = ParserSoundBuffer->GetFileExtensions();
	UInt32_t			countExtensions = ParserSoundBuffer->GetCountFileExtensions();

	if ( countExtensions <= 0 || !extensions )
	{
		g_consoleSystem->PrintError( "Parser sound buffer not registered, supported extensions not founded" );
		return;
	}

	for ( UInt32_t index = 0; index < countExtensions; ++index )
		loaderSoundBuffers[ extensions[ index ] ] = ParserSoundBuffer;

	g_consoleSystem->PrintInfo( "Parser sound buffer registered. Name: %s, Version: %s, Author: %s", ParserSoundBuffer->GetName(), ParserSoundBuffer->GetVersion(), ParserSoundBuffer->GetAuthor() );
}