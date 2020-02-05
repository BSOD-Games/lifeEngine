//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADER_LIGHTING_H
#define SHADER_LIGHTING_H

#include "common/types.h"
#include "engine/lifeengine.h"
#include "engine/icamera.h"

#include "pointlight.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class GPUProgram;

	//---------------------------------------------------------------------//

	class ShaderLighting
	{
	public:

		//---------------------------------------------------------------------//

		enum LIGHTING_TYPE
		{
			LT_POINT,
			LT_SPOT,
			LT_DIRECTIONAL
		};

		//---------------------------------------------------------------------//

		ShaderLighting();
		~ShaderLighting();

		bool				Create();
		void				Delete();
		inline void			Bind()
		{
			if ( !gpuProgram ) return;
			gpuProgram->Bind();
		}

		inline void			Unbind()
		{
			if ( !gpuProgram ) return;
			gpuProgram->Unbind();
		}

		inline void			SetSizeViewport( const Vector2D_t& SizeViewport )
		{
			if ( !gpuProgram ) return;
			
			Bind();
			gpuProgram->SetUniform( "screenSize", SizeViewport );
			Unbind();
		}

		inline void			SetCamera( ICamera* Camera )
		{
			if ( !gpuProgram ) return;
			LIFEENGINE_ASSERT( Camera );

			Bind();
			gpuProgram->SetUniform( "camera.position", Camera->GetPosition() );
			gpuProgram->SetUniform( "camera.invProjectionMatrix", glm::inverse( Camera->GetProjectionMatrix() ) );
			gpuProgram->SetUniform( "camera.invViewMatrix", glm::inverse( Camera->GetViewMatrix() ) );
			gpuProgram->SetUniform( "camera.near", Camera->GetNear() );
			gpuProgram->SetUniform( "camera.far", Camera->GetFar() );
			Unbind();
		}

		inline void			SetLight( const PointLight* PointLight )
		{
			if ( !gpuProgram ) return;
			LIFEENGINE_ASSERT( PointLight );

			Bind();
			gpuProgram->SetUniform( "light.position", PointLight->GetPosition() );
			gpuProgram->SetUniform( "light.color", PointLight->GetColor() );
			gpuProgram->SetUniform( "light.specular", PointLight->GetSpecular() );
			gpuProgram->SetUniform( "light.intensivity", PointLight->GetIntensivity() );
			gpuProgram->SetUniform( "light.radius", PointLight->GetRadius() );
			Unbind();
		}

		inline GPUProgram*	GetHandle() const
		{
			return gpuProgram;
		}

	private:
		GPUProgram*			gpuProgram;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADER_LIGHTING_H

