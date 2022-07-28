#version 410 core

out float FragColor;

in vec3 texCoords;
in vec3 cellCoords;


uniform vec3 gridDimension;

#define OBSTACLE_EXTERIOR  0.0f
#define OBSTACLE_BOUNDARY  128.0f/255.0f
#define OBSTACLE_INTERIOR  1.0f


// Set the box faces as obstacles. We don't care about branching here because this shader is run only once at the fluid simulation initialization step.
void main()
{
    ivec3 T = ivec3(cellCoords);

	if (T.x <= 1 || T.x >= gridDimension.x - 1)
	{
		FragColor = OBSTACLE_BOUNDARY;
	}
	else if (T.y <= 1 || T.y >= gridDimension.y - 1)
	{
		FragColor = OBSTACLE_BOUNDARY;
	}
	else if (T.z <= 1 || T.z >= gridDimension.z - 1)
	{
		FragColor = OBSTACLE_BOUNDARY;
	}
	else
	{
		FragColor = OBSTACLE_EXTERIOR;
	}

}