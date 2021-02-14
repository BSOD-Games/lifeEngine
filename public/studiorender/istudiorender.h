//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISTUDIORENDER_H
#define ISTUDIORENDER_H

#include "common/types.h"
#include "engine/lifeengine.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IFactory;
    class ICamera;
    class IText;
	class ISprite;
	class IModel;
    class IPointLight;
    class ISpotLight;
    class IDirectionalLight;
    struct StudioRenderViewport;
    struct StudioRenderDeviceConfigurations;

    //---------------------------------------------------------------------//

    enum CULLFACE_TYPE
    {
        CT_FRONT,
        CT_BACK
    };

    //---------------------------------------------------------------------//

    class IStudioRender
    {
    public:
        virtual ~IStudioRender() {}
        virtual void										BeginScene( ICamera* Camera ) = 0;
        virtual void										SubmitDebugLine( const Vector3D_t& From, const Vector3D_t& To, const Vector3D_t& Color ) = 0;
        virtual void										SubmitDebugPoint( const Vector3D_t& Position, const Vector3D_t& Color ) = 0;
		virtual void										SubmitText( IText* Text ) = 0;
		virtual void										SubmitSprite( ISprite* Sprite ) = 0;
		virtual void										SubmitModel( IModel* Model ) = 0;
		virtual void										SubmitModel( IModel* Model, UInt32_t StartSurface, UInt32_t CountSurface ) = 0;
        virtual void										SubmitLight( IPointLight* PointLight ) = 0;
        virtual void										SubmitLight( ISpotLight* SpotLight ) = 0;
        virtual void										SubmitLight( IDirectionalLight* DirectionalLight ) = 0;
        virtual void										EndScene() = 0;

        virtual void										SetVerticalSyncEnabled( bool IsEnabled = true ) = 0;
        virtual void										SetViewport( const StudioRenderViewport& Viewport ) = 0;

        virtual IFactory*									GetFactory() const = 0;
        virtual const StudioRenderViewport&					GetViewport() const = 0;
        virtual const StudioRenderDeviceConfigurations&		GetDeviceConfigurations() const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDER_H
