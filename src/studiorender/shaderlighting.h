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

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class GPUProgram;

	//---------------------------------------------------------------------//

	class ShaderLighting
	{
	public:
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

