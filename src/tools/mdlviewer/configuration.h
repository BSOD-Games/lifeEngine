//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <vector>

#include "gamedescriptor.h"

//---------------------------------------------------------------------//

class Configuration
{
public:
	// Configuration
	Configuration();
	~Configuration();

	bool									Load();
	void									Save();

	void									SetGames( const std::vector<GameDescriptor>& Games );

	const std::vector<GameDescriptor>&		GetGames() const;

private:
	std::vector<GameDescriptor>			games;
};

//---------------------------------------------------------------------//

#endif // CONFIGURATION_H
