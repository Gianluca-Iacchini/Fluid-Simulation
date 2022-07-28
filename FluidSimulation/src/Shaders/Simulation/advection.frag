#version 410 core

//in vec3 texCoords;
//in vec3 cellCoords;
in vec3 texCoords;
in vec3 cellCoords;

out vec4 FragColor;


uniform sampler3D obstacle;
uniform sampler3D velocity;
uniform sampler3D quantity;


uniform float timeStep;
uniform vec3 gridDimension;
uniform float dissipation;

#define OBSTACLE_BOUNDARY  128.0f/255.0f
bool IsNonEmptyCell(ivec3 coords)
{
    return texelFetch(obstacle, coords, 0).x > 0;
}


// Trilinear interpolation
vec4 trilerp(sampler3D sam, vec3 pos, vec3 inverseGridSize)
{    
    vec3 uv = (floor(pos - 0.5f) + 0.5f) * inverseGridSize;

    vec4 lA = texture(sam, uv + vec3(0, 0, 0) * inverseGridSize);
    vec4 lB = texture(sam, uv + vec3(1, 0, 0) * inverseGridSize);
    vec4 lC = texture(sam, uv + vec3(0, 1, 0) * inverseGridSize);
    vec4 lD = texture(sam, uv + vec3(1, 1, 0) * inverseGridSize);

    vec4 lE = texture(sam, uv + vec3(0, 0, 1) * inverseGridSize);
    vec4 lF = texture(sam, uv + vec3(1, 0, 1) * inverseGridSize);
    vec4 lG = texture(sam, uv + vec3(0, 1, 1) * inverseGridSize);
    vec4 lH = texture(sam, uv + vec3(1, 1, 1) * inverseGridSize);

    vec3 a = pos - (floor(pos - 0.5f) + 0.5f);
   

    vec4 interp1 = mix(mix(lA, lB, a.x), mix(lC, lD, a.x), a.y);
    vec4 interp2 = mix(mix(lE, lF, a.x), mix(lG, lH, a.x), a.y);
    return mix(interp1, interp2, a.z);
}


void main()
{    
    // If there is an obstacle in the current cell then kill the fragment shader early.
    if (IsNonEmptyCell(ivec3(cellCoords)))
    {
        FragColor = vec4(0);
        return;
    }


    vec3 velocity = texture(velocity, texCoords).xyz;

    vec3 coord =  cellCoords - timeStep * velocity;

    FragColor = trilerp(quantity, coord, (1.f / gridDimension)) * dissipation;
}

