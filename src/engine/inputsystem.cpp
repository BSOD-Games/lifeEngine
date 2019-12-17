//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "common/configurations.h"
#include "global.h"
#include "engine.h"
#include "inputsystem.h"

// ------------------------------------------------------------------------------------ //
// Применить событие
// ------------------------------------------------------------------------------------ //
void le::InputSystem::ApplyEvent( const Event& Event )
{
	switch ( Event.type )
	{
	case Event::ET_KEY_PRESSED:
	case Event::ET_KEY_RELEASED:
		keyboardKeyEvents[ Event.key.code ] = Event.type;
		break;

	case Event::ET_MOUSE_PRESSED:
	case Event::ET_MOUSE_RELEASED:
		mouseButtonEvents[ Event.mouseButton.code ] = Event.type;
		break;

	case Event::ET_MOUSE_MOVE:
		mouseOffset.x = Event.mouseMove.xDirection;
		mouseOffset.y = Event.mouseMove.yDirection;
		mousePosition.x = Event.mouseMove.x;
		mousePosition.y = Event.mouseMove.y;
		break;

	case Event::ET_MOUSE_WHEEL:
		mouseWheel = Event.mouseWheel.y > 0 ? MW_UP : Event.mouseWheel.y < 0 ? MW_DOWN : MW_NONE;
		break;

	case Event::ET_TEXT_INPUT:
		// TODO: Сделать обработку ввода текста
		break;
	}
}

// ------------------------------------------------------------------------------------ //
// Сбросить состояния всех клавиш
// ------------------------------------------------------------------------------------ //
void le::InputSystem::Clear()
{
	for ( uint32_t index = 0; index < KK_KEY_COUNT; ++index )
		if ( keyboardKeyEvents[ index ] == Event::ET_KEY_RELEASED )
			keyboardKeyEvents[ index ] = Event::ET_NONE;

	for ( uint32_t index = 0; index < MK_KEY_COUNT; ++index )
		if ( mouseButtonEvents[ index ] == Event::ET_KEY_RELEASED )
			mouseButtonEvents[ index ] = Event::ET_NONE;

	mouseWheel = MW_NONE;
	mouseOffset = Vector2D_t( 0.f );
}

// ------------------------------------------------------------------------------------ //
// Нажата ли кнопка на клавиатуре
// ------------------------------------------------------------------------------------ //
bool le::InputSystem::IsKeyDown( KEYBOARD_KEY Key )
{
	return keyboardKeyEvents[ Key ] == Event::ET_KEY_PRESSED;
}

// ------------------------------------------------------------------------------------ //
// Отпущена ли кнопка на клавиатуре
// ------------------------------------------------------------------------------------ //
bool le::InputSystem::IsKeyUp( KEYBOARD_KEY Key )
{
	return keyboardKeyEvents[ Key ] == Event::ET_KEY_RELEASED;
}

// ------------------------------------------------------------------------------------ //
// Нажата ли кнопка мыши
// ------------------------------------------------------------------------------------ //
bool le::InputSystem::IsMouseKeyDown( MOUSE_KEY Key )
{
	return mouseButtonEvents[ Key ] == Event::ET_MOUSE_PRESSED;
}

// ------------------------------------------------------------------------------------ //
// Отпущена ли кнопка мыши
// ------------------------------------------------------------------------------------ //
bool le::InputSystem::IsMouseKeyUp( MOUSE_KEY Key )
{
	return mouseButtonEvents[ Key ] == Event::ET_MOUSE_RELEASED;
}

// ------------------------------------------------------------------------------------ //
// Крутится ли колесико мышки
// ------------------------------------------------------------------------------------ //
bool le::InputSystem::IsMouseWheel( MOUSE_WHEEL Wheel )
{
	return mouseWheel == Wheel;
}

// ------------------------------------------------------------------------------------ //
// Получить позицию мышки
// ------------------------------------------------------------------------------------ //
const le::Vector2D_t& le::InputSystem::GetMousePosition() const
{
	return mousePosition;
}

// ------------------------------------------------------------------------------------ //
// Получить смещение мышки
// ------------------------------------------------------------------------------------ //
const le::Vector2D_t& le::InputSystem::GetMouseOffset() const
{
	return mouseOffset;
}

// ------------------------------------------------------------------------------------ //
// Получить чувствительность мышки
// ------------------------------------------------------------------------------------ //
float le::InputSystem::GetMouseSensitivity() const
{
	return g_engine->GetConfigurations().sensitivityMouse;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::InputSystem::InputSystem() :
	mouseWheel( MW_NONE ),
	mousePosition( 0.f ),
	mouseOffset( 0.f )
{
	memset( &keyboardKeyEvents, Event::ET_NONE, KK_KEY_COUNT * sizeof( Event::EVENT_TYPE ) );
	memset( &mouseButtonEvents, Event::ET_NONE, MK_KEY_COUNT * sizeof( Event::EVENT_TYPE ) );
}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::InputSystem::~InputSystem()
{}
