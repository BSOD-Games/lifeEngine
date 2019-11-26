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

	struct Configurations
	{
		bool		isFullscreen;
		bool		isVerticalSinc;

		float		fov;
		float		sensitivityMouse;

		UInt32_t	windowWidth;
		UInt32_t	windowHeight;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !CONFIGURATIONS_H

