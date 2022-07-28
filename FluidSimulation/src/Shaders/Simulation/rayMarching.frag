#version 410 core

out vec4 FragColor;

// Camera position in texture coords range [0-1]
uniform vec3 eyeOnGrid;
// Dimension of the grid
uniform vec3 gridDim;
uniform vec3 recGridDim;
// Scale factor (in case of rectangular boxes)
uniform float gridScaleFactor = 1.0f;
uniform float maxGridDim;
// Ray tracing texture size (should be same as screen size)
uniform vec2 RTSize;
// Near plane
uniform float zNear;
uniform sampler2D rayDataTexture;
uniform sampler2D jitterTexture;
uniform sampler3D dyeTexture;

in RayCastData {
	vec4 pos;
	vec3 posInGrid;
} rayCastData;

// Sample dye texture at the specified position.
void DoSample(float weight, vec3 O, inout vec4 color)
{
	vec3 texcoords;
	vec4 sampleColor;
	float t;

	texcoords = vec3(O.x, O.y, O.z);
	sampleColor = weight * texture(dyeTexture, texcoords);
	sampleColor.a = (sampleColor.a) * 0.1;

	t = sampleColor.a * (1.0f - color.a);
	color.rgb += sampleColor.rgb * t;
	color.a += t;
}

void main()
{    
	vec4 outColor = vec4(0);

	vec4 rayData = texture(rayDataTexture ,gl_FragCoord.xy / RTSize);

	if (rayData.x < 0)
	{
		FragColor = outColor;
		return;
	}

	if (rayData.y < 0)
	{
		rayData.xyz = (rayCastData.posInGrid / gl_FragCoord.z);
		rayData.w = rayData.w - zNear;
	}



	vec3 rayOrigin = rayData.xyz;
	
	float rayLength = rayData.w;

	// Samples are proportional to grid dimension 
	float fSamples = (rayLength / gridScaleFactor * maxGridDim) * 2.0f;
	int nSamples = int(floor(fSamples));

	float offset = texture(jitterTexture, gl_FragCoord.xy / 256.f).x;

	// ray direction is given by the eye origin and the front faces of the box
	vec3 stepVec = normalize((rayOrigin - eyeOnGrid) * gridDim) * recGridDim * 0.5f;

	vec3 O = rayOrigin + stepVec * offset;

	int i = 0;

	for (i; i < nSamples; i++)
	{
		DoSample(1, O, outColor);
		O += stepVec;

		if (outColor.a > 0.99f)
			break;
	}

	// Last sample is used to correct banding artifacts
	if (i == nSamples)
	{
		DoSample(fract(fSamples), O, outColor);
	}

	FragColor = outColor;
	
}