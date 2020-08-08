//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUASTUDIORENDER_H
#define LUASTUDIORENDER_H

#include "common/types.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAVector3D;
	class LUACamera;
	class LUAModel;

	//---------------------------------------------------------------------//

	class LUAStudioRender
	{
	public:
		static void				Register( lua_State* LuaVM );
		static void				BeginScene( const LUACamera& Camera );
		static void				SubmitDebugLine( const LUAVector3D& From, const LUAVector3D& To, const LUAVector3D& Color );
		static void				SubmitDebugPoint( const LUAVector3D& Position, const LUAVector3D& Color );
		//static void				SubmitText( IText* Text ) = 0;
		//static void				SubmitSprite( ISprite* Sprite ) = 0;
		static void				SubmitModel( const LUAModel& Model );
		static void				SubmitModel( const LUAModel& Model, UInt32_t StartSurface, UInt32_t CountSurface );
		//static void				SubmitLight( IPointLight* PointLight ) = 0;
		//static void				SubmitLight( ISpotLight* SpotLight ) = 0;
		//static void				SubmitLight( IDirectionalLight* DirectionalLight ) = 0;
		static void				EndScene();

		static void				SetVerticalSyncEnabled( bool IsEnabled = true );
		//static void				SetViewport( const StudioRenderViewport& Viewport ) = 0;

		//static const StudioRenderViewport& GetViewport() const = 0;
		//static const StudioRenderDeviceConfigurations& GetDeviceConfigurations() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUASTUDIORENDER_H
