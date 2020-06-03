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
layout( location = 1 )  out vec4 out_normalShininess;
layout( location = 2 )  out vec4 out_emission;

in vec2 		texCoords;
in vec2 		lightmapCoords;
in vec4 		vertexColor;
in vec3			normal;

uniform vec3		color;
uniform sampler2D	lightmap;

#ifdef BASETEXTURE
	uniform sampler2D	basetexture;
#endif

void main()
{
#ifdef BASETEXTURE
    out_albedoSpecular = vec4( texture2D( basetexture, texCoords ).rgb * color.rgb, 0.f );
#else
	out_albedoSpecular = vec4( color.rgb, 0.f );
#endif

    out_normalShininess = vec4( normalize( normal ), 1 );
    out_emission = texture2D( lightmap, lightmapCoords ) * vertexColor;
}
