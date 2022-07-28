#version 410 core


out vec4 FragColor;

in vec3 texCoords;
in vec3 cellCoords;


uniform float value;
uniform sampler3D base;


void main()
{
	// Resets a texture like a glClear(GL_COLOR_BUFFER_BIT) call would.
	ivec3 T = ivec3(cellCoords);

	FragColor = value * texelFetch(base, T, 0);
}