//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef CONSOLE_SYSTEM_FACTORY_H
#define CONSOLE_SYSTEM_FACTORY_H

#include "engine/ifactory.h"

namespace le
{
	//---------------------------------------------------------------------//

	class ConsoleSystemFactory : public IFactory
	{
	public:
		// IFactory
		virtual void*			Create( const char* NameInterface );
		virtual void			Delete( const char* NameInterface, void* Object );
	};

	//---------------------------------------------------------------------//
}

#endif // !CONSOLE_SYSTEM_FACTORY_H

