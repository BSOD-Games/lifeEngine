//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IINPUTSYSTEM_H
#define IINPUTSYSTEM_H

#include "common/types.h"
#include "common/buttoncode.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IInputSystem
	{
	public:
        virtual ~IInputSystem() {}
		virtual bool				IsKeyDown( BUTTON_CODE Key ) = 0;
		virtual bool				IsKeyUp( BUTTON_CODE Key ) = 0;
		virtual	bool				IsMouseKeyDown( BUTTON_CODE Key ) = 0;
		virtual	bool				IsMouseKeyUp( BUTTON_CODE Key ) = 0;
		virtual bool				IsMouseWheel( BUTTON_CODE Wheel ) = 0;

                virtual const Vector2DInt_t&            GetMousePosition() const = 0;
                virtual const Vector2DInt_t&            GetMouseOffset() const = 0;
		virtual float				GetMouseSensitivity() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IINPUTSYSTEM_H

