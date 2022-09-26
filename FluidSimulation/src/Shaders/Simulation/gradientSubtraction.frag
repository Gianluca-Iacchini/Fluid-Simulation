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


    /*  
       Step and mix functions are used to avoid branching, functions are equivalent to the following if statement.
       if (texelFetchOffset(obstacle, T, 0, ivec3(-1,0,0,)).x)
       {
           pLeft = pCenter;
           vMask.x = 0.0f;
       }
    */
    // If any of the neighbour cell contains an obstacle then mark it and use this cell to compute the gradient instead.
    float stepLeft = step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(-1,0,0)).x);
    pLeft = mix(pLeft, pCenter, stepLeft);
    vMask.x = mix(1,0, stepLeft);

    float stepRight = step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(1,0,0)).x);
    pRight = mix(pRight, pCenter, stepRight);
    vMask.x = mix(1,0, stepRight);

    float stepBottom = step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(0,-1,0)).x);
    pBottom = mix(pBottom, pCenter, stepBottom);
    vMask.y = mix(1,0, stepBottom);

    float stepTop = step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(0,1,0)).x);
    pTop = mix(pTop, pCenter, stepTop);
    vMask.y = mix(1,0, stepTop);

    float stepBack = step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(0,0,-1)).x);
    pBack = mix(pBack, pCenter, stepBack);
    vMask.z = mix(1,0, stepBack);

    float stepFront = step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(0,0,1)).x);
    pFront = mix(pFront, pCenter, stepFront);
    vMask.z = mix(1,0, stepFront);



    // Subtracts pressure gradient from velocity to obtain final velocity.
    vec3 oldVel = texelFetch(velocity, T, 0).xyz;
    vec3 gradient = 0.5f *  vec3(pRight - pLeft, pTop - pBottom, pFront - pBack);
    vec3 newVel = oldVel - gradient;
    
    FragColor = vMask * newVel;
}