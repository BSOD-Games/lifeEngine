//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IGPUPROGRAM_H
#define IGPUPROGRAM_H

#include "common/types.h"
#include "common/shaderdescriptor.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IGPUProgram
	{
		virtual bool				Compile( const ShaderDescriptor& ShaderDescriptor, UInt32_t CountDefines = 0, const char** Defines = nullptr ) = 0;
		virtual void				Bind() = 0;
		virtual void				Unbind() = 0;
		virtual void				Clear() = 0;

		virtual void				SetUniform( const char* Name, int Value ) = 0;
		virtual void				SetUniform( const char* Name, float Value ) = 0;
		virtual void				SetUniform( const char* Name, bool Value ) = 0;
		virtual void				SetUniform( const char* Name, const Vector2D_t& Value ) = 0;
		virtual void				SetUniform( const char* Name, const Vector3D_t& Value ) = 0;
		virtual void				SetUniform( const char* Name, const Vector4D_t& Value ) = 0;
		virtual void				SetUniform( const char* Name, const Matrix4x4_t& Value ) = 0;

		virtual bool				IsCompile() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define GPUPROGRAM_INTERFACE_VERSION "LE_GPUProgram001"

//---------------------------------------------------------------------//

#endif // !IGPUPROGRAM_H

