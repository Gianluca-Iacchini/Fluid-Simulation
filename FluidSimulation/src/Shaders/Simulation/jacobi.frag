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




    /*  
       Step and mix functions are used to avoid branching, functions are equivalent to the following if statement.
       if (texelFetchOffset(obstacle, T, 0, ivec3(-1,0,0,)).x)
       {
           pLeft = pCenter;
       }
    */

    float stepLeft = step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(-1,0,0)).x);
    pLeft = mix(pLeft, pCenter, stepLeft);

    float stepRight = step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(1,0,0)).x);
    pRight = mix(pRight, pCenter, stepRight);

    float stepBottom = step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(0,-1,0)).x);
    pBottom = mix(pBottom, pCenter, stepBottom);

    float stepTop = step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(0,1,0)).x);
    pTop = mix(pTop, pCenter, stepTop);

    float stepBack = step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(0,0,-1)).x);
    pBack = mix(pBack, pCenter, stepBack);

    float stepFront = step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(0,0,1)).x);
    pFront = mix(pFront, pCenter, stepFront);

    float bC = texelFetch(divergence, T, 0).x;
    
    // Computes this cell pressure by using the Poisson-pressure equation
    FragColor = (pLeft + pRight + pBottom + pTop + pFront + pBack - bC) / 6.0f;
}