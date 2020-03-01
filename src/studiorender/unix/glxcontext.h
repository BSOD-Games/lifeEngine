//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://gitlab.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef GLX_CONTEXT_H
#define GLX_CONTEXT_H

#include "rendercontext.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    bool            GLX_CreateContext( WindowHandle_t WindowHandle, const SettingsContext& SettingsContext, ContextDescriptor_t& ContextDescriptor, ContextDescriptor_t* ShareContext = nullptr );
    bool            GLX_MakeCurrentContext( const ContextDescriptor_t& ContextDescriptor );
    void            GLX_DeleteContext( ContextDescriptor_t& ContextDescriptor );
    void            GLX_SwapBuffers( const ContextDescriptor_t& ContextDescriptor );
    void            GLX_SetVerticalSync( bool IsEnable = true );

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !GLX_CONTEXT_H
