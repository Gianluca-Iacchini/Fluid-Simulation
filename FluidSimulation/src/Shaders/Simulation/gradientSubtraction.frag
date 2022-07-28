#version 410 core


out vec3 FragColor;

in vec3 texCoords;
in vec3 cellCoords;


uniform sampler3D obstacle;
uniform sampler3D pressure;
uniform sampler3D velocity;

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
        FragColor = vec3(0);
        return;
    }
    

    // Compututes pressure gradient
    float pLeft =   texelFetchOffset(pressure, T, 0, ivec3(-1, 0, 0)).x; 
    float pRight =  texelFetchOffset(pressure, T, 0, ivec3(1, 0, 0)).x; 
    float pBottom = texelFetchOffset(pressure, T, 0, ivec3(0, -1, 0)).x; 
    float pTop =    texelFetchOffset(pressure, T, 0, ivec3(0, 1, 0)).x; 
    float pBack =   texelFetchOffset(pressure, T, 0, ivec3(0, 0, -1)).x; 
    float pFront =  texelFetchOffset(pressure, T, 0, ivec3(0, 0, 1)).x; 
    float pCenter = texelFetch(pressure, T, 0).x;


    vec3 vMask = vec3(1.f);

    // If any of the neighbour cell contains an obstacle then mark it and use this cell to compute the gradient instead.
    if (IsBoundaryCell(T, ivec3(-1, 0, 0))) { pLeft = pCenter; vMask.x = 0;}
    if (IsBoundaryCell(T, ivec3(1, 0, 0))) { pRight = pCenter; vMask.x = 0;}
    if (IsBoundaryCell(T, ivec3(0, -1, 0))) { pBottom = pCenter; vMask.y = 0;}
    if (IsBoundaryCell(T, ivec3(0, 1, 0))) { pTop = pCenter; vMask.y = 0;}
    if (IsBoundaryCell(T, ivec3(0, 0, -1))) { pBack = pCenter; vMask.z = 0;}
    if (IsBoundaryCell(T, ivec3(0, 0, 1))) { pFront = pCenter; vMask.z = 0;}

    vec3 oldVel = texelFetch(velocity, T, 0).xyz;
    vec3 gradient = 0.5f *  vec3(pRight - pLeft, pTop - pBottom, pFront - pBack);
    vec3 newVel = oldVel - gradient;
    
    FragColor = vMask * newVel;
}