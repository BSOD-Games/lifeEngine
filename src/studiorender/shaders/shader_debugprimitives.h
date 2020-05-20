//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADER_DEBUGPRIMITIVES_H
#define SHADER_DEBUGPRIMITIVES_H

#include "common/types.h"

#include "scenedescriptor.h"
#include "gpuprogram.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ShaderDebugPrimitives
	{
	public:
		ShaderDebugPrimitives();
		~ShaderDebugPrimitives();

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

		inline void			SetProjectionMatrix( const Matrix4x4_t& ProjectionMatrix  )
		{
			if ( !gpuProgram ) return;

			Bind();
			gpuProgram->SetUniform( "matrixProjection", ProjectionMatrix );
			Unbind();
		}

		inline void			SetPrimitive( UInt32_t Index, const Line& Line )
		{
			if ( !gpuProgram ) return;

			Bind();
			gpuProgram->SetUniform( ( "primitives[" + std::to_string( Index ) + "].verteces[0]" ).c_str(), Line.from );
			gpuProgram->SetUniform( ( "primitives[" + std::to_string( Index ) + "].verteces[1]" ).c_str(), Line.to );
			gpuProgram->SetUniform( ( "primitives[" + std::to_string( Index ) + "].color" ).c_str(), Line.color );
			Unbind();
		}

		inline void			SetPrimitive( UInt32_t Index, const Point& Point )
		{
			if ( !gpuProgram ) return;

			Bind();
			gpuProgram->SetUniform( ( "primitives[" + std::to_string( Index ) + "].verteces[0]" ).c_str(), Point.position );
			gpuProgram->SetUniform( ( "primitives[" + std::to_string( Index ) + "].color" ).c_str(), Point.color );
			Unbind();
		}

		inline void			SetColor( const Vector3D_t& Color )
		{
			if ( !gpuProgram ) return;

			Bind();
			gpuProgram->SetUniform( "colorPrimitive", Color );
			Unbind();
		}

	private:
		GPUProgram*			gpuProgram;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADER_DEBUGPRIMITIVES_H