#version 410 core

out vec3 FragColor;

in vec3 texCoords;
in vec3 cellCoords;


uniform vec3 gridDimension;

uniform float vorticity;
uniform float timeStep;

uniform sampler3D obstacle;
uniform sampler3D curl;
uniform sampler3D w;

#define OBSTACLE_BOUNDARY  128.0f/255.0f
bool IsNonEmptyCell(vec3 texCoords)
{
    return texture(obstacle, texCoords).x > 0;
}

void main()
{
	ivec3 fragCoords = ivec3(cellCoords);

	vec3 tCoords = fragCoords / gridDimension;

	// If there is an obstacle in this cell then kill the fragment early;
	if (IsNonEmptyCell(tCoords))
	{
		FragColor = vec3(0);
		return;
	}


	float halfRdx = 0.5f;

	float vLeft = length(texelFetch(curl, fragCoords, 0));
	float vRight = length(texelFetchOffset(curl, fragCoords, 0, ivec3(1,0,0)));
	float vBottom = length(texelFetchOffset(curl, fragCoords, 0, ivec3(0,-1, 0)));
	float vTop = length(texelFetchOffset(curl, fragCoords, 0, ivec3(0,1,0)));
	float vBack = length(texelFetchOffset(curl, fragCoords, 0, ivec3(0,0,-1)));
	float vFront = length(texelFetchOffset(curl, fragCoords, 0, ivec3(0,0,1)));

	vec4 vC = texelFetch(curl, fragCoords, 0);

	vec3 eta = halfRdx * vec3(vRight - vLeft, vTop - vBottom, vFront - vBack);

	eta = normalize(eta + vec3(0.001));

	vec3 force;
	force.xyz = timeStep * vorticity * vec3(eta.y * vC.z - eta.z * vC.y, eta.z * vC.x - eta.x * vC.z, eta.x * vC.y - eta.y * vC.x);

	vec3 oldVel = texelFetch(w, fragCoords, 0).xyz;
	force.xyz = force.xyz + oldVel;

	FragColor = force;
}