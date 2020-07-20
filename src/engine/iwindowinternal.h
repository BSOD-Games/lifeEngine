//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IWINDOW_INTERNAL_H
#define IWINDOW_INTERNAL_H

#include "engine/iwindow.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct Event;

	//---------------------------------------------------------------------//

	enum STYLE_WINDOW
	{
		SW_NONE			= 0,					
		SW_RESIZABLE	= 1 << 0,				
		SW_DECORATED	= 1 << 1,				
		SW_FLOATING		= 1 << 2,				
		SW_FULLSCREEN	= 1 << 3,		

		SW_DEFAULT = SW_DECORATED | SW_RESIZABLE
	};

	//---------------------------------------------------------------------//

	class IWindowInternal : public IWindow
	{
	public:
		virtual bool				PollEvent( Event& Event ) = 0;
		virtual bool				Create( const char* Title, int Width, int Heigt, STYLE_WINDOW Style ) = 0;
		virtual void				Close() = 0;

		virtual void				SetHandle( WindowHandle_t WindowHandle ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IWINDOW_INTERNAL_H
