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

uniform sampler2D		basetexture;
#ifdef NORMAL_MAP
    uniform sampler2D		normalmap;
#endif
#ifdef SPECULAR_MAP
    uniform sampler2D		specularmap;
#endif

void main()
{
#ifdef SPECULAR_MAP
    out_albedoSpecular = vec4( texture2D( basetexture, texCoords ).rgb, texture2D( specularmap, texCoords ).r );
#else
    out_albedoSpecular = vec4( texture2D( basetexture, texCoords ).rgb, 0.f );
#endif

#ifdef NORMAL_MAP
    vec3 normal = texture2D( normalmap, texCoords ).rgb * 2.0 - 1.0;
    out_normalShininess = vec4( normalize( tbnMatrix * normal ), 32.f );
#else
    out_normalShininess = vec4( normalize( normal ), 32.f );
#endif

    out_emission = vec4( 0.f );
}
