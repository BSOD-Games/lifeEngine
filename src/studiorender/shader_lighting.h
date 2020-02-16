//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADER_LIGHTING_H
#define SHADER_LIGHTING_H

#include <unordered_map>

#include "common/types.h"
#include "engine/lifeengine.h"
#include "engine/icamera.h"

#include "pointlight.h"
#include "spotlight.h"
#include "directionallight.h"

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

		inline void			SetType( LIGHTING_TYPE Type )
		{
			gpuProgram = gpuPrograms[ Type ];
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
			Unbind();
		}

		inline void			SetPVTMatrix( const Matrix4x4_t& PVTMatrix )
		{
			if ( !gpuProgram ) return;

			Bind();
			gpuProgram->SetUniform( "pvtMatrix", PVTMatrix );
			Unbind();
		};

		inline void			SetLight( PointLight* PointLight )
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

		inline void			SetLight( SpotLight* SpotLight )
		{
			if ( !gpuProgram ) return;
			LIFEENGINE_ASSERT( SpotLight );

			Bind();
			gpuProgram->SetUniform( "light.position", SpotLight->GetPosition() );
			gpuProgram->SetUniform( "light.color", SpotLight->GetColor() );
			gpuProgram->SetUniform( "light.specular", SpotLight->GetSpecular() );
			gpuProgram->SetUniform( "light.intensivity", SpotLight->GetIntensivity() );
			gpuProgram->SetUniform( "light.radius", SpotLight->GetRadius() );
			gpuProgram->SetUniform( "light.height", SpotLight->GetHeight() );
			gpuProgram->SetUniform( "light.cutoff", SpotLight->GetCutoff() );
			gpuProgram->SetUniform( "light.direction", SpotLight->GetDirection() );
			Unbind();
		}		

		inline void			SetLight( DirectionalLight* DirectionalLight )
		{
			if ( !gpuProgram ) return;
			LIFEENGINE_ASSERT( DirectionalLight );

			Bind();
			gpuProgram->SetUniform( "light.color", DirectionalLight->GetColor() );
			gpuProgram->SetUniform( "light.specular", DirectionalLight->GetSpecular() );
			gpuProgram->SetUniform( "light.intensivity", DirectionalLight->GetIntensivity() );
			gpuProgram->SetUniform( "light.direction", DirectionalLight->GetDirection() );
			Unbind();
		}

	private:
		GPUProgram*												gpuProgram;
		std::unordered_map< LIGHTING_TYPE, GPUProgram* >		gpuPrograms;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADER_LIGHTING_H

