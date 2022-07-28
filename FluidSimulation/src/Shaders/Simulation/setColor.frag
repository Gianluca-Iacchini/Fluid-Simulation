#version 410 core


out vec4 FragColor;

uniform vec4 color;
uniform float level;

in vec3 texCoords;
in vec3 cellCoords;

void main()
{
	vec3 cellCoords = vec3(cellCoords);

	if (cellCoords.y < level)
	{
		FragColor = vec4(color.xyz, 1);
	}
}