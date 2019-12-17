//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IWINDOW_H
#define IWINDOW_H

#include "engine/lifeengine.h"
#include "common/types.h"
#include "common/image.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IWindow
	{
	public:
		virtual void				ResetCursor() = 0;
		
		virtual void				SetTitle( const char* Title ) = 0;
		virtual void				SetIcon( const Image& Image ) = 0;
		virtual void				SetCursor( const Image& Image ) = 0;
		virtual void				SetSize( int Width, int Height ) = 0;
		virtual void				SetShowCursor( bool IsShow = true ) = 0;

		virtual void				GetSize( UInt32_t& Width, UInt32_t& Height ) const = 0;
		virtual bool				IsOpen() const = 0;
		virtual WindowHandle_t		GetHandle() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IWINDOW_H
