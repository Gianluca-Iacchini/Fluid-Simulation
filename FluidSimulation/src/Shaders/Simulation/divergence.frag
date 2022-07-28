#version 410 core


in vec3 texCoords;
in vec3 cellCoords;


out float FragColor;

uniform sampler3D obstacle;
uniform sampler3D velocity;

uniform vec3 gridDimension;

#define OBSTACLE_BOUNDARY  128.0f/255.0f
bool IsBoundaryCell(ivec3 T, ivec3 offset)
{
    return (texelFetchOffset(obstacle, T, 0, offset).x == OBSTACLE_BOUNDARY);
}

// Computes velocity divergence
void main()
{
    
    ivec3 T = ivec3(cellCoords);

    vec3 vLeft =   texelFetchOffset(velocity, T, 0, ivec3(-1, 0, 0)).xyz; 
    vec3 vRight =  texelFetchOffset(velocity, T, 0, ivec3(1, 0, 0)).xyz; 
    vec3 vBottom = texelFetchOffset(velocity, T, 0, ivec3(0, -1, 0)).xyz; 
    vec3 vTop =    texelFetchOffset(velocity, T, 0, ivec3(0, 1, 0)).xyz; 
    vec3 vBack =   texelFetchOffset(velocity, T, 0, ivec3(0, 0, -1)).xyz; 
    vec3 vFront =  texelFetchOffset(velocity, T, 0, ivec3(0, 0, 1)).xyz; 


    // If any of the neighbour contains an obstacle then treat that cell as having velocity 0.
    if (IsBoundaryCell(T, ivec3(-1, 0, 0))) vLeft = vec3(0);
    if (IsBoundaryCell(T, ivec3(1, 0, 0))) vRight = vec3(0);
    if (IsBoundaryCell(T, ivec3(0, -1, 0))) vBottom = vec3(0);
    if (IsBoundaryCell(T, ivec3(0, 1, 0))) vTop = vec3(0);
    if (IsBoundaryCell(T, ivec3(0, 0, -1))) vBack = vec3(0);
    if (IsBoundaryCell(T, ivec3(0, 0, 1))) vFront = vec3(0);

    float divergence = 0.5f * ((vRight.x - vLeft.x) + (vTop.y - vBottom.y) + (vFront.z - vBack.z));
    FragColor = divergence;

    
}