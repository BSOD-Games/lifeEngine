//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "engine/consolesystem.h"
#include "engine/window.h"
#include "engine/global.h"
#include "engine/buttontranslation.h"

// ------------------------------------------------------------------------------------ //
// Сбросить курсор
// ------------------------------------------------------------------------------------ //
void le::Window::ResetCursor()
{
	if ( !cursor ) return;

	SDL_FreeCursor( cursor );
	cursor = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_ARROW );
	SDL_SetCursor( cursor );
}

// ------------------------------------------------------------------------------------ //
// Задать название окна
// ------------------------------------------------------------------------------------ //
void le::Window::SetTitle( const char* Title )
{
	if ( !window ) return;
	SDL_SetWindowTitle( window, Title );
}

// ------------------------------------------------------------------------------------ //
// Задать иконку
// ------------------------------------------------------------------------------------ //
void le::Window::SetIcon( const Image& Image )
{
	if ( !window || !Image.data ) return;

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom( Image.data, Image.width, Image.height, Image.depth, Image.pitch,
													 Image.rMask, Image.gMask, Image.bMask, Image.aMask );
	SDL_SetWindowIcon( window, surface );
	SDL_FreeSurface( surface );
}

// ------------------------------------------------------------------------------------ //
// Задать курсор
// ------------------------------------------------------------------------------------ //
void le::Window::SetCursor( const Image& Image )
{
	if ( !Image.data ) return;
	if ( cursor ) SDL_FreeCursor( cursor );

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom( Image.data, Image.width, Image.height, Image.depth, Image.pitch,
													 Image.rMask, Image.gMask, Image.bMask, Image.aMask );
	cursor = SDL_CreateColorCursor( surface, 0, 0 );
	
	SDL_SetCursor( cursor );
	SDL_FreeSurface( surface );
}

// ------------------------------------------------------------------------------------ //
// Задать размер окна
// ------------------------------------------------------------------------------------ //
void le::Window::SetSize( int Width, int Height )
{
	if ( !window ) return;

	SDL_SetWindowSize( window, Width, Height );
	SDL_SetWindowPosition( window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED );
}

// ------------------------------------------------------------------------------------ //
// Показывать ли курсор
// ------------------------------------------------------------------------------------ //
void le::Window::SetShowCursor( bool IsShow )
{
	if ( IsShow )
		SDL_SetRelativeMouseMode( SDL_FALSE );
	else
		SDL_SetRelativeMouseMode( SDL_TRUE );

	isShowCursor = IsShow;
}

// ------------------------------------------------------------------------------------ //
// Получить размер окна
// ------------------------------------------------------------------------------------ //
void le::Window::GetSize( UInt32_t& Width, UInt32_t& Height ) const
{
	if ( !window ) Width = Height = 0;
	SDL_GetWindowSize( window, ( int* ) &Width, ( int* ) &Height );
}

// ------------------------------------------------------------------------------------ //
// Открыто ли окно
// ------------------------------------------------------------------------------------ //
bool le::Window::IsOpen() const
{
	return window;
}

// ------------------------------------------------------------------------------------ //
// Получить заголовок окна
// ------------------------------------------------------------------------------------ //
le::WindowHandle_t le::Window::GetHandle() const
{
	return handle;
}

