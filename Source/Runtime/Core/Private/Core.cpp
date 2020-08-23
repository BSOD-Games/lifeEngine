// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Core.h"
#include "Misc/CoreGlobals.h"

/**
 * Constructor core
 */
le::Core::Core()
{
	GLogger = &logger;
	GWindow = &window;
}

/**
 * Destructor core
 */
le::Core::~Core()
{
}

/**
 * Initialize
 */
bool le::Core::Initialize( const std::string& InConfigPath, const std::string& InLogPath )
{
	logger.SetFile( InLogPath );
	return true;
}