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

		virtual void				GetSize( int& Width, int& Height ) const;
		virtual bool				IsOpen() const;
		virtual WindowHandle_t		GetHandle() const;

		// IWindowInternal
		virtual bool				PollEvent( Event& Event );
		virtual bool				Create( const char* Title, int Width, int Heigt, STYLE_WINDOW Style );
		virtual void				Close();

		virtual void				SetHandle( WindowHandle_t WindowHandle );

		// Window
		Window();
		~Window();

	private:
		bool				isShowCursor;
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

