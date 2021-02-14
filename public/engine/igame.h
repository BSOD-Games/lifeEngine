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
        virtual ~IGame() {}
		virtual bool				Initialize( IEngine* Engine, UInt32_t CountArguments, const char** Arguments ) = 0;
		virtual void				Update() = 0;
		virtual void                Render() = 0;
		virtual void				OnEvent( const Event& Event ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IGAME_H

