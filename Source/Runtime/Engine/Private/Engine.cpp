// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Misc/EngineGlobals.h"
#include "Logging/Logger.h"
#include "Rendering/RenderSystem.h"
#include "Resources/ResourceSystem.h"
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
	GLogger->CloseFile();
}

/**
 * Initialize engine
 */
bool le::Engine::Initialize( const std::string& InConfigPath, const std::string& InLogPath )
{
	GLogger->SetFile( InLogPath );
	GRenderSystem->Initialize();	
	GResourceSystem->Initialize();

	return false;
}