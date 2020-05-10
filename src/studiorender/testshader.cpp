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
bool le::TestShader::InitInstance( UInt32_t CountParams, IShaderParameter** ShaderParameters )
{
	ShaderDescriptor			shaderDescriptor;
	shaderDescriptor.vertexShaderSource = " \
	#version 330 core\n \
	\n \
	layout( location = 0 ) 			in vec3 vertex_position; \n \
	layout( location = 1 ) 			in vec3 vertex_normal; \n \
	layout( location = 2 ) 			in vec2 vertex_texCoords; \n \
	\n \
		out vec2 				texCoords; \n \
		out vec3				normal; \n \
	\n \
		uniform mat4    		matrix_Projection; \n \
		uniform mat4			matrix_Transformation; \n \
	\n \
	void main() \n \
	{\n \
		texCoords = vertex_texCoords; \n \
		normal = ( matrix_Transformation * vec4( vertex_normal, 0.f ) ).xyz; \n \
		gl_Position = matrix_Projection * matrix_Transformation * vec4( vertex_position, 1.f ); \n \
	}";

	shaderDescriptor.fragmentShaderSource = " \
	#version 330 core\n\
	\n\
		layout ( location = 0 ) 		out vec4 out_albedoSpecular;\n\
		layout( location = 1 ) 			out vec4 out_normalShininess;\n\
		layout( location = 2 ) 			out vec4 out_emission;\n\
		\n\
		in vec2 						texCoords;\n\
		in vec3							normal; \n\
	\n\
		uniform sampler2D				basetexture;\n\
	\n\
	void main()\n\
	{\n\
		vec4 texelColor = texture2D( basetexture, texCoords );\n\
		out_albedoSpecular = vec4( texelColor.rgb * vec3( 0.5f, 0.f, 0.f ), 0 ); \n \
		out_normalShininess = vec4( normal, 1 );\n\
		out_emission = vec4( 0, 0, 0, 1 );\n\
	}\n";

	std::vector< const char* >			defines;
	UInt32_t							flags = 0;

	if ( !LoadShader( shaderDescriptor, defines, flags ) )
		return false;

	IGPUProgram*		gpuProgram = GetGPUProgram( flags );
	if ( !gpuProgram ) return false;

	gpuProgram->Bind();
	gpuProgram->SetUniform( "basetexture", 0 );
	gpuProgram->Unbind();

	return true;
}

// ------------------------------------------------------------------------------------ //
// Подготовка к отрисовке элементов
// ------------------------------------------------------------------------------------ //
void le::TestShader::OnDrawMesh( UInt32_t CountParams, IShaderParameter** ShaderParameters, const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	IGPUProgram*		gpuProgram = GetGPUProgram( 0 );
	if ( !gpuProgram ) return;

	ShaderParameters[ 0 ]->GetValueTexture()->Bind();

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
// Конструктор
// ------------------------------------------------------------------------------------ //
le::TestShader::TestShader()
{
	shaderParams =
	{
		{ "basetexture",	SPT_TEXTURE		}
	};
}
