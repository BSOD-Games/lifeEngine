//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WGL_CONTEXT_H
#define WGL_CONTEXT_H

#include "rendercontext.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	bool					WinGL_CreateContext( WindowHandle_t WindowHandle, const SettingsContext& SettingsContext, ContextDescriptor_t& ContextDescriptor, ContextDescriptor_t* ShareContext = nullptr );
	bool					WinGL_MakeCurrentContext( const ContextDescriptor_t& ContextDescriptor );
	void					WinGL_DeleteContext( ContextDescriptor_t& ContextDescriptor );
	void					WinGL_SwapBuffers( const ContextDescriptor_t& ContextDescriptor );
	void					WinGL_SetVerticalSync( bool IsEnable = true );

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !WGL_CONTEXT_H
