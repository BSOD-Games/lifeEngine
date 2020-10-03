#version 330 core

layout( location = 0 ) 		in vec3 vertex_position;
layout( location = 1 ) 		in vec3 vertex_normal;
layout( location = 2 ) 		in vec2 vertex_texCoords;

out vec2		texCoord;

uniform vec2	spriteSize;
uniform vec3	position;
uniform mat4	PVTMatrix;

void main()
{
	texCoord = vertex_texCoords;
	gl_Position = PVTMatrix * vec4( position + vertex_position * vec3( spriteSize, 1.f ), 1.f );
}