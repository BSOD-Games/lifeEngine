//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIALSYSTEM_INTERNAL_H
#define IMATERIALSYSTEM_INTERNAL_H

#include "materialsystem/imaterialsystem.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngine;

	//---------------------------------------------------------------------//

	class IMaterialSystemInternal : public IMaterialSystem
	{
	public:
		virtual bool			Initialize( IEngine* Engine ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIALSYSTEM_INTERNAL_H

