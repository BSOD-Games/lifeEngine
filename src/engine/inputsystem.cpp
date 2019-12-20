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

#define KEYBOARD_BIND( KeyTrigger ) \
	le::g_inputSystem->keyboardBind.push_back( le::BindDescriptor< le::KEYBOARD_KEY >( KeyTrigger, eventTrigger, Arguments[ 1 ] ) )

#define MOUSE_BIND( KeyTrigger ) \
	le::g_inputSystem->mousedBind.push_back( le::BindDescriptor< le::MOUSE_KEY >( KeyTrigger, eventTrigger, Arguments[ 1 ] ) );

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

		std::string					strKeyTrigger = Arguments[ 0 ];
		for ( UInt32_t index = 0, count = strKeyTrigger.size(); index < count; ++index )
			strKeyTrigger[ index ] = tolower( strKeyTrigger[ index ] );
		
		Event::EVENT_TYPE			eventTrigger = Event::ET_KEY_PRESSED;
		if ( CountArguments >= 3 )
		{
			std::string				strEventTrigger = Arguments[ 2 ];
			for ( UInt32_t index = 0, count = strEventTrigger.size(); index < count; ++index )
				strEventTrigger[ index ] = tolower( strEventTrigger[ index ] );

			if ( CountArguments >= 2 && strcmp( "up", strEventTrigger.c_str() ) == 0 )
				eventTrigger = Event::ET_KEY_RELEASED;
		}

		// TODO: УБРАТЬ ЭТОТ ЕБАННЫЙ ГОВНО КОДА И СДЕЛАЙ, СУКА, НОРМАЛЬНО!! *ГОВОРИТ ЕГОР СЕБЕ*

		if ( strcmp( "q", strKeyTrigger.c_str() ) == 0 )				KEYBOARD_BIND( KK_Q );
		else if ( strcmp( "w", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_W );
		else if ( strcmp( "e", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_E );
		else if ( strcmp( "r", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_R );
		else if ( strcmp( "t", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_T );
		else if ( strcmp( "y", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_Y );
		else if ( strcmp( "u", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_U );
		else if ( strcmp( "i", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_I );
		else if ( strcmp( "o", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_O );
		else if ( strcmp( "p", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_P );
		else if ( strcmp( "[", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_LEFTBRACKET );
		else if ( strcmp( "]", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_RIGHTBRACKET );
		else if ( strcmp( "a", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_A );
		else if ( strcmp( "s", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_S );
		else if ( strcmp( "d", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_D );
		else if ( strcmp( "f", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_F );
		else if ( strcmp( "g", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_G );
		else if ( strcmp( "h", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_H );
		else if ( strcmp( "j", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_J );
		else if ( strcmp( "k", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_K );
		else if ( strcmp( "l", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_L );
		else if ( strcmp( ";", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_SEMICOLON );
		else if ( strcmp( "'", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_APOSTROPHE );
		else if ( strcmp( "z", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_Z );
		else if ( strcmp( "x", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_X );
		else if ( strcmp( "c", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_C );
		else if ( strcmp( "v", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_V );
		else if ( strcmp( "b", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_B );
		else if ( strcmp( "n", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_N );
		else if ( strcmp( "m", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_M );
		else if ( strcmp( ",", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_COMMA );
		else if ( strcmp( ".", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_PERIOD );
		else if ( strcmp( "/", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_SLASH );
		else if ( strcmp( "`", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_GRAVE );
		else if ( strcmp( "1", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_1 );
		else if ( strcmp( "2", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_2 );
		else if ( strcmp( "3", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_3 );
		else if ( strcmp( "4", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_4 );
		else if ( strcmp( "5", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_5 );
		else if ( strcmp( "6", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_6 );
		else if ( strcmp( "7", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_7 );
		else if ( strcmp( "8", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_8 );
		else if ( strcmp( "9", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_9 );
		else if ( strcmp( "0", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_0 );
		else if ( strcmp( "esc", strKeyTrigger.c_str() ) == 0 )			KEYBOARD_BIND( KK_ESCAPE );
		else if ( strcmp( "space", strKeyTrigger.c_str() ) == 0 )		KEYBOARD_BIND( KK_SPACE );
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
// Обновить систему ввода
// ------------------------------------------------------------------------------------ //
void le::InputSystem::Update()
{
	for ( UInt32_t index = 0, count = keyboardBind.size(); index < count; ++index )
	{
		BindDescriptor< KEYBOARD_KEY >&			bindDescriptor = keyboardBind[ index ];	
		if ( keyboardKeyEvents[ bindDescriptor.keyTrigger ] != bindDescriptor.eventTrigger ) 
			continue;

		g_consoleSystem->Exec( bindDescriptor.command.c_str() );
	}

	for ( UInt32_t index = 0, count = mousedBind.size(); index < count; ++index )
	{
		BindDescriptor< MOUSE_KEY >&			bindDescriptor = mousedBind[ index ];
		if ( mouseButtonEvents[ bindDescriptor.keyTrigger ] != bindDescriptor.eventTrigger )
			continue;

		g_consoleSystem->Exec( bindDescriptor.command.c_str() );
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
{
	if ( cmd_bind )
	{
		g_consoleSystem->UnregisterCommand( cmd_bind->GetName() );
		delete cmd_bind;
	}
}
