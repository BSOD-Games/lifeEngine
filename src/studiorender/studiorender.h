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
#include "studiorender/gbuffer.h"
#include "studiorender/mesh.h"
#include "studiorender/gpuprogram.h"
#include "studiorender/quad.h"
#include "studiorender/sphere.h"
#include "studiorender/cone.h"
#include "studiorender/studiorenderdeviceconfigurations.h"

#include "shaders/shader_lighting.h"
#include "shaders/shader_depth.h"
#include "shaders/shader_postprocess.h"
#include "shaders/shader_debugprimitives.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class StudioRender : public IStudioRenderInternal
	{
	public:
		// IStudioRenderInternal
		virtual bool											Initialize( IEngine* Engine );
		virtual bool											CreateContext( WindowHandle_t WindowHandle, UInt32_t Width, UInt32_t Height );
		virtual void											Begin();
		virtual void											End();
		virtual void											Present();

		// IStudioRender				
		virtual void											BeginScene( ICamera* Camera );
		virtual void											SubmitDebugLine( const Vector3D_t& From, const Vector3D_t& To, const Vector3D_t& Color );
		virtual void											SubmitDebugPoint( const Vector3D_t& Position, const Vector3D_t& Color );
		virtual void											SubmitMesh( IMesh* Mesh, const Matrix4x4_t& Transformation );
		virtual void											SubmitMesh( IMesh* Mesh, const Matrix4x4_t& Transformation, UInt32_t StartSurface, UInt32_t CountSurface );
		virtual void											SubmitLight( IPointLight* PointLight );
		virtual void											SubmitLight( ISpotLight* SpotLight );
		virtual void											SubmitLight( IDirectionalLight* DirectionalLight );
		virtual void											EndScene();

		virtual void											SetVerticalSyncEnabled( bool IsEnabled = true );
		virtual void											SetViewport( const StudioRenderViewport& Viewport );

		virtual IFactory*										GetFactory() const;
		virtual const StudioRenderViewport&						GetViewport() const;
		virtual const StudioRenderDeviceConfigurations&			GetDeviceConfigurations() const;
		
		// StudioRender
		StudioRender();
		~StudioRender();

	private:
		void								Render_GeometryPass();
		void								Render_LightPass();
		void								Render_FinalPass();

		bool								isInitialize;
		bool								isNeedRenderDebugPrimitives;

		RenderContext						renderContext;
		StudioRenderDeviceConfigurations	deviceConfigurations;
		StudioRenderFactory					studioRenderFactory;
		StudioRenderViewport				viewport;
		GBuffer								gbuffer;
		Quad								quad;
		Sphere								sphere;
		Cone								cone;
		ShaderDepth							shaderDepth;
		ShaderLighting						shaderLighting;
		ShaderPostprocess					shaderPostrocess;
		ShaderDebugPrimitives				shaderDebugPrimitives;

		VertexArrayObject					vao_DebugPrimitive;
		VertexBufferObject					vbo_DebugPrimitive;

		UInt32_t							currentScene;
		std::vector< SceneDescriptor >		scenes;		
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDER_INTERNAL_H
