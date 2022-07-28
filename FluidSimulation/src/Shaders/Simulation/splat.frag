#version 410 core

out vec4 FragColor;

in vec3 texCoords;
in vec3 cellCoords;



uniform vec3 color;
uniform vec3 position;
uniform float radius;
uniform sampler3D obstacle;
uniform sampler3D base;


#define OBSTACLE_BOUNDARY  128.0f/255.0f
bool IsNonEmptyCell(ivec3 texCoords)
{
    return texelFetch(obstacle, texCoords, 0).x > 0;
}

float gaussian(vec3 cellCoords, vec3 pos, float radius)
{
    float dist = length(cellCoords - pos) * radius;
	return exp(-dist * dist);
}


void main()
{

    ivec3 T = ivec3(cellCoords);

    // If there is an obstacle in this cell then kill the fragment early
    if (IsNonEmptyCell(T))
    {
        FragColor = vec4(0);
        return;
    }

    vec4 bColor = texelFetch(base, T, 0);

    // Creates a gaussian splat
    float d = distance(position, T); 
    if (d < radius) { //splat in radius
        float a = (radius - d) * 0.5;
        a = min(a, 1.0);
        FragColor = vec4(color, a) * a + bColor * (1 - a);
    } else {
        FragColor = bColor;
    }

}