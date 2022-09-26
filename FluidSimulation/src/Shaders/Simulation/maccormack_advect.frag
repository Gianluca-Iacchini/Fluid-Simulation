#version 410 core



out vec4 FragColor;

in vec3 texCoords;
in vec3 cellCoords;

uniform sampler3D obstacle;
uniform sampler3D velocity;
uniform sampler3D phi_n;
uniform sampler3D phi_hat;
uniform sampler3D phi_hat_1;

uniform float timeStep;
uniform float dissipation;
uniform vec3 gridDimension;

vec4 phiMin;
vec4 phiMax;

#define OBSTACLE_BOUNDARY  128.0f/255.0f
bool IsNonEmptyCell(vec3 tCoords)
{
    return texture(obstacle, tCoords).x > 0;
}




void calcPhiMinMax(vec3 p) {
	vec3 uv = (floor(p - 0.5) + 0.5) * (1.f / gridDimension);
    vec4 d000 = texture(phi_n, uv + vec3(0,0,0) * (1.f / gridDimension));
    vec4 d100 = texture(phi_n, uv + vec3(1,0,0) * (1.f / gridDimension));
    vec4 d010 = texture(phi_n, uv + vec3(0,1,0) * (1.f / gridDimension));
    vec4 d110 = texture(phi_n, uv + vec3(1,1,0) * (1.f / gridDimension));
	vec4 d001 = texture(phi_n, uv + vec3(0,0,1) * (1.f / gridDimension));
    vec4 d101 = texture(phi_n, uv + vec3(1,0,1) * (1.f / gridDimension));
    vec4 d011 = texture(phi_n, uv + vec3(0,1,1) * (1.f / gridDimension));
    vec4 d111 = texture(phi_n, uv + vec3(1,1,1) * (1.f / gridDimension));

	phiMin = min(min(min(min(min(min(min(d000, d001), d010), d011), d100), d101), d110), d111);
	phiMax = max(max(max(max(max(max(max(d000, d001), d010), d011), d100), d101), d110), d111);

}

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


// This is an implementation of the MacCormack scheme, which performs two intermediate advection steps to achieve higher-order accuray. With this advection scheme the fluid will look less viscous.
void main() {

    // If there is an obstacle in the current cell then kill the fragment shader early.
    if (IsNonEmptyCell(texCoords))
    {
        FragColor = vec4(0);
        return;
    }


    vec3 u = texture(velocity, texCoords).xyz;
    vec3 coord = cellCoords - timeStep * u;
	calcPhiMinMax(coord);
	
	vec4 phiHatSample = texture(phi_hat, texCoords);
	vec4 phiSample = texture(phi_n, texCoords);
	vec4 phiHat1Sample = trilerp(phi_hat_1, coord, 1.f / gridDimension);

	vec4 result = phiHat1Sample + 0.5 * (phiSample - phiHatSample);

    // We clamp the result because MacCormack is not unconditionally stable.
	result = max(min(result, phiMax), phiMin);
    
	FragColor = dissipation * result;
}

