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

layout( location = 0 ) 	in vec3     vertex_position;
layout( location = 1 ) 	in vec2     vertex_texCoords;
layout( location = 2 ) 	in vec2     vertex_lightmapCoords;
layout( location = 3 ) 	in vec3     vertex_normal;
layout( location = 4 ) 	in vec4     vertex_color;

out vec2 	texCoords;
out vec2 	lightmapCoords;
out vec4 	vertexColor;
out vec3	normal;

uniform mat4    matrix_Projection;
uniform mat4	matrix_Transformation;

void main()
{
    texCoords = vertex_texCoords;
    lightmapCoords = vertex_lightmapCoords;
    vertexColor = vertex_color;
    normal = vec3( matrix_Transformation * vec4( vertex_normal, 0.f ) );
    gl_Position = matrix_Projection * matrix_Transformation * vec4( vertex_position, 1.f );
}
