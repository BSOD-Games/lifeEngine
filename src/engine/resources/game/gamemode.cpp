#include <engine/lifeengine.h>

#include "gamemode.h"

LIFEENGINE_GAMEMODE_API( GameMode );

// ------------------------------------------------------------------------------------ //
// Initialize game mode
// ------------------------------------------------------------------------------------ //
bool GameMode::Initialize( le::IEngine* Engine, le::UInt32_t CountArguments, const char** Arguments )
{
	return true;
}

// ------------------------------------------------------------------------------------ //
// Load game
// ------------------------------------------------------------------------------------ //
bool GameMode::LoadGame( const char* PathLevel )
{
	return true;
}

// ------------------------------------------------------------------------------------ //
// Unload game
// ------------------------------------------------------------------------------------ //
void GameMode::UnloadGame()
{}