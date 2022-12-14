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

// Computes velocity divergence using the discrete formula
void main()
{
    
    ivec3 T = ivec3(cellCoords);

    vec3 vLeft =   texelFetchOffset(velocity, T, 0, ivec3(-1, 0, 0)).xyz; 
    vec3 vRight =  texelFetchOffset(velocity, T, 0, ivec3(1, 0, 0)).xyz; 
    vec3 vBottom = texelFetchOffset(velocity, T, 0, ivec3(0, -1, 0)).xyz; 
    vec3 vTop =    texelFetchOffset(velocity, T, 0, ivec3(0, 1, 0)).xyz; 
    vec3 vBack =   texelFetchOffset(velocity, T, 0, ivec3(0, 0, -1)).xyz; 
    vec3 vFront =  texelFetchOffset(velocity, T, 0, ivec3(0, 0, 1)).xyz; 


    /*  Mix functions is used to avoid branching, the assignments are equivalent to the following if statement.
       if (texelFetchOffset(obstacle, T, 0, ivec3(-1,0,0,)).x)
       {
           vLeft = vec3(0);
       }
    */
    vLeft = vLeft * (1.f - step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(-1,0,0)).x));
    vRight = vRight * (1.f - step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(1,0,0)).x));
    vBottom = vBottom * (1.f - step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(0,-1,0)).x));
    vTop = vTop * (1.f - step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(0,1,0)).x));
    vBack = vBack * (1.f - step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(0,0,-1)).x));
    vFront = vFront * (1.f - step(0.3f, texelFetchOffset(obstacle, T, 0, ivec3(0,0,1)).x));


    float divergence = 0.5f * ((vRight.x - vLeft.x) + (vTop.y - vBottom.y) + (vFront.z - vBack.z));
    FragColor = divergence;

    
}