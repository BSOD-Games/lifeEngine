#version 330 core

in vec2					texCoord;
out vec4				color;

uniform sampler2D		baseTexture;

void main()
{
	color = texture2D( baseTexture, texCoord );
}