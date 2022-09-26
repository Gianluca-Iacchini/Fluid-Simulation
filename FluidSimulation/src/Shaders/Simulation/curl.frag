#version 410 core


out vec3 FragColor;

in vec3 texCoords;
in vec3 cellCoords;


// Velocity
uniform sampler3D w;

// Computes velocity curl using the discrete formula
void main()
{

	ivec3 coord = ivec3(cellCoords);
	
	float halfRdx = 0.5f;

	vec4 wLeft = texelFetchOffset(w, coord, 0, ivec3(-1, 0, 0));
	vec4 wRight = texelFetchOffset(w, coord, 0, ivec3(1, 0, 0));
	vec4 wBottom = texelFetchOffset(w, coord, 0, ivec3(0, -1, 0));
	vec4 wTop = texelFetchOffset(w, coord, 0, ivec3(0, 1, 0));
	vec4 wBack = texelFetchOffset(w, coord, 0, ivec3(0, 0, -1));
	vec4 wFront = texelFetchOffset(w, coord, 0, ivec3(0, 0, 1));
	
	float xFactor = (wTop.z - wBottom.z) - (wFront.y - wBack.y);
	float yFactor = (wFront.x - wBack.x) - (wRight.z - wLeft.z) ;
	float zFactor = (wRight.y - wLeft.y) - (wTop.x - wBottom.x);


	FragColor = halfRdx * vec3(xFactor, yFactor, zFactor);
}