//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IGAME_H
#define IGAME_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct Event;
	class IEngine;

	//---------------------------------------------------------------------//

	class IGame
	{
	public:
		virtual bool				Initialize( IEngine* Engine ) = 0;
		virtual void				Update( UInt32_t DeltaTime ) = 0;
		virtual void				OnEvent( const Event& Event ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IGAME_H

