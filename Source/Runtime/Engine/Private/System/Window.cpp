// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "Misc/Types.h"
#include "Logging/LogMacros.h"
#include "System/Window.h"
#include "System/ButtonTranslation.h"

/**
 * Constructor
 */
le::Window::Window() :
	isShowCursor( true ),
	handle( nullptr ),
	window( nullptr ),
	cursor( nullptr ),
	sysWMinfo( nullptr )
{}

/**
 * Destructor
 */
le::Window::~Window()
{
	Close();
}

/**
 * Show message box
 */
void le::Window::ShowMessageBox( const std::string& InMessage, EMessageBoxType InType )
{
	uint32				flags = 0;
	const char* title = "Unknown";

	switch ( InType )
	{
	case MBT_Info:
		flags = SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION;
		title = "Info lifeEngine";
		break;

	case MBT_Warning:
		flags = SDL_MessageBoxFlags::SDL_MESSAGEBOX_WARNING;
		title = "Warning lifeEngine";
		break;

	case MBT_Error:
		flags = SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR;
		title = "Error lifeEngine";
		break;
	}

	SDL_ShowSimpleMessageBox( flags, title, InMessage.c_str(), GWindow->window );
}

/**
 * Open window
 */
bool le::Window::Open( const std::string& InTitle, uint32 InWidth, uint32 InHeight, EStyleWindow InStyleWindow )
{
	// If window allready opened - close this window
	if ( window )		Close();

	// Prepare flags for style window
	uint32		flags = SDL_WINDOW_SHOWN;
	if ( InStyleWindow & SW_Fullscreen )
		flags |= SDL_WINDOW_FULLSCREEN;
	else
	{
		if ( InStyleWindow & SW_Resizable )
			flags |= SDL_WINDOW_RESIZABLE;

		if ( !( InStyleWindow & SW_Decorated ) )
			flags |= SDL_WINDOW_BORDERLESS;
	}

	// Open window
	window = SDL_CreateWindow( InTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, InWidth, InHeight, flags );
	if ( !window )
	{
		// TODO: change if's on try/catch
		LIFEENGINE_LOG_ERROR( "Engine", "Failed created window. SDL error: %s", SDL_GetError() );
		return false;
	}

	LIFEENGINE_LOG_INFO( "Engine", "Window created (%ix%i)", InWidth, InHeight );

	// Get handle on window
	sysWMinfo = new SDL_SysWMinfo();
	SDL_VERSION( &sysWMinfo->version );
	SDL_GetWindowWMInfo( window, sysWMinfo );

#ifdef PLATFORM_WINDOWS
	handle = sysWMinfo->info.win.window;
#else
	#error Unknown platform
#endif

	return true;
}

/**
 * Close window
 */
void le::Window::Close()
{
	// If window opened and not native - close this window
	if ( !window ) return;

	SDL_DestroyWindow( window );
	if ( cursor )		SDL_FreeCursor( cursor );
	if ( sysWMinfo )	delete sysWMinfo;

	LIFEENGINE_LOG_INFO( "Engine", "Window closed" );

	window = nullptr;
	handle = nullptr;
	cursor = nullptr;
	sysWMinfo = nullptr;
}

/**
 * Poll event
 */
bool le::Window::PollEvent( SEvent& OutEvent )
{
	// If window not opened or he is native - escape from method
	if ( !window )		return false;

	// Get from SDL2 window event
	SDL_Event			sdlEvent;
	bool				isEndEvent = SDL_PollEvent( &sdlEvent );

	switch ( sdlEvent.type )
	{
		// Input text event:
	case SDL_TEXTINPUT:
		OutEvent.type = SEvent::ET_TextInput;
		OutEvent.event.textInputEvent.text = sdlEvent.text.text;
		break;

		// Keyboard input events
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		if ( sdlEvent.type == SDL_KEYUP )
			OutEvent.type = SEvent::ET_KeyReleased;
		else
			OutEvent.type = SEvent::ET_KeyPressed;

		OutEvent.event.key.isAlt = sdlEvent.key.keysym.mod & KMOD_ALT;
		OutEvent.event.key.isCapsLock = sdlEvent.key.keysym.mod & KMOD_CAPS;
		OutEvent.event.key.isControl = sdlEvent.key.keysym.mod & KMOD_CTRL;
		OutEvent.event.key.isNumLock = sdlEvent.key.keysym.mod & KMOD_NUM;
		OutEvent.event.key.isShift = sdlEvent.key.keysym.mod & KMOD_SHIFT;
		OutEvent.event.key.isSuper = sdlEvent.key.keysym.mod & KMOD_GUI;
		OutEvent.event.key.code = ScanCodeToButtonCode( sdlEvent.key.keysym.scancode );
		break;

		// Mouse input events
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		if ( sdlEvent.type == SDL_MOUSEBUTTONUP )
			OutEvent.type = SEvent::ET_MouseReleased;
		else
			OutEvent.type = SEvent::ET_MousePressed;

		OutEvent.event.mouseButton.code = MouseButtonToButtonCode( sdlEvent.button.button );
		OutEvent.event.mouseButton.x = sdlEvent.button.x;
		OutEvent.event.mouseButton.y = sdlEvent.button.y;
		break;

		// Mouse whell events
	case SDL_MOUSEWHEEL:
		OutEvent.type = SEvent::ET_MouseWheel;
		OutEvent.event.mouseWheel.x = sdlEvent.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? sdlEvent.wheel.x * -1 : sdlEvent.wheel.x;
		OutEvent.event.mouseWheel.y = sdlEvent.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? sdlEvent.wheel.y * -1 : sdlEvent.wheel.y;
		break;

		// Mouse move events
	case SDL_MOUSEMOTION:
		OutEvent.type = SEvent::ET_MouseMove;
		OutEvent.event.mouseMove.x = sdlEvent.motion.x;
		OutEvent.event.mouseMove.y = sdlEvent.motion.y;
		OutEvent.event.mouseMove.xDirection = sdlEvent.motion.xrel;
		OutEvent.event.mouseMove.yDirection = sdlEvent.motion.yrel;
		break;

		// Window events
	case SDL_WINDOWEVENT:
		switch ( sdlEvent.window.event )
		{
			// Window close
		case SDL_WINDOWEVENT_CLOSE:
			OutEvent.type = SEvent::ET_WindowClose;
			break;

			// Event change size window
		case SDL_WINDOWEVENT_SIZE_CHANGED:
		case SDL_WINDOWEVENT_RESIZED:
			OutEvent.type = SEvent::ET_WindowResize;
			OutEvent.event.windowResize.width = static_cast< float >( sdlEvent.window.data1 );
			OutEvent.event.windowResize.height = static_cast< float >( sdlEvent.window.data2 );
			break;

			// Event gained focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			OutEvent.type = SEvent::ET_WindowFocusGained;
			if ( !isShowCursor ) SDL_SetRelativeMouseMode( SDL_TRUE );
			break;

			// Event lost focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			OutEvent.type = SEvent::ET_WindowFocusLost;
			if ( !isShowCursor ) SDL_SetRelativeMouseMode( SDL_FALSE );
			break;
		}
		break;

	default:		OutEvent.type = SEvent::ET_None;	break;
	}

	return isEndEvent;
}
