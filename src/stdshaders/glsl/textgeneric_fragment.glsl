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

layout ( location = 0 ) out vec4    out_albedoSpecular;
layout( location = 2 ) out vec4     out_emission;

in vec2 		texCoords;

uniform vec3            color;
uniform sampler2D	basetexture;

void main()
{
    float	alphaGlyph = texture2D( basetexture, texCoords ).r;
    if ( alphaGlyph < 0.001f )      discard;

    out_albedoSpecular = vec4( alphaGlyph * color, 0.f );
    out_emission = vec4( 1.f );
}
