#version 410

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 viewProjection;

void main()
{
	TexCoords = aPos;
	vec4 pos = viewProjection * vec4(aPos, 1);

	gl_Position = pos.xyww;
}