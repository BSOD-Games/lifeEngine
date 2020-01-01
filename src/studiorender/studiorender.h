//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef STUDIORENDER_H
#define STUDIORENDER_H

#include <unordered_map>
#include <vector>

#include "studiorender/istudiorenderinternal.h"
#include "studiorender/studiorenderviewport.h"
#include "studiorender/rendercontext.h"
#include "studiorender/studiorenderfactory.h"
#include "studiorender/scenedescriptor.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class StudioRender : public IStudioRenderInternal
	{
	public:
		// IStudioRenderInternal
		virtual bool							Initialize( IEngine* Engine );
		virtual void							Begin();
		virtual void							End();
		virtual void							Present();

		// IStudioRender
		virtual void							BeginScene( ICamera* Camera );
		virtual void							SubmitMesh( IMesh* Mesh, const Matrix4x4_t& Transformation );
		virtual void							SubmitMesh( IMesh* Mesh, const Matrix4x4_t& Transformation, UInt32_t StartSurface, UInt32_t CountSurface );
		virtual void							EndScene();

		virtual void							SetDepthTestEnabled( bool IsEnabled = true );
		virtual void							SetCullFaceEnabled( bool IsEnabled = true );
		virtual void							SetBlendEnabled( bool IsEnabled = true );
		virtual void							SetCullFaceType( CULLFACE_TYPE CullFaceType );
		virtual void							SetVerticalSyncEnabled( bool IsEnabled = true );
		virtual void							SetViewport( const StudioRenderViewport& Viewport );

		virtual IFactory*						GetFactory() const;
		virtual const StudioRenderViewport&		GetViewport() const;
		
		// StudioRender
		StudioRender();
		~StudioRender();

		void									InitOpenGLStates( const OpenGLState& OpenGLState );

	private:
		bool								isInitialize;

		OpenGLState							openGLState;
		RenderContext						renderContext;
		StudioRenderFactory					studioRenderFactory;
		StudioRenderViewport				viewport;

		UInt32_t							currentScene;
		std::vector< SceneDescriptor >		scenes;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDER_INTERNAL_H
