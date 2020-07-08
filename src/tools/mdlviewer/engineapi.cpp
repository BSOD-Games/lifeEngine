//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <stdexcept>
#include <fstream>
#include <qmessagebox.h>

#include "errors.h"
#include "paths.h"
#include "engineapi.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
EngineAPI::EngineAPI() :
	engine( nullptr ),
    resourceSystem( nullptr ),
    consoleSystem( nullptr ),
	studioRender( nullptr ),
	LE_CreateEngine( nullptr ),
	LE_SetCriticalError( nullptr ),
	LE_DeleteEngine( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
EngineAPI::~EngineAPI()
{
	Unload();
}

// ------------------------------------------------------------------------------------ //
// Load engine
// ------------------------------------------------------------------------------------ //
bool EngineAPI::Load()
{
	if ( engine ) return true;

	try
	{
		engineDLL.setFileName( "./" LIFEENGINE_ENGINE_DLL );
		if ( !engineDLL.load() )			throw std::runtime_error( engineDLL.errorString().toLocal8Bit().data() );

		LE_CreateEngine = ( le::LE_CreateEngineFn_t ) engineDLL.resolve( "LE_CreateEngine" );
		LE_DeleteEngine = ( le::LE_DeleteEngineFn_t ) engineDLL.resolve( "LE_DeleteEngine" );
		LE_SetCriticalError = ( le::LE_SetCriticalErrorFn_t ) engineDLL.resolve( "LE_SetCriticalError" );

		if ( !LE_CreateEngine )				throw std::runtime_error( "Function LE_CreateEngine not found in [./" LIFEENGINE_ENGINE_DLL "]" );
		if ( LE_SetCriticalError )			LE_SetCriticalError( Error_Critical );

		engine = ( le::IEngineInternal* ) LE_CreateEngine();
		if ( !engine->Initialize( "./", "lmdviewer.log" ) )
			throw std::runtime_error( "Failed initialize engine" );

        resourceSystem = ( le::IResourceSystemInternal* ) engine->GetResourceSystem();
        consoleSystem = engine->GetConsoleSystem();
		studioRender = ( le::IStudioRenderInternal* ) engine->GetStudioRender();
		materialSystem = ( le::IMaterialSystemInternal* ) engine->GetMaterialSystem();
	}
	catch ( const std::exception& Exception)
	{
		errorString = Exception.what();
		return 1;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// Unload engine
// ------------------------------------------------------------------------------------ //
void EngineAPI::Unload()
{
    if ( engine )
	{
		if ( LE_DeleteEngine )		LE_DeleteEngine( engine );
		engineDLL.unload();
	}

	engine = nullptr;
    resourceSystem = nullptr;
    consoleSystem = nullptr;
	studioRender = nullptr;
	LE_CreateEngine = nullptr;
	LE_SetCriticalError = nullptr;
    LE_DeleteEngine = nullptr;
}
