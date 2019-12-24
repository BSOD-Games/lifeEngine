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
#include "engine/consolesystem.h"
#include "engine/concmd.h"
#include "global.h"
#include "engine.h"
#include "inputsystem.h"
#include "buttontranslation.h"

le::ConCmd*					cmd_bind = nullptr;;

namespace le
{
	// ------------------------------------------------------------------------------------ //
	// Консольная команда биндинга клавиши
	// ------------------------------------------------------------------------------------ //
	void CMD_Bind( le::UInt32_t CountArguments, const char** Arguments )
	{
		if ( CountArguments < 2 || !Arguments )
		{
			le::g_consoleSystem->PrintInfo( "using command \"bind\": bind <key> <command> (down/up, deffault = down)" );
			le::g_consoleSystem->PrintInfo( "example: bind q exit" );
		}

		std::string					strButtonTrigger = Arguments[ 0 ];
		for ( UInt32_t index = 0, count = strButtonTrigger.size(); index < count; ++index )
			strButtonTrigger[ index ] = tolower( strButtonTrigger[ index ] );
		
		Event::EVENT_TYPE			eventTrigger = Event::ET_KEY_PRESSED;
		if ( CountArguments >= 3 )
		{
			std::string				strEventTrigger = Arguments[ 2 ];
			for ( UInt32_t index = 0, count = strEventTrigger.size(); index < count; ++index )
				strEventTrigger[ index ] = tolower( strEventTrigger[ index ] );

			if ( CountArguments >= 2 && strcmp( "up", strEventTrigger.c_str() ) == 0 )
				eventTrigger = Event::ET_KEY_RELEASED;
		}

		// TODO: Исправить баг с тригерным событием (работает только для клавиш клавиатуры)
		le::g_inputSystem->binds.push_back( le::BindDescriptor( ButtonCode_StringToButtonCode( strButtonTrigger.c_str() ), eventTrigger, Arguments[ 1 ] ) );
	}
}

// ------------------------------------------------------------------------------------ //
// Инициализировать систему ввода
// ------------------------------------------------------------------------------------ //
bool le::InputSystem::Initialize( IEngine* Engine )
{
	if ( !g_consoleSystem )			return false;

	cmd_bind = new ConCmd();
	cmd_bind->Initialize( "bind", CMD_Bind );
	g_consoleSystem->RegisterCommand( cmd_bind );

	return true;
}

// ------------------------------------------------------------------------------------ //
// Применить событие
// ------------------------------------------------------------------------------------ //
void le::InputSystem::ApplyEvent( const Event& Event )
{
	switch ( Event.type )
	{
	case Event::ET_KEY_PRESSED:
	case Event::ET_KEY_RELEASED:
		buttonEvents[ Event.key.code ] = Event.type;
		break;

	case Event::ET_MOUSE_PRESSED:
	case Event::ET_MOUSE_RELEASED:
		buttonEvents[ Event.mouseButton.code ] = Event.type;
		break;

	case Event::ET_MOUSE_MOVE:
		mouseOffset.x = Event.mouseMove.xDirection;
		mouseOffset.y = Event.mouseMove.yDirection;
		mousePosition.x = Event.mouseMove.x;
		mousePosition.y = Event.mouseMove.y;
		break;

	case Event::ET_MOUSE_WHEEL:
		if ( Event.mouseWheel.y == 0 ) break;
		buttonEvents[ Event.mouseWheel.y > 0 ? BC_MOUSE_WHEEL_UP : BC_MOUSE_WHEEL_DOWN ] = Event::ET_MOUSE_WHEEL;
		break;

	case Event::ET_TEXT_INPUT:
		// TODO: Сделать обработку ввода текста
		break;
	}
}

// ------------------------------------------------------------------------------------ //
// Обновить систему ввода
// ------------------------------------------------------------------------------------ //
void le::InputSystem::Update()
{
	for ( UInt32_t index = 0, count = binds.size(); index < count; ++index )
	{
		BindDescriptor&			bindDescriptor = binds[ index ];	
		if ( buttonEvents[ bindDescriptor.buttonTrigger ] != bindDescriptor.eventTrigger ) 
			continue;

		g_consoleSystem->Exec( bindDescriptor.command.c_str() );
	}
}

// ------------------------------------------------------------------------------------ //
// Сбросить состояния всех клавиш
// ------------------------------------------------------------------------------------ //
void le::InputSystem::Clear()
{
	for ( uint32_t index = 0; index < BC_COUNT; ++index )
		if ( buttonEvents[ index ] == Event::ET_KEY_RELEASED || 
			 buttonEvents[ index ] == Event::ET_MOUSE_RELEASED ||
			 buttonEvents[ index ] == Event::ET_MOUSE_WHEEL )
			buttonEvents[ index ] = Event::ET_NONE;

	mouseOffset = Vector2D_t( 0.f );
}

// ------------------------------------------------------------------------------------ //
// Нажата ли кнопка на клавиатуре
// ------------------------------------------------------------------------------------ //
bool le::InputSystem::IsKeyDown( BUTTON_CODE Key )
{
	if ( Key < BC_KEY_FIRST || Key > BC_KEY_LAST )			return false;
	return buttonEvents[ Key ] == Event::ET_KEY_PRESSED;
}

// ------------------------------------------------------------------------------------ //
// Отпущена ли кнопка на клавиатуре
// ------------------------------------------------------------------------------------ //
bool le::InputSystem::IsKeyUp( BUTTON_CODE Key )
{
	if ( Key < BC_KEY_FIRST || Key > BC_KEY_LAST )			return false;
	return buttonEvents[ Key ] == Event::ET_KEY_RELEASED;
}

// ------------------------------------------------------------------------------------ //
// Нажата ли кнопка мыши
// ------------------------------------------------------------------------------------ //
bool le::InputSystem::IsMouseKeyDown( BUTTON_CODE Key )
{
	if ( Key < BC_MOUSE_FIRST || Key > BC_MOUSE_LAST )			return false;
	return buttonEvents[ Key ] == Event::ET_MOUSE_PRESSED;
}

// ------------------------------------------------------------------------------------ //
// Отпущена ли кнопка мыши
// ------------------------------------------------------------------------------------ //
bool le::InputSystem::IsMouseKeyUp( BUTTON_CODE Key )
{
	if ( Key < BC_MOUSE_FIRST || Key > BC_MOUSE_LAST )			return false;
	return buttonEvents[ Key ] == Event::ET_MOUSE_RELEASED;
}

// ------------------------------------------------------------------------------------ //
// Крутится ли колесико мышки
// ------------------------------------------------------------------------------------ //
bool le::InputSystem::IsMouseWheel( BUTTON_CODE Wheel )
{
	if ( Wheel != BC_MOUSE_WHEEL_UP && Wheel != BC_MOUSE_WHEEL_DOWN )		return false;
	return buttonEvents[ Wheel ] == Event::ET_MOUSE_WHEEL;
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
	mousePosition( 0.f ),
	mouseOffset( 0.f )
{
	memset( &buttonEvents, Event::ET_NONE, BC_COUNT * sizeof( Event::EVENT_TYPE ) );
}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::InputSystem::~InputSystem()
{
	if ( cmd_bind )
	{
		g_consoleSystem->UnregisterCommand( cmd_bind->GetName() );
		delete cmd_bind;
	}
}
