#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 modelViewProj;
uniform vec3 gridDimension;


out RayData
{
	vec4 pos;
	vec3 posInGrid;
	float depth;
} rayData;


// Basic vertex shader to show a box in a 3D space.
void main()
{
	rayData.pos = modelViewProj * vec4(aPos, 1);
	rayData.posInGrid = aPos;
	rayData.depth = rayData.pos.z;

	gl_Position = rayData.pos;
}