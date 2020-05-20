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

struct Primitive
{
	vec3		verteces[ 2 ];
	vec3		color;
};

out vec3				colorPrimitive;
uniform mat4			matrixProjection;
uniform Primitive		primitives[ 255 ];

void main()
{
	colorPrimitive = primitives[ gl_InstanceID ].color;
	gl_Position = matrixProjection * vec4( primitives[ gl_InstanceID ].verteces[ gl_VertexID ], 1.f );
}