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
uniform sampler2D		emission;
uniform sampler2D		finalFrame;

void main()
{
	vec2	fragCoord = gl_FragCoord.xy / screenSize;

	vec3	albedoColor = texture( albedoSpecular, fragCoord ).rgb;
    vec4	emissonColor = texture( emission, fragCoord );
    vec4	lightColor = texture( finalFrame, fragCoord );

    color = vec4( albedoColor, 1.f ) * ( emissonColor + lightColor );
}