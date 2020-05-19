//////////////////////////////////////////////////////////////////////////
//
//          *** lifeEngine (Двигатель жизни) ***
//		Copyright (C) 2018-2020
//
// Репозиторий движка:      https://github.com/zombihello/lifeEngine
// Авторы:                  Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#version 330 core

layout( location = 0 ) 		in vec3 vertex_position;
layout( location = 1 ) 		in vec3 vertex_normal;
layout( location = 2 ) 		in vec2 vertex_texCoords;

#ifdef NORMAL_MAP
layout( location = 3 )          in vec3 vertex_tangent;
layout( location = 4 )          in vec3 vertex_bitangent;
#endif

out vec2 		texCoords;

#ifdef NORMAL_MAP
out mat3		tbnMatrix;
#else
out vec3		normal;
#endif

uniform mat4    	matrix_Projection;
uniform mat4		matrix_Transformation;

void main()
{
    texCoords = vertex_texCoords;

#ifdef NORMAL_MAP
    vec3    normal = vec3( matrix_Transformation * vec4( vertex_normal, 0.f ) );
    vec3    tangent = vec3( matrix_Transformation * vec4( vertex_tangent, 0.f ) );
    vec3    bitangent = vec3( matrix_Transformation * vec4( vertex_bitangent, 0.f ) );
    tbnMatrix = mat3( tangent, bitangent, normal );
#else
    normal = vec3( matrix_Transformation * vec4( vertex_normal, 0.f ) );
#endif

    gl_Position = matrix_Projection * matrix_Transformation * vec4( vertex_position, 1.f );
}
