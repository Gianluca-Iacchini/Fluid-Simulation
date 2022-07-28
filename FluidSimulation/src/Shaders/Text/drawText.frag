#version 410

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D textTexture;
uniform vec4 textColor;

void main()
{
	FragColor = vec4(textColor.rgb, texture(textTexture, TexCoords).r);

}