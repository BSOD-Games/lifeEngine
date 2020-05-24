//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef GPUPROGRAM_H
#define GPUPROGRAM_H

#include <GL/glew.h>
#include <string>
#include <unordered_map>

#include "studiorender/igpuprogram.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class GPUProgram : public IGPUProgram
	{
	public:
        // IReferenceObject
        virtual void                IncrementReference();
        virtual void                DecrementReference();
        virtual void                Release();
        virtual UInt32_t            GetCountReferences() const;

		// IGPUProgram
		virtual bool				Compile( const ShaderDescriptor& ShaderDescriptor, UInt32_t CountDefines = 0, const char** Defines = nullptr );
		virtual void				Bind();
		virtual void				Unbind();
		virtual void				Clear();

		virtual void				SetUniform( const char* Name, int Value );
		virtual void				SetUniform( const char* Name, float Value );
		virtual void				SetUniform( const char* Name, bool Value );
		virtual void				SetUniform( const char* Name, const Vector2D_t& Value );
		virtual void				SetUniform( const char* Name, const Vector3D_t& Value );
		virtual void				SetUniform( const char* Name, const Vector4D_t& Value );
		virtual void				SetUniform( const char* Name, const Matrix4x4_t& Value );

		virtual bool				IsCompile() const;

		// GPUProgram
		GPUProgram();
		~GPUProgram();

		inline UInt32_t				GetHandle() const
		{
			return programID;
		}

	private:
		// GPUProgram
		bool						Compile_VertexShader( const char* Code );
		bool						Compile_GeometryShader( const char* Code );
		bool						Compile_FragmentShader( const char* Code );
		bool						Link();
		bool						IsUniformExists( const char* Name );
		UInt32_t					GetUniformLocation( const char* Name );

		GLuint						vertexShaderID;
		GLuint						geometryShaderID;
		GLuint						fragmentShaderID;
		GLuint						programID;
        UInt32_t                    countReferences;

		std::unordered_map< std::string, UInt32_t >		uniforms;       
    };

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !GPUPROGRAM_H

