#version 410 core

out vec4 FragColor;


in RayData
{
	vec4 pos;
	vec3 posInGrid;
	float depth;
} rayData;

uniform bool isBackFace;
uniform vec2 screenSize;
uniform float zNear;
uniform float zFar;

uniform sampler2D depthTexture;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));	
}


void main()
{    
	float depthValue = texture(depthTexture, gl_FragCoord.xy / screenSize).x;
	float linearDepth = LinearizeDepth(depthValue);

	// We set the back face to a negative green value. If the front face is clipped (e.g. camera is inside the simulation box)
	// Then the negative value will remain; otherwise the .rgb channels will be overwritten with the front faces values;
	if (isBackFace)
	{
		FragColor.xyz = vec3(0.f, -1.0f, 0.0f);
		FragColor.w = min(rayData.depth, linearDepth);
	}
	else
	{
		// This condition is used to check if an object is between the camera and the fluid, if that's the case than a negative value will be written to the texture red channel (we are outputting a positive value
		// because we are using subtractive blending).
		if (linearDepth < rayData.depth) 
		{
			FragColor = vec4(1, 0, 0, 0);
			return;
		}

		// Returns the tex coords position, adjusted to account for negative blending
		FragColor.xyz = -rayData.posInGrid;
		// Returns depth through volume in the alpha channel
		FragColor.w = rayData.depth;
	}
}