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

layout ( location = 0 ) out vec4 out_albedoSpecular;
layout( location = 1 ) out vec4 out_normalShininess;
layout( location = 2 ) out vec4 out_emission;

in vec2 		texCoords;
in vec3 		normal;

#ifdef NORMAL_MAP
in vec3			fragPos;
#endif

uniform vec3			color;

#ifdef BASETEXTURE
	uniform sampler2D	basetexture;
#endif

#ifdef NORMAL_MAP
	uniform sampler2D	normalmap;
#endif

#ifdef SPECULAR_MAP
	uniform sampler2D	specularmap;
#endif

#ifdef NORMAL_MAP
	vec3 perturbNormal()
	{
		vec3    tangentNormal = texture( normalmap, texCoords ).xyz * 2.0 - 1.0;

		vec3    q1 = dFdx( fragPos );
		vec3    q2 = dFdy( fragPos );
		vec2    st1 = dFdx( texCoords );
		vec2    st2 = dFdy( texCoords );

		vec3    N = normalize( normal );
		vec3    T = normalize( q1 * st2.t - q2 * st1.t );
		vec3    B = -normalize( cross( N, T ) );
		mat3    TBN = mat3( T, B, N );

		return normalize( TBN * tangentNormal );
	}
#endif

void main()
{
#ifdef BASETEXTURE
    vec4            colorPixel = texture2D( basetexture, texCoords );
    if ( colorPixel.a < 0.01f )      discard;
#else
	vec4            colorPixel = vec4( color, 1.f );
#endif

#ifdef SPECULAR_MAP
    out_albedoSpecular = vec4( colorPixel.rgb, texture2D( specularmap, texCoords ).r );
#else
    out_albedoSpecular = vec4( colorPixel.rgb, 0.f );
#endif

#ifdef NORMAL_MAP
    out_normalShininess = vec4( perturbNormal(), 32.f );
#else
    out_normalShininess = vec4( normal, 32.f );
#endif

    out_emission = vec4( 0.f );
}
