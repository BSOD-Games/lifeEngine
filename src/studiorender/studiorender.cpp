//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>

#include "common/configurations.h"
#include "engine/lifeengine.h"
#include "engine/iengine.h"
#include "engine/iwindow.h"
#include "engine/iconsolesystem.h"
#include "settingscontext.h"

#include "global.h"
#include "studiorender.h"

LIFEENGINE_STUDIORENDER_API( le::StudioRender );

// ------------------------------------------------------------------------------------ //
// ���������������� ������
// ------------------------------------------------------------------------------------ //
bool le::StudioRender::Initialize( IEngine* Engine )
{
	LIFEENGINE_ASSERT( Engine );

	g_consoleSystem = Engine->GetConsoleSystem();

	// ���� � ���� ���� �� ������� (��������� �� IWindow nullptr) ���
	// ��������� ���� nullptr, �� ����������� ������

	if ( !Engine->GetWindow() || !Engine->GetWindow()->GetHandle() )
	{
		g_consoleSystem->PrintError( "Window not open or not valid handle" );
		return false;
	}

	// ������� �������� OpenGL

	Configurations				configurations = Engine->GetConfigurations();
	SettingsContext				settingsContext;
	settingsContext.redBits = 8;
	settingsContext.greenBits = 8;
	settingsContext.blueBits = 8;
	settingsContext.alphaBits = 8;
	settingsContext.depthBits = 24;
	settingsContext.stencilBits = 8;
	settingsContext.majorVersion = 3;
	settingsContext.minorVersion = 3;
	settingsContext.attributeFlags = SettingsContext::CA_CORE;

	if ( !renderContext.Create( Engine->GetWindow()->GetHandle(), settingsContext ) )
	{
		g_consoleSystem->PrintError( "Failed created context" );
		return false;
	}

	renderContext.SetVerticalSync( configurations.isVerticalSinc );

	// �������������� OpenGL

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	return true;
}

// ------------------------------------------------------------------------------------ //
// �������� ������ ��������
// ------------------------------------------------------------------------------------ //
void le::StudioRender::ResizeViewport( UInt32_t X, UInt32_t Y, UInt32_t Width, UInt32_t Height )
{
	if ( !renderContext.IsCreated() ) return;
	glViewport( X, Y, Width, Height );
}

// ------------------------------------------------------------------------------------ //
// ���������� ����
// ------------------------------------------------------------------------------------ //
void le::StudioRender::RenderFrame()
{
	if ( !renderContext.IsCreated() ) return;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	renderContext.SwapBuffers();
}

// ------------------------------------------------------------------------------------ //
// �������� ������������ �������������
// ------------------------------------------------------------------------------------ //
void le::StudioRender::SetVerticalSyncEnabled( bool IsEnabled )
{
	if ( !renderContext.IsCreated() ) return;
	renderContext.SetVerticalSync( IsEnabled );
}
