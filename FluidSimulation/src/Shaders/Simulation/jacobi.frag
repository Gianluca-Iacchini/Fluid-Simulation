#version 410 core

out float FragColor;

in vec3 texCoords;
in vec3 cellCoords;

uniform sampler3D obstacle;
uniform sampler3D divergence;
uniform sampler3D pressure;

uniform vec3 gridDimension;

#define OBSTACLE_BOUNDARY  128.0f/255.0f
bool IsBoundaryCell(ivec3 T, ivec3 offset)
{
    return (texelFetchOffset(obstacle, T, 0, offset).x == OBSTACLE_BOUNDARY);
}


void main()
{


    ivec3 T = ivec3(cellCoords);

    // If there is an obstacle in the current cell then kill the fragment shader early.
    if (IsBoundaryCell(T, ivec3(0)))
    {
        FragColor = 0;
        return;
    }


    float pLeft =   texelFetchOffset(pressure, T, 0, ivec3(-1, 0, 0)).x; 
    float pRight =  texelFetchOffset(pressure, T, 0, ivec3(1, 0, 0)).x; 
    float pBottom = texelFetchOffset(pressure, T, 0, ivec3(0, -1, 0)).x; 
    float pTop =    texelFetchOffset(pressure, T, 0, ivec3(0, 1, 0)).x; 
    float pBack =   texelFetchOffset(pressure, T, 0, ivec3(0, 0, -1)).x; 
    float pFront =  texelFetchOffset(pressure, T, 0, ivec3(0, 0, 1)).x; 
    float pCenter = texelFetch(pressure, T, 0).x;


    // If any of the neighbour cell contains an obstacle then use this cell to compute the pressure instead.
    if (IsBoundaryCell(T, ivec3(-1, 0, 0))) pLeft = pCenter;
    if (IsBoundaryCell(T, ivec3(1, 0, 0))) pRight = pCenter;
    if (IsBoundaryCell(T, ivec3(0, -1, 0))) pBottom = pCenter;
    if (IsBoundaryCell(T, ivec3(0, 1, 0))) pTop = pCenter;
    if (IsBoundaryCell(T, ivec3(0, 0, -1))) pBack = pCenter;
    if (IsBoundaryCell(T, ivec3(0, 0, 1))) pFront = pCenter;

    float bC = texelFetch(divergence, T, 0).x;
    
    FragColor = (pLeft + pRight + pBottom + pTop + pFront + pBack - bC) / 6.0f;
}