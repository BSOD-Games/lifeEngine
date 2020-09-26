// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <qapplication.h>
#include <qmessagebox.h>

#include <Engine.h>
#include <Misc/EngineGlobals.h>
#include <Logging/LogMacros.h>
#include <Rendering/RenderSystem.h>
#include <System/Window.h>
#include <System/FileSystem.h>
#include <System/InputSystem.h>
#include <Resources/ResourceSystem.h>

#include <Resources/Resource.h>
#include <Resources/Texture2D.h>

// ------------------------------------------------------------------------------------ //
// Main function
// ------------------------------------------------------------------------------------ //
int main( int argc, char** argv )
{
	le::GFileSystem->SetRootPath( "../../" );

	le::GEngine->Initialize( "../../lifeEditor.log" );
	le::GWindow->Open( "lifeEditor", 800, 600 );
	le::FRHIContext rhiContext = le::GRenderSystem->CreateContext( le::GWindow->GetHandle() );
	le::GRenderSystem->MakeCurrentContext( rhiContext );
	le::GRenderSystem->SetViewport( 0, 0, 800, 600 );

	le::Texture2D*		r = le::Object::Cast< le::Texture2D >( le::GResourceSystem->FindResource( "", le::RT_Texture2D ) );

	while ( le::GWindow->IsOpen() )
	{
		le::SEvent		event;
		while ( le::GWindow->PollEvent( event ) )
		{
			switch ( event.type )
			{
			case le::SEvent::ET_WindowClose:
				le::GWindow->Close();
				break;

			case le::SEvent::ET_KeyPressed:
			case le::SEvent::ET_KeyReleased:
			case le::SEvent::ET_MouseMove:
			case le::SEvent::ET_MousePressed:
			case le::SEvent::ET_MouseReleased:
			case le::SEvent::ET_MouseWheel:
			case le::SEvent::ET_TextInput:
				le::GInputSystem->ApplyEvent( event );
				break;

			case le::SEvent::ET_WindowResize:
				le::GRenderSystem->SetViewport( 0, 0, event.event.windowResize.width, event.event.windowResize.height );
				break;

			default: break;
			}
		}

		le::GRenderSystem->Begin();
		le::GRenderSystem->End();
		le::GRenderSystem->Present();
		le::GInputSystem->Reset();
	}
	
	return 0;
}

//---------------------------------------------------------------------//

#ifdef PLATFORM_WINDOWS
#include <Windows.h>

// ------------------------------------------------------------------------------------ //
// Windows main function
// ------------------------------------------------------------------------------------ //
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpCmdLine, int nCmdShow )
{
	return main( 0, nullptr );
}

#endif // PLATFORM_WINDOWS

//---------------------------------------------------------------------//