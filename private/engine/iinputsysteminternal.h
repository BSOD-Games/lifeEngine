//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IINPUTSYSTEM_INTERNAL_H
#define IINPUTSYSTEM_INTERNAL_H

#include "engine/iinputsystem.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngine;
	struct Event;

	//---------------------------------------------------------------------//

	class IInputSystemInternal : public IInputSystem
	{
	public:
		virtual bool			Initialize( IEngine* Engine ) = 0;
                virtual void			ApplyEvent( const Event& Event ) = 0;
		virtual void			Update() = 0;
		virtual void			Clear() = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IINPUTSYSTEM_INTERNAL_H
