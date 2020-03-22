//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include "common/shaderdescriptor.h"
#include "engine/iconsolesystem.h"

#include "global.h"
#include "gpuprogram.h"
#include "shader_depth.h"

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ShaderDepth::ShaderDepth() :
    gpuProgram( nullptr ),
    activeType( GT_SPHERE )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ShaderDepth::~ShaderDepth()
{
    Delete();
}

// ------------------------------------------------------------------------------------ //
// Создать шейдер
// ------------------------------------------------------------------------------------ //
bool le::ShaderDepth::Create()
{
    ShaderDescriptor		shaderDescriptor = {};
    shaderDescriptor.vertexShaderSource = "\
        #version 330 core\n\
                                          \
                                          layout ( location = 0 )         in vec3 vertex_position;\n\
                                          \
                                          uniform mat4 pvtMatrix;\n\
                                          \n\
                                      #if defined( SPHERE ) \n\
                                          uniform float       radius;\n\
                                      #elif defined( CONE ) \n\
                                          uniform float       radius;\n\
                                          uniform float       height;\n\
                                      #endif \n\
                                          \
                                          void main()\n\
    {\n\
                                      #if defined( SPHERE ) \n\
                                          gl_Position = pvtMatrix * vec4( vertex_position * radius, 1.f ); \n\
                                      #elif defined( CONE ) \n\
                                          gl_Position = pvtMatrix * vec4( vertex_position.x * radius, vertex_position.y * height, vertex_position.z * radius, 1.f ); \n \
                                      #else \n\
                                          gl_Position = pvtMatrix * vec4( vertex_position, 1.f ); \n \
                                      #endif \n\
}";

                                          shaderDescriptor.fragmentShaderSource = "\
                                      #version 330 core\n\
                                          \
                                          out vec4 color;\n\
                                          \
                                          void main()\n\
    {\n\
                                          color = vec4( 1.f, 0.f, 0.f, 1.f );\n\
}";

                                          // Компилируем шейдер записи в глубину для геометрии "Сфера"
            std::vector< const char* >		defines = { "SPHERE" };
    GPUProgram*         gpuProgram_sphere = new GPUProgram();
    gpuProgram_sphere->IncrementReference();

    if ( !gpuProgram_sphere->Compile( shaderDescriptor, defines.size(), defines.data() ) )
    {
        delete gpuProgram_sphere;
        return false;
    }

    // Компилируем шейдер записи в глубину для геометрии "Конус"

    defines = { "CONE" };
    GPUProgram*         gpuProgram_cone = new GPUProgram();
    gpuProgram_cone->IncrementReference();

    if ( !gpuProgram_cone->Compile( shaderDescriptor, defines.size(), defines.data() ) )
    {
        delete gpuProgram_sphere;
        delete gpuProgram_cone;
        return false;
    }

    // Компилируем шейдер записи в глубину для геометрии "Неизвестная геометрия"

    GPUProgram*         gpuProgram_unknown = new GPUProgram();
    gpuProgram_unknown->IncrementReference();

    if ( !gpuProgram_unknown->Compile( shaderDescriptor ) )
    {
        delete gpuProgram_sphere;
        delete gpuProgram_cone;
        delete gpuProgram_unknown;
        return false;
    }

    gpuProgram = gpuProgram_sphere;
    activeType = GT_SPHERE;

    gpuPrograms[ GT_SPHERE ] = gpuProgram_sphere;
    gpuPrograms[ GT_CONE ] = gpuProgram_cone;
    gpuPrograms[ GT_UNKNOWN ] = gpuProgram_unknown;

    return true;
}

// ------------------------------------------------------------------------------------ //
// Удалить шейдер
// ------------------------------------------------------------------------------------ //
void le::ShaderDepth::Delete()
{
    Unbind();

    for ( auto it = gpuPrograms.begin(), itEnd = gpuPrograms.end(); it != itEnd; ++it )
        if ( it->second->GetCountReferences() <= 1 )
            it->second->Release();
        else
            it->second->DecrementReference();

    activeType = GT_SPHERE;
    gpuProgram = nullptr;
    gpuPrograms.clear();
}
