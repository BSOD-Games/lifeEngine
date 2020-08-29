// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "Misc/Types.h"
#include "Logging/LogMacros.h"
#include "System/Window.h"

/**
 * Constructor
 */
le::Window::Window() :
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
	const char*			title = "Unknown";
	
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

	SDL_ShowSimpleMessageBox( flags, title, InMessage.c_str(), Window::GetInstance()->window );
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
bool le::Window::PollEvent( Event& OutEvent )
{
	// If window not opened or he is native - escape from method
	if ( !window )		return false;
	
	// Get from SDL2 window event
	SDL_Event			sdlEvent;
	bool				isEndEvent = SDL_PollEvent( &sdlEvent );

	switch ( sdlEvent.type )
	{
		// Window events
		case SDL_WINDOWEVENT:
			switch ( sdlEvent.window.event )
			{
				// Window close
			case SDL_WINDOWEVENT_CLOSE:
				OutEvent.type = ET_WindowClose;
				break;
			}
			break;

		default:		OutEvent.type = ET_None;	break;
	}

	return isEndEvent;
}
