//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ENGINE_FACTORY_H
#define ENGINE_FACTORY_H

#include "engine/ifactory.h"

namespace le
{
	//---------------------------------------------------------------------//

	class EngineFactory : public IFactory
	{
	public:
		// IFactory
		virtual void*			Create( const char* NameInterface );
		virtual void			Delete( void* Object );
	};

	//---------------------------------------------------------------------//
}

#endif // !ENGINE_FACTORY_H

