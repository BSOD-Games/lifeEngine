//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

#include "types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IConVar;

	//---------------------------------------------------------------------//

	struct Configurations
	{
		IConVar*	windowWidth;
		IConVar*	windowHeight;
		IConVar*	windowFullscreen;
		IConVar*	mouseSensitivity;
		IConVar*	rvsinc;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !CONFIGURATIONS_H

