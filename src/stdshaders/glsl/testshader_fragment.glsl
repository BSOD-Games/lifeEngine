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

layout( location = 0 ) 	out vec4        out_albedoSpecular;
layout( location = 1 ) 	out vec4        out_normalShininess;
layout( location = 2 ) 	out vec4        out_emission;

in vec2                 texCoords;
in vec3                 normal;

uniform sampler2D	basetexture;

void main()
{
    vec4        texelColor = texture2D( basetexture, texCoords );

    out_albedoSpecular      = vec4( texelColor.rgb * vec3( 0.5f, 0.f, 0.f ), 0.f );
    out_normalShininess     = vec4( normal, 1.f );
    out_emission            = vec4( 0.f, 0.f, 0.f, 1.f );
}