// ------------------------------------------------------------------------------------ //
// Получить событие из очереди событий
// ------------------------------------------------------------------------------------ //
bool le::Window::PollEvent( Event& Event )
{
	if ( !window )	return false;

	SDL_Event			sdlEvent;
	bool				isEndEvent = SDL_PollEvent( &sdlEvent );

	switch ( sdlEvent.type )
	{
	// Событие ввода текста
	case SDL_TEXTINPUT:
		// TODO: Эта хуйня мне не нравится, шото не уверен в таком виде хранения введеной строки
		Event.type = Event::ET_TEXT_INPUT;
		Event.textInputEvent.text = sdlEvent.text.text;
		break;

	// Событие нажатия и отжатия клавиш
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		if ( sdlEvent.type == SDL_KEYUP )
			Event.type = Event::ET_KEY_RELEASED;
		else
			Event.type = Event::ET_KEY_PRESSED;

		Event.key.isAlt = sdlEvent.key.keysym.mod & KMOD_ALT;
		Event.key.isCapsLock = sdlEvent.key.keysym.mod & KMOD_CAPS;
		Event.key.isControl = sdlEvent.key.keysym.mod & KMOD_CTRL;
		Event.key.isNumLock = sdlEvent.key.keysym.mod & KMOD_NUM;
		Event.key.isShift = sdlEvent.key.keysym.mod & KMOD_SHIFT;
		Event.key.isSuper = sdlEvent.key.keysym.mod & KMOD_GUI;
		Event.key.code = ButtonCode_ScanCodeToButtonCode( sdlEvent.key.keysym.scancode );
		break;

	// Событие нажатия и отжатия кнопок мыши
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		if ( sdlEvent.type == SDL_MOUSEBUTTONUP )
			Event.type = Event::ET_MOUSE_RELEASED;
		else
			Event.type = Event::ET_MOUSE_PRESSED;

		Event.mouseButton.code = ButtonCode_MouseButtonToButtonCode( sdlEvent.button.button );
		Event.mouseButton.x = sdlEvent.button.x;
		Event.mouseButton.y = sdlEvent.button.y;
		break;

	// Событие вращения колесика мыши
	case SDL_MOUSEWHEEL:
		Event.type = Event::ET_MOUSE_WHEEL;
		Event.mouseWheel.x = sdlEvent.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? sdlEvent.wheel.x * -1 : sdlEvent.wheel.x;
		Event.mouseWheel.y = sdlEvent.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? sdlEvent.wheel.y * -1 : sdlEvent.wheel.y;
		break;

	// Событие перемещения мышки
	case SDL_MOUSEMOTION:
		Event.type = Event::ET_MOUSE_MOVE;
		Event.mouseMove.x = sdlEvent.motion.x;
		Event.mouseMove.y = sdlEvent.motion.y;
		Event.mouseMove.xDirection = sdlEvent.motion.xrel;
		Event.mouseMove.yDirection = sdlEvent.motion.yrel;
		break;

	// События окна
	case SDL_WINDOWEVENT:
		switch ( sdlEvent.window.event )
		{
		// Событие закрытия окна
		case SDL_WINDOWEVENT_CLOSE:
			Event.type = Event::ET_WINDOW_CLOSE;
			break;

		// Событие изменения размеров окна
		case SDL_WINDOWEVENT_SIZE_CHANGED:
		case SDL_WINDOWEVENT_RESIZED:
			Event.type = Event::ET_WINDOW_RESIZE;
			Event.windowResize.width = sdlEvent.window.data1;
			Event.windowResize.height = sdlEvent.window.data2;
			break;

		// Событие получения фокуса 
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			Event.type = Event::ET_WINDOW_FOCUS_GAINED;
			if ( !isShowCursor ) SDL_SetRelativeMouseMode( SDL_TRUE );
			break;

		// Событие потери фокуса
		case SDL_WINDOWEVENT_FOCUS_LOST:
			Event.type = Event::ET_WINDOW_FOCUS_LOST;
			if ( !isShowCursor ) SDL_SetRelativeMouseMode( SDL_FALSE );
			break;
		}
		break;

	default:		Event.type = Event::ET_NONE;		break;
	}

	return isEndEvent;
}

// ------------------------------------------------------------------------------------ //
// Создать окно
// ------------------------------------------------------------------------------------ //
bool le::Window::Create( const char* Title, int Width, int Heigt, STYLE_WINDOW Style )
{
	if ( window ) Close();

	UInt32_t			flags = SDL_WINDOW_SHOWN;
	if ( Style & SW_FULLSCREEN )
		flags |= SDL_WINDOW_FULLSCREEN;
	else
	{
		if ( Style & SW_RESIZABLE )
			flags |= SDL_WINDOW_RESIZABLE;

		if ( !( Style & SW_DECORATED ) )
			flags |= SDL_WINDOW_BORDERLESS;
	}

	window = SDL_CreateWindow( Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Heigt, flags );
	if ( !window )
	{
		g_consoleSystem->PrintError( "Failed created window. SDL error: %s", SDL_GetError() );
		return false;
	}

	g_consoleSystem->PrintInfo( "Window created (%ix%i)", Width, Heigt );

	windowInfo = new SDL_SysWMinfo();
	SDL_VERSION( &windowInfo->version );
	SDL_GetWindowWMInfo( window, windowInfo );

#if defined( PLATFORM_WINDOWS )
	handle = windowInfo->info.win.window;
#else
	#error Unknown platform
#endif

	windowID = SDL_GetWindowID( window );
	return true;
}

// ------------------------------------------------------------------------------------ //
// Закрыть окно
// ------------------------------------------------------------------------------------ //
void le::Window::Close()
{
	if ( !window )	return;
	else SDL_DestroyWindow( window );

	if ( cursor )			SDL_FreeCursor( cursor );
	if ( !isShowCursor )	SDL_SetRelativeMouseMode( SDL_FALSE );
	if ( windowInfo )		delete windowInfo;

	g_consoleSystem->PrintInfo( "Window closed" );

	window = nullptr;
	handle = nullptr;
	cursor = nullptr;
	windowInfo = nullptr;
	isShowCursor = true;
	windowID = 0;
}

// ------------------------------------------------------------------------------------ //
// Задать заголовок окна
// ------------------------------------------------------------------------------------ //
void le::Window::SetHandle( WindowHandle_t WindowHandle )
{
	if ( window )	Close();
	handle = WindowHandle;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Window::Window() :
	isShowCursor( true ),
	windowID( 0 ),
	window( nullptr ),
	cursor( nullptr ),
	windowInfo( nullptr ),
	handle( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Window::~Window()
{
	Close();
}
