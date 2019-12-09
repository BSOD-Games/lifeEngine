//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IRESOURCESYSTEM_INTERNAL_H
#define IRESOURCESYSTEM_INTERNAL_H

#include "engine/iresourcesystem.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngine;

	//---------------------------------------------------------------------//

	class IResourceSystemInternal : public IResourceSystem
	{
	public:
		virtual bool			Initialize( IEngine* Engine ) = 0;

		virtual void			SetGameDir( const char* GameDir ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IRESOURCESYSTEM_INTERNAL_H

