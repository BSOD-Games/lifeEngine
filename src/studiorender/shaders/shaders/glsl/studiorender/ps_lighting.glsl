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

out vec4				color;

uniform vec2			screenSize;
uniform sampler2D		albedoSpecular;
uniform sampler2D		normalShininess;
uniform sampler2D		emission;
uniform sampler2D		depth;

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

vec3 ReconstructPosition( vec2 FragCoord ) 
{ 
	float 		depth = texture( depth, FragCoord ).r;
    
	vec4 		position = camera.invViewMatrix * camera.invProjectionMatrix * vec4( FragCoord * 2.f - 1.f, depth * 2.f - 1.f, 1.f );
    position /= position.w;
    return position.xyz;
}	

void main()
{
    vec2	fragCoord = gl_FragCoord.xy / screenSize;
    
    vec4	fragColor = texture( albedoSpecular, fragCoord ); 
    vec4	normal = texture( normalShininess, fragCoord ); 
    vec3	posFrag = ReconstructPosition( fragCoord ); 
    vec3	viewDirection = normalize( camera.position - posFrag ); 

#if defined( POINT_LIGHT ) || defined( SPOT_LIGHT )
	vec3	lightDirection = light.position - posFrag; 
	float	distance = length( lightDirection ); 
	lightDirection = normalize( lightDirection ); 
#elif defined( DIRECTIONAL_LIGHT ) 
	vec3	lightDirection = normalize( light.direction );
#endif 

	vec3	halfwayDirection = normalize( lightDirection + viewDirection ); 		
	float 	NdotL = max( dot( normal.xyz, lightDirection ), 0.f ); 
		
#ifdef POINT_LIGHT 
	float 	attenuation = pow( clamp( 1.f - pow( distance / light.radius, 4.f ), 0.f, 1.f ), 2.f ) / ( pow( distance, 2.f ) + 1.f );
#elif defined( SPOT_LIGHT ) 
	float 	attenuation = pow( clamp( 1.f - pow( distance / light.height, 4.f ), 0.f, 1.f ), 2.f ) / ( pow( distance, 2.f ) + 1.f );
#endif 

	float	specularFactor = max( pow( dot( reflect( -lightDirection, normal.xyz ), viewDirection ), normal.a ) * fragColor.a, 0.f );
		
#ifdef SPOT_LIGHT 
	float 		spotFactor = dot( -lightDirection, normalize( light.direction ) );
	spotFactor = clamp( ( spotFactor - light.cutoff ) / ( 0.95f - light.cutoff ), 0.0f, 1.0f ); 
#endif 
		
#ifdef POINT_LIGHT 
	color = ( light.color * light.intensivity + light.color * specularFactor * light.intensivity ) * attenuation * NdotL ; 
#elif defined( SPOT_LIGHT ) 
	color = ( light.color * light.intensivity + light.color * specularFactor * light.intensivity ) * attenuation * spotFactor * NdotL ; 
#elif defined( DIRECTIONAL_LIGHT ) 
	color = ( light.color * light.intensivity + light.color * specularFactor ) * NdotL; 
#endif 
}