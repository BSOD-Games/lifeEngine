//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>

#include "common/shaderdescriptor.h"
#include "studiorender/gpuprogram.h"
#include "global.h"

#include "shader_postprocess.h"

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ShaderPostprocess::ShaderPostprocess() :
	gpuProgram( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ShaderPostprocess::~ShaderPostprocess()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Создать шейдер
// ------------------------------------------------------------------------------------ //
bool le::ShaderPostprocess::Create()
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
		uniform sampler2D		finalFrame;\n\
	\n \
	void main()\n\
	{\n\
		vec2	fragCoord = gl_FragCoord.xy / screenSize;\n\
		\n\
		vec3	albedoColor = texture( albedoSpecular, fragCoord ).rgb; \n\
        vec4	emissonColor = texture( emission, fragCoord ); \n\
        vec4	lightColor = texture( finalFrame, fragCoord ); \n\
        \n\
        color = emissonColor * vec4( albedoColor, 1.f ) + lightColor;\n\
	}\n";

	// Компилируем шейдер для точечного освещения

	gpuProgram = new GPUProgram();
	if ( !gpuProgram->Compile( shaderDescriptor ) )
		return false;

	gpuProgram->Bind();
	gpuProgram->SetUniform( "albedoSpecular", 0 );
	gpuProgram->SetUniform( "emission", 1 );
	gpuProgram->SetUniform( "finalFrame", 2 );
	gpuProgram->Unbind();
	
	return true;
}

// ------------------------------------------------------------------------------------ //
// Удалить шейдер
// ------------------------------------------------------------------------------------ //
void le::ShaderPostprocess::Delete()
{
    Unbind();

    delete gpuProgram;
    gpuProgram = nullptr;
}
