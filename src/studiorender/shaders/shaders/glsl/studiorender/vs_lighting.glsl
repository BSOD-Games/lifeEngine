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

layout( location = 0 ) 			in vec3 vertex_position;\

uniform struct Camera
{
    mat4		pvMatrix;
    mat4		invProjectionMatrix;
    mat4		invViewMatrix;
    vec3		position;
} camera;

uniform struct Light
{
    vec4		color;
    vec4		specular;
    float		intensivity;
	
#ifdef POINT_LIGHT
    vec3		position;
    float 		radius;
#elif defined( SPOT_LIGHT )
    vec3		position;
    float		radius;
    float		height;
    float		cutoff;
    vec3		direction;
#elif defined( DIRECTIONAL_LIGHT )
    vec3		direction;
#endif
} light;

uniform mat4 		pvtMatrix;

void main()
{
	#ifdef POINT_LIGHT
		gl_Position = pvtMatrix * vec4( vertex_position * light.radius, 1.f );
	#elif defined( SPOT_LIGHT )
		gl_Position = pvtMatrix * vec4( vertex_position.x * light.radius, vertex_position.y * light.height, vertex_position.z * light.radius, 1.f );
	#elif defined( DIRECTIONAL_LIGHT )
		gl_Position = vec4( vertex_position, 1.f );
	#endif
}