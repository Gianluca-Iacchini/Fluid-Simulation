#version 410

layout (location = 0) in vec4 CoordAndTextCoord;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(CoordAndTextCoord.xy, 0, 1);
	TexCoords = CoordAndTextCoord.zw;
}