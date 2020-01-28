//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/icamera.h"
#include "studiorender/igpuprogram.h"
#include "studiorender/itexture.h"

#include "global.h"
#include "unlitgeneric.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать экземпляр шейдера
// ------------------------------------------------------------------------------------ //
bool le::UnlitGeneric::InitInstance( UInt32_t CountParams, IShaderParameter** ShaderParameters )
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
		normal = vertex_normal; \n \
		gl_Position = matrix_Projection * matrix_Transformation * vec4( vertex_position, 1.f ); \n \
	}";

	shaderDescriptor.fragmentShaderSource = " \
	#version 330 core\n\
	\n\
		layout ( location = 0 ) out vec4 out_albedoSpecular;\n\
		layout( location = 1 ) out vec4 out_normalShininess;\n\
		layout( location = 2 ) out vec4 out_emission;\n\
		\n\
		in vec2 				texCoords;\n\
		in vec3					normal; \n\
	\n\
		uniform sampler2D		basetexture;\n\
	\n\
	void main()\n\
	{\n\
		out_albedoSpecular = texture2D( basetexture, texCoords ); \n \
		out_normalShininess = vec4( normal, 1 );\n\
		out_emission = vec4( 0 );\n\
	}\n";

	return LoadShader( shaderDescriptor );
}

// ------------------------------------------------------------------------------------ //
// Подготовка к отрисовке элементов
// ------------------------------------------------------------------------------------ //
void le::UnlitGeneric::OnDrawMesh( UInt32_t CountParams, IShaderParameter** ShaderParameters, const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	if ( !gpuProgram ) return;
	if ( ShaderParameters[ 0 ]->IsDefined() )
		ShaderParameters[ 0 ]->GetValueTexture()->Bind();

	gpuProgram->Bind();
	gpuProgram->SetUniform( "matrix_Transformation", Transformation );
	gpuProgram->SetUniform( "matrix_Projection", Camera->GetProjectionMatrix() * Camera->GetViewMatrix() );
}

// ------------------------------------------------------------------------------------ //
// Получить название шейдера
// ------------------------------------------------------------------------------------ //
const char* le::UnlitGeneric::GetName() const
{
	return "UnlitGeneric";
}

// ------------------------------------------------------------------------------------ //
// Получить запасной шейдер
// ------------------------------------------------------------------------------------ //
const char* le::UnlitGeneric::GetFallbackShader() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::UnlitGeneric::UnlitGeneric()
{
	shaderParams =
	{
		{ "basetexture",	SPT_TEXTURE		}
	};
}
