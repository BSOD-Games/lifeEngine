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

layout ( location = 0 ) out vec4        out_albedoSpecular;
layout( location = 1 )  out vec4        out_normalShininess;
layout( location = 2 )  out vec4        out_emission;

in vec2 				texCoords;

#ifdef NORMAL_MAP
    in mat3				tbnMatrix;
#else
    in vec3				normal;
#endif


uniform vec3			color;

#ifdef BASETEXTURE
	uniform sampler2D		basetexture;
#endif

#ifdef NORMAL_MAP
    uniform sampler2D		normalmap;
#endif
#ifdef SPECULAR_MAP
    uniform sampler2D		specularmap;
#endif
#ifdef ALPHA_MAP
    uniform sampler2D		alphamap;
#endif

void main()
{
#ifdef ALPHA_MAP
	float		alpha = texture2D( alphamap, texCoords ).r;
	if ( alpha < 0.001f ) 		discard;
#endif

#ifdef BASETEXTURE
	vec3		fragColor = texture2D( basetexture, texCoords ).rgb * color.rgb;
#else
	vec3		fragColor = color.rgb;
#endif

#ifdef SPECULAR_MAP
    out_albedoSpecular = vec4( fragColor, texture2D( specularmap, texCoords ).r );
#else
    out_albedoSpecular = vec4( fragColor, 0.f );
#endif

#ifdef NORMAL_MAP
    vec3 normal = texture2D( normalmap, texCoords ).rgb * 2.0 - 1.0;
    out_normalShininess = vec4( normalize( tbnMatrix * normal ), 32.f );
#else
    out_normalShininess = vec4( normalize( normal ), 32.f );
#endif

    out_emission = vec4( 0.f );
}
