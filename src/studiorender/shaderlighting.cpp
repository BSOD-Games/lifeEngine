//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "common/shaderdescriptor.h"
#include "studiorender/gpuprogram.h"

#include "shaderlighting.h"

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ShaderLighting::ShaderLighting() :
	gpuProgram( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ShaderLighting::~ShaderLighting()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Создать экземпляр шейдера
// ------------------------------------------------------------------------------------ //
bool le::ShaderLighting::Create()
{
	ShaderDescriptor		shaderDescriptor = {};
	shaderDescriptor.vertexShaderSource = " \
	#version 330 core\n \
	\n \
	layout( location = 0 ) 			in vec3 vertex_position; \n \
	\n \
	void main() \n \
	{\n \
		gl_Position = vec4( vertex_position, 1.f ); \n \
	}";

	shaderDescriptor.fragmentShaderSource = "\
	#version 330 core\n\
	\n\
		out vec4				color;\n\
	\n\
		uniform vec2			screenSize;\n\
		uniform sampler2D		albedoSpecular;\n\
		uniform sampler2D		emission;\n\
	\n \
	void main()\n\
	{\n\
		vec2	texCoord = gl_FragCoord.xy / screenSize;\n\
		color = texture( albedoSpecular, texCoord ) * texture( emission, texCoord );\n\
	}\n";

	gpuProgram = new GPUProgram();
	if ( !gpuProgram->Compile( shaderDescriptor ) )
		return false;

	gpuProgram->Bind();
	gpuProgram->SetUniform( "albedoSpecular", 0 );
	gpuProgram->SetUniform( "emission", 2 );
	gpuProgram->Unbind();

	return true;
}

// ------------------------------------------------------------------------------------ //
// Удалить экземпляр шейдера
// ------------------------------------------------------------------------------------ //
void le::ShaderLighting::Delete()
{
	if ( gpuProgram )		delete gpuProgram;
}
