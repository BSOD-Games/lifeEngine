//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/ifactory.h"
#include "engine/icamera.h"
#include "studiorender/igpuprogram.h"
#include "studiorender/itexture.h"

#include "global.h"
#include "testshader.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать экземпляр шейдера
// ------------------------------------------------------------------------------------ //
bool le::TestShader::InitInstance( UInt32_t CountParams, IMaterialVar** MaterialVars )
{
	gpuProgram = ( IGPUProgram* ) g_studioRenderFactory->Create( GPUPROGRAM_INTERFACE_VERSION );
	if ( !gpuProgram ) return false;

	ShaderDescriptor			shaderDescriptor;
	shaderDescriptor.vertexShaderSource = " \
	#version 330 core\n \
	\n \
	layout( location = 0 ) 			in vec3 vertex_position; \n \
	layout( location = 2 ) 			in vec2 vertex_texCoords; \n \
	\n \
		out vec2 				texCoords; \n \
	\n \
		uniform mat4    		matrix_Projection; \n \
		uniform mat4			matrix_Transformation; \n \
	\n \
	void main() \n \
	{\n \
		texCoords = vertex_texCoords; \n \
		gl_Position = matrix_Projection * matrix_Transformation * vec4( vertex_position, 1.f ); \n \
	}";

	shaderDescriptor.fragmentShaderSource = " \
	#version 330 core\n\
	\n\
		in vec2 				texCoords;\n\
		out vec4				color;\n\
	\n\
		uniform sampler2D		basetexture;\n\
	\n\
	void main()\n\
	{\n\
		color = texture2D( basetexture, texCoords );\n\
	}\n";

	if ( !gpuProgram->Compile( shaderDescriptor ) )
	{
		g_studioRenderFactory->Delete( gpuProgram );
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// Подготовка к отрисовке элементов
// ------------------------------------------------------------------------------------ //
void le::TestShader::OnDrawElements( UInt32_t CountParams, IMaterialVar** MaterialVars, const Matrix4x4_t& Transformation, ICamera* Camera )
{
	if ( !gpuProgram ) return;
	MaterialVars[ 0 ]->GetValueTexture()->Bind();

	gpuProgram->Bind();
	gpuProgram->SetUniform( "matrix_Transformation", Transformation );
	gpuProgram->SetUniform( "matrix_Projection", Camera->GetProjectionMatrix() * Camera->GetViewMatrix() );
}

// ------------------------------------------------------------------------------------ //
// Получить название шейдера
// ------------------------------------------------------------------------------------ //
const char* le::TestShader::GetName() const
{
	return "testshader";
}

// ------------------------------------------------------------------------------------ //
// Получить запасной шейдер
// ------------------------------------------------------------------------------------ //
const char* le::TestShader::GetFallbackShader() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Получить количество параметров
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::TestShader::GetCountParams() const
{
	return shaderParams.size();
}

// ------------------------------------------------------------------------------------ //
// Получить массив параметров
// ------------------------------------------------------------------------------------ //
le::ShaderParamInfo* le::TestShader::GetParams() const
{
	return ( ShaderParamInfo* ) shaderParams.data();
}

// ------------------------------------------------------------------------------------ //
// Получить параметр
// ------------------------------------------------------------------------------------ //
le::ShaderParamInfo* le::TestShader::GetParam( UInt32_t Index ) const
{
	if ( Index >= shaderParams.size() ) return nullptr;
	return ( ShaderParamInfo* ) &shaderParams[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::TestShader::TestShader() :
	gpuProgram( nullptr )
{
	shaderParams =
	{
		{ "basetexture",	MVT_TEXTURE		}
	};
}
