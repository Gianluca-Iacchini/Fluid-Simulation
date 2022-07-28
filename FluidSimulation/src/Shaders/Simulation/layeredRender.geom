#version 410 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 TexCoords[];
in vec2 CellCoords[];
in int vInstance[];

out vec3 texCoords;
out vec3 cellCoords;
out float gLayer;

uniform vec3 gridDimension;

// For each instance emit a quad identical to the original with a different gLayer value which is used to compute the z value for both texture and grid coordinates.
void main()
{   
	int layer = vInstance[0];
	for (unsigned int i = 0; i < gl_in.length(); i++)
	{

		gl_Layer = layer;
		gLayer = float(gl_Layer) + 0.5f;
		cellCoords = vec3(CellCoords[i], gLayer);
		texCoords = vec3(TexCoords[i], gLayer/gridDimension.z);
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}