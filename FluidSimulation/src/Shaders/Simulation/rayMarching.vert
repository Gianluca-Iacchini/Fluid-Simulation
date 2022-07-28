#version 410 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCords;
layout (location = 2) in vec2 aCellCords;

uniform mat4 invModelViewProj;
uniform float zNear;

out RayCastData {
	vec4 pos;
	vec3 posInGrid;
} rayCastData;

void main()
{
	// Basic position for a full screen quad
	rayCastData.pos = vec4(aPos, 0.0f, 1.f);
	// Converts the quad position to the box object space
	rayCastData.posInGrid = (invModelViewProj * vec4(aPos.xy * zNear, 0, zNear)).xyz;

	gl_Position = rayCastData.pos;
}