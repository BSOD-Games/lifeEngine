// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Logging/Logger.h"
#include "Window.h"
#include "Engine.h"

/**
 * Constructor
 */
le::Engine::Engine()
{}

/**
 * Destructor
 */
le::Engine::~Engine()
{
	Logger::GetInstance()->CloseFile();
}

/**
 * Initialize engine
 */
bool le::Engine::Initialize( const std::string& InConfigPath, const std::string& InLogPath )
{
	Logger::GetInstance()->SetFile( InLogPath );
	Window::GetInstance()->Open( "lifeEngine", 800, 600 );
	
	return false;
}