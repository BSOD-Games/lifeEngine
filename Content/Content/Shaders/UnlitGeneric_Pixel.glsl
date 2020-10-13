#version 330 core

in vec2					texCoord;
out vec4				color;

uniform sampler2D		baseTexture;

void main()
{
	vec4	texel = texture2D( baseTexture, texCoord );
	if ( texel.a < 0.001 )		discard;
	color = texel;
}