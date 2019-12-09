//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_SYSTEM_FACTORY_H
#define MATERIAL_SYSTEM_FACTORY_H

#include "engine/ifactory.h"

namespace le
{
	//---------------------------------------------------------------------//

	class MaterialSystemFactory : public IFactory
	{
	public:
		// IFactory
		virtual void*			Create( const char* NameInterface );
		virtual void			Delete( void* Object );
	};

	//---------------------------------------------------------------------//
}

#endif // !MATERIAL_SYSTEM_FACTORY_H

