#version 410

out vec3 FragColor;

in vec3 texCoords;
in vec3 cellCoords;

uniform sampler3D obstacle;
uniform sampler3D velocity;
uniform sampler3D temperature;
uniform sampler3D dye;

uniform float ambientTemp;
uniform float timeStep;
uniform float buoyancy;
uniform float weight;

bool IsNonEmptyCell(ivec3 coords)
{
    return texelFetch(obstacle, coords, 0).x > 0;
}

void main() {

    ivec3 coord = ivec3(cellCoords);

    // If there is an obstacle in the current cell then kill the fragment shader early.
    if (IsNonEmptyCell(coord))
    {
        FragColor = vec3(0);
        return;
    }

	float temperatureValue = texelFetch(temperature, coord, 0).x;
	vec3 velocity = texelFetch(velocity, coord, 0).xyz;

    FragColor = velocity;

    // We are using an inverted vec3 (0,-1,0) to launch the fluid upwards like smoke would. 
    vec3 dyeValue = texelFetch(dye, coord, 0).xyz;
    FragColor += (timeStep * (temperatureValue - ambientTemp) * buoyancy - length(dyeValue) * weight) * vec3(0, -1, 0);
    

}