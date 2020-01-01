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
#include "lightmappedgeneric.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать экземпляр шейдера
// ------------------------------------------------------------------------------------ //
bool le::LightmappedGeneric::InitInstance( UInt32_t CountParams, IMaterialVar** MaterialVars )
{
	ShaderDescriptor			shaderDescriptor;
	shaderDescriptor.vertexShaderSource = " \
	#version 330 core\n \
	\n \
	layout( location = 0 ) 			in vec3 vertex_position; \n \
	layout( location = 1 ) 			in vec2 vertex_texCoords; \n \
	layout( location = 2 ) 			in vec2 vertex_lightmapCoords; \n \
	\n \
		out vec2 				texCoords; \n \
		out vec2 				lightmapCoords; \n \
		out vec4 				pcolor; \n \
	\n \
		uniform mat4    		matrix_Projection; \n \
		uniform mat4			matrix_Transformation; \n \
	\n \
	void main() \n \
	{\n \
		texCoords = vertex_texCoords; \n \
		lightmapCoords = vertex_lightmapCoords; \n \
		gl_Position = matrix_Projection * matrix_Transformation * vec4( vertex_position, 1.f ); \n \
	}";

	shaderDescriptor.fragmentShaderSource = " \
	#version 330 core\n\
	\n\
		in vec2 				texCoords;\n\
		in vec2 				lightmapCoords;\n\
		out vec4				color;\n\
	\n\
		uniform sampler2D		basetexture;\n\
		uniform sampler2D		lightmap;\n\
	\n\
	void main()\n\
	{\n\
		color = texture2D( basetexture, texCoords ) * texture2D( lightmap, lightmapCoords );\n\
	}\n";

	if ( !LoadShader( shaderDescriptor ) )
		return false;

	gpuProgram->Bind();
	gpuProgram->SetUniform( "basetexture", 0 );
	gpuProgram->SetUniform( "lightmap", 1 );
	gpuProgram->Unbind();

	return true;
}

// ------------------------------------------------------------------------------------ //
// Подготовка к отрисовке элементов
// ------------------------------------------------------------------------------------ //
void le::LightmappedGeneric::OnDrawMesh( UInt32_t CountParams, IMaterialVar** MaterialVars, const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	if ( !gpuProgram ) return;
	if ( MaterialVars[ 0 ]->IsDefined() )
		MaterialVars[ 0 ]->GetValueTexture()->Bind( 0 );

	if ( Lightmap )
		Lightmap->Bind( 1 );

	gpuProgram->Bind();
	gpuProgram->SetUniform( "matrix_Transformation", Transformation );
	gpuProgram->SetUniform( "matrix_Projection", Camera->GetProjectionMatrix() * Camera->GetViewMatrix() );
}

// ------------------------------------------------------------------------------------ //
// Получить название шейдера
// ------------------------------------------------------------------------------------ //
const char* le::LightmappedGeneric::GetName() const
{
	return "LightmappedGeneric";
}

// ------------------------------------------------------------------------------------ //
// Получить запасной шейдер
// ------------------------------------------------------------------------------------ //
const char* le::LightmappedGeneric::GetFallbackShader() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::LightmappedGeneric::LightmappedGeneric()
{
	shaderParams =
	{
		{ "basetexture",	MVT_TEXTURE		}
	};
}
