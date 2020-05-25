//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WINDOW_H
#define WINDOW_H

#include "common/event.h"
#include "engine/iwindowinternal.h"

//---------------------------------------------------------------------//

struct SDL_Window;
struct SDL_Cursor;
struct SDL_SysWMinfo;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Window : public IWindowInternal
	{
	public:
		// IWindow
		virtual void				ResetCursor();

		virtual void				SetTitle( const char* Title );
		virtual void				SetIcon( const Image& Image );
		virtual void				SetCursor( const Image& Image );
		virtual void				SetSize( int Width, int Height );
		virtual void				SetShowCursor( bool IsShow = true );
		virtual void				SetFullscreen( bool IsFullscreen );

		virtual bool				IsOpen() const;
		virtual bool				IsShowingCursor() const;
		virtual bool				IsFullscreen() const;
		virtual void				GetSize( UInt32_t& Width, UInt32_t& Height ) const;
		virtual WindowHandle_t		GetHandle() const;

		// IWindowInternal
		virtual bool				PollEvent( Event& Event );
		virtual bool				Create( const char* Title, int Width, int Heigt, STYLE_WINDOW Style );
		virtual void				Close();

		virtual void				SetHandle( WindowHandle_t WindowHandle );

		// Window
		Window();
		~Window();

		inline SDL_Window*			GetSDLWindow() const		{ return window; };

	private:
		bool				isShowCursor;
		bool				isFullscreen;

		UInt32_t			windowID;

		SDL_Window*			window;
		SDL_Cursor*			cursor;
		SDL_SysWMinfo*		windowInfo;
		WindowHandle_t		handle;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !WINDOW_H

