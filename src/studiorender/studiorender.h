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

#include <vector>

#include "studiorender/istudiorenderinternal.h"
#include "studiorender/studiorenderviewport.h"
#include "studiorender/rendercontext.h"
#include "studiorender/studiorenderfactory.h"
#include "studiorender/scenedescriptor.h"
#include "studiorender/shadermanager.h"
#include "studiorender/gbuffer.h"
#include "studiorender/mesh.h"
#include "studiorender/gpuprogram.h"
#include "studiorender/quad.h"
#include "studiorender/sphere.h"
#include "studiorender/cone.h"
#include "studiorender/studiorenderdeviceconfigurations.h"

#include "shader_lighting.h"
#include "shader_depth.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class StudioRender : public IStudioRenderInternal
	{
	public:
		// IStudioRenderInternal
		virtual bool											Initialize( IEngine* Engine );
		virtual void											Begin();
		virtual void											End();
		virtual void											Present();

		// IStudioRender				
		virtual void											BeginScene( ICamera* Camera );
		virtual void											SubmitMesh( IMesh* Mesh, const Matrix4x4_t& Transformation );
		virtual void											SubmitMesh( IMesh* Mesh, const Matrix4x4_t& Transformation, UInt32_t StartSurface, UInt32_t CountSurface );
		virtual void											SubmitLight( IPointLight* PointLight );
		virtual void											SubmitLight( ISpotLight* SpotLight );
		virtual void											SubmitLight( IDirectionalLight* DirectionalLight );
		virtual void											EndScene();

		virtual void											SetVerticalSyncEnabled( bool IsEnabled = true );
		virtual void											SetViewport( const StudioRenderViewport& Viewport );

		virtual IFactory*										GetFactory() const;
		virtual IShaderManager*									GetShaderManager() const;
		virtual const StudioRenderViewport&						GetViewport() const;
		virtual const StudioRenderDeviceConfigurations&			GetDeviceConfigurations() const;
		
		// StudioRender
		StudioRender();
		~StudioRender();

	private:
		void								Render_GeometryPass( const SceneDescriptor& SceneDescriptor );
		void								Render_LightPass( const SceneDescriptor& SceneDescriptor );
		void								Render_FinalPass( const SceneDescriptor& SceneDescriptor );

		bool								isInitialize;

		RenderContext						renderContext;
		StudioRenderDeviceConfigurations	deviceConfigurations;
		StudioRenderFactory					studioRenderFactory;
		StudioRenderViewport				viewport;
		ShaderManager						shaderManager;
		GBuffer								gbuffer;
		Quad								quad;
		Sphere								sphere;
		Cone								cone;
		ShaderDepth							shaderDepth;
		ShaderLighting						shaderLighting;

		UInt32_t							currentScene;
		std::vector< SceneDescriptor >		scenes;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDER_INTERNAL_H
