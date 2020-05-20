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

layout ( location = 0 )         in vec3 vertex_position;

uniform mat4 pvtMatrix;

#if defined( SPHERE )
    uniform float       radius;
#elif defined( CONE )
    uniform float       radius;
    uniform float       height;
#endif

void main()
{
	#if defined( SPHERE )
		gl_Position = pvtMatrix * vec4( vertex_position * radius, 1.f );
	#elif defined( CONE )
		gl_Position = pvtMatrix * vec4( vertex_position.x * radius, vertex_position.y * height, vertex_position.z * radius, 1.f );
	#else
		gl_Position = pvtMatrix * vec4( vertex_position, 1.f );
	#endif
}