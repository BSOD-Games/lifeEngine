//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <exception>
#include <string>
#include <SDL2/SDL.h>

#include "common/gameinfo.h"
#include "engine/lifeengine.h"
#include "engine/iengine.h"
#include "engine/iconsolesystem.h"
#include "stdshaders/ishaderdll.h"
#include "stdshaders/ishader.h"

#include "global.h"
#include "shadermanager.h"

// ------------------------------------------------------------------------------------ //
// Загрузить библиотеку шейдеров
// ------------------------------------------------------------------------------------ //
bool le::ShaderManager::LoadShaderDLL( const char* FullPath )
{
    LIFEENGINE_ASSERT( FullPath );

    // Проверяем, не загружена ли уже библиотеку шейдеров
    for ( UInt32_t index, count = shaderLibs.size(); index < count; ++index )
        if ( shaderLibs[ index ].fileName == FullPath )
            return true;

    g_consoleSystem->PrintInfo( "Loading shader dll [%s]", FullPath );

    try
    {
        ShaderDLLDescriptor			shaderDLLDescriptor;

        // Загружаем модуль
        const char*                 gameDir = g_engine->GetGameInfo().gameDir;

        if ( gameDir && gameDir != "" )         shaderDLLDescriptor.handle = SDL_LoadObject( ( std::string( gameDir ) + "/" + FullPath ).c_str() );
        if ( !shaderDLLDescriptor.handle )      shaderDLLDescriptor.handle = SDL_LoadObject( FullPath );
        if ( !shaderDLLDescriptor.handle )      throw std::runtime_error( SDL_GetError() );

        // Берем из модуля API для работы с ним
        shaderDLLDescriptor.LE_CreateShaderDLL = ( LE_CreateShaderDLLFn_t ) SDL_LoadFunction( shaderDLLDescriptor.handle, "LE_CreateShaderDLL" );
        shaderDLLDescriptor.LE_DeleteShaderDLL = ( LE_DeleteShaderDLLFn_t ) SDL_LoadFunction( shaderDLLDescriptor.handle, "LE_DeleteShaderDLL" );
        shaderDLLDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( shaderDLLDescriptor.handle, "LE_SetCriticalError" );
        if ( !shaderDLLDescriptor.LE_CreateShaderDLL )	throw std::runtime_error( "Function LE_CreateShaderDLL not found" );

        // Создаем рендер
        if ( shaderDLLDescriptor.LE_SetCriticalError )
            shaderDLLDescriptor.LE_SetCriticalError( g_criticalError );

        shaderDLLDescriptor.shaderDLL = ( IShaderDLL* ) shaderDLLDescriptor.LE_CreateShaderDLL();
        if ( !shaderDLLDescriptor.shaderDLL->Initialize( g_engine ) )				throw std::runtime_error( "Fail initialize shader dll" );

        shaderDLLDescriptor.fileName = FullPath;

        // Загружаем все шейдера из библиотеки
        for ( UInt32_t index = 0, count = shaderDLLDescriptor.shaderDLL->GetShaderCount(); index < count; ++index )
        {
            IShader* shader = shaderDLLDescriptor.shaderDLL->GetShader( index );
            if ( !shader ) continue;

            shaderDLLDescriptor.shaders.insert( std::make_pair( shader->GetName(), shader ) );
            g_consoleSystem->PrintInfo( "Loaded shader [%s] from [%s]", shader->GetName(), FullPath );
        }

        // Регестрируем библиотеку шейдеров в системе
        shaderLibs.push_back( shaderDLLDescriptor );
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
void le::ShaderManager::UnloadShaderDLL( const char* FullPath )
{
    LIFEENGINE_ASSERT( FullPath );

    for ( UInt32_t index = 0, count = shaderLibs.size(); index < count; ++index )
        if ( shaderLibs[ index ].fileName == FullPath )
        {
            UnloadShaderDLL( shaderLibs[ index ] );
            shaderLibs.erase( shaderLibs.begin() + index );
            break;
        }
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ShaderManager::ShaderManager()
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ShaderManager::~ShaderManager()
{
    for ( auto it = shaderLibs.begin(); it != shaderLibs.end(); ++it )
        UnloadShaderDLL( *it );
}

// ------------------------------------------------------------------------------------ //
// Найти шейдер
// ------------------------------------------------------------------------------------ //
le::IShader* le::ShaderManager::FindShader( const char* ShaderName ) const
{
    LIFEENGINE_ASSERT( ShaderName );

    for ( auto it = shaderLibs.rbegin(), itEnd = shaderLibs.rend(); it != itEnd; ++it )
    {
        auto itShader = it->shaders.find( ShaderName );
        if ( itShader != it->shaders.end() )
            return itShader->second;
    }

    // -------
    // Я люблю Source Engine <3
    // -------

    return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Выгрузить библиотеку шейдеров
// ------------------------------------------------------------------------------------ //
void le::ShaderManager::UnloadShaderDLL( const ShaderDLLDescriptor& ShaderDLLDescriptor )
{
    if ( ShaderDLLDescriptor.LE_DeleteShaderDLL )
        ShaderDLLDescriptor.LE_DeleteShaderDLL( ShaderDLLDescriptor.shaderDLL );

    SDL_UnloadObject( ShaderDLLDescriptor.handle );
    g_consoleSystem->PrintInfo( "Unloaded shader dll [%s]", ShaderDLLDescriptor.fileName.c_str() );
}
