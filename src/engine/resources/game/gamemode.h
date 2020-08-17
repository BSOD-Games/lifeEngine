#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <game/igamemode.h>

class GameMode : public le::IGameMode
{
public:
	// IGameMode
	virtual bool				Initialize( le::IEngine* Engine, le::UInt32_t CountArguments, const char** Arguments );
	virtual bool				LoadGame( const char* PathLevel );
	virtual void				UnloadGame();
};

#endif // !GAMEMODE_H