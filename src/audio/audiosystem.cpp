//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <stdexcept>

#include "engine/iengine.h"

#include "global.h"
#include "audiosystem.h"

LIFEENGINE_AUDIOSYSTEM_API( le::AudioSystem );

// ------------------------------------------------------------------------------------ //
// Initialize subsystem
// ------------------------------------------------------------------------------------ //
bool le::AudioSystem::Initialize( IEngine* Engine )
{
	g_consoleSystem = Engine->GetConsoleSystem();
	
	try
	{
		if ( !g_consoleSystem )				throw std::runtime_error( "Console system not founded in engine" );
		if ( !audioDevice.Create() )		throw std::runtime_error( "Failed in creating audio device" );
	}
	catch ( const std::exception& Exception )
	{
		if ( g_consoleSystem )	g_consoleSystem->PrintError( Exception.what() );
		else					g_criticalError( Exception.what() );
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// Get factory
// ------------------------------------------------------------------------------------ //
le::IFactory* le::AudioSystem::GetFactory() const
{
	return ( IFactory* ) &factory;
}