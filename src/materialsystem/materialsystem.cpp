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
#include <SDL2/SDL.h>

#include "engine/lifeengine.h"
#include "engine/iengine.h"
#include "engine/iconsolesystem.h"
#include "stdshaders/ishaderdll.h"

#include "global.h"
#include "materialsystem.h"

LIFEENGINE_MATERIALSYSTEM_API( le::MaterialSystem );

// ------------------------------------------------------------------------------------ //
// Инициализировать систему материалов
// ------------------------------------------------------------------------------------ //
bool le::MaterialSystem::Initialize( IEngine* Engine )
{
	try
	{
		if ( !Engine )					throw std::exception( "Material system requared engine api" );
		g_engine = Engine;

		g_consoleSystem = Engine->GetConsoleSystem();
		if ( !g_consoleSystem )			throw std::exception( "Material system requared console system" );

		g_studioRender = Engine->GetStudioRender();
		if ( !g_studioRender )			throw std::exception( "Material system requared studiorender" );
	}
	catch ( std::exception& Exception )
	{
		if ( g_consoleSystem ) g_consoleSystem->PrintError( Exception.what() );
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// Загрузить библиотеку шейдеров
// ------------------------------------------------------------------------------------ //
bool le::MaterialSystem::LoadShaderDLL( const char* FullPath )
{
	if ( !FullPath || shaderDLLs.find( FullPath ) != shaderDLLs.end() )	return true;

	g_consoleSystem->PrintInfo( "Loading shader dll [%s]", FullPath );

	try
	{
		ShaderDLLDescriptor		shaderDLLDescriptor;

		// Загружаем модуль
		shaderDLLDescriptor.handle = SDL_LoadObject( FullPath );
		if ( !shaderDLLDescriptor.handle )	throw std::exception( SDL_GetError() );

		// Берем из модуля API для работы с ним
		shaderDLLDescriptor.LE_CreateShaderDLL = ( LE_CreateShaderDLLFn_t ) SDL_LoadFunction( shaderDLLDescriptor.handle, "LE_CreateShaderDLL" );
		shaderDLLDescriptor.LE_DeleteShaderDLL = ( LE_DeleteShaderDLLFn_t ) SDL_LoadFunction( shaderDLLDescriptor.handle, "LE_DeleteShaderDLL" );
		shaderDLLDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( shaderDLLDescriptor.handle, "LE_SetCriticalError" );
		if ( !shaderDLLDescriptor.LE_CreateShaderDLL )	throw std::exception( "Function LE_CreateShaderDLL not found" );

		// Создаем рендер
		if ( shaderDLLDescriptor.LE_SetCriticalError )
			shaderDLLDescriptor.LE_SetCriticalError( g_criticalError );

		shaderDLLDescriptor.shaderDLL = ( IShaderDLL* ) shaderDLLDescriptor.LE_CreateShaderDLL();
		if ( !shaderDLLDescriptor.shaderDLL->Initialize( g_engine ) )				throw std::exception( "Fail initialize shader dll" );
	
		shaderDLLs.insert( std::make_pair( std::string( FullPath ), shaderDLLDescriptor ) );
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( Exception.what() );
		return false;
	}

	g_consoleSystem->PrintInfo( "Loaded shader dll [%s]", FullPath );
	return true;
}

// ------------------------------------------------------------------------------------ //
// Выгрузить библиотеку шейдеров
// ------------------------------------------------------------------------------------ //
void le::MaterialSystem::UnloadShaderDLL( const char* FullPath )
{
	if ( !FullPath ) return;

	auto			itShaderDLL = shaderDLLs.find( FullPath );
	if ( itShaderDLL == shaderDLLs.end() )	return;

	if ( itShaderDLL->second.LE_DeleteShaderDLL )
		itShaderDLL->second.LE_DeleteShaderDLL( itShaderDLL->second.shaderDLL );

	SDL_UnloadObject( itShaderDLL->second.handle );
	shaderDLLs.erase( itShaderDLL );

	g_consoleSystem->PrintInfo( "Unloaded shader dll [%s]", FullPath );
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::MaterialSystem::MaterialSystem()
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::MaterialSystem::~MaterialSystem()
{
	//TODO: реализовать удаление шейдерных библиотек
}