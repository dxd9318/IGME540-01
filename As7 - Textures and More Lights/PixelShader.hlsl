
struct DirectionalLight
{
	float3 AmbientColor;
	// padding would go here
	float3 DiffuseColor;
	// padding would go here
	float3 Direction;
};

struct PointLight
{
	float3 AmbientColor;
	// padding would go here
	float3 DiffuseColor;
	// padding would go here
	float3 Position;
};

cbuffer ExternalPSData : register(b0) 
{
	DirectionalLight directionalLight1;
	DirectionalLight directionalLight2;
	DirectionalLight directionalLight3;

	PointLight pointLight1;

	float specularIntensity;
	float3 cameraWorldPosition;
}

Texture2D diffuseTexture	: register(t0); // "t" registers
SamplerState samplerOptions	: register(s0); // "s" registers

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: POSITION;		// pixel position to pass into pixelShader for point light and specularity calculations
};

/*
	For directional lights, just use the light's direction as the lightDirection input.
	For point lights, use (input.worldPos - pointLight.position) as the lightDirection input.
*/
float3 calculateLightColor(VertexToPixel input, float3 surfaceColor, float3 lightDirection, float3 lightDiffuseColor, float3 lightAmbientColor) 
{
	// Normalized direction TO the light
	float3 lightDir = normalize(-lightDirection);	// lightDirection is always negated no matter the light source type

	// Calculate light amount
	float dotResult = saturate(dot(input.normal, lightDir));	// dot product of normal to light direction, clamped between 0 and 1

	// Calculate specularity of light given pixel's material and position in relation to the camera
	/* 
		The Phong algorithm describes specularity as the dot product between 
		the vector of the reflection of the light source, and the surface-to-camera "view" vector.
		This dot product determines how close the camera is to that reflection. 
		We then raise the result to an exponent (specularIntensity) to determine the fall off, which 
		determines how matte vs shiny a surface appears.
	*/
	float3 V = normalize(cameraWorldPosition - input.worldPos);
	float3 R = reflect(normalize(lightDirection), input.normal);
	float spec = specularIntensity * pow(saturate(dot(R, V)), 64.0f);

	// Calculate final pixel color	// lightAmount * lightColor * surfaceColor + ambientColor * surfaceColor
	/* 
		Diffuse: dotResult * diffuseColor * colorTint
			PLUS
		Ambient: ambientColor * colorTint
			PLUS
		Specular: float spec = specularIntensity * pow(saturate(dot(R,V)), specExponent);
	*/
	float3 finalColor = (surfaceColor * input.color.rgb) * (dotResult * lightDiffuseColor + lightAmbientColor) + spec;

	return float3(finalColor);
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// The following values will be used to calculate the final light color for each light
	// Normalize the normal of the current pixel. 
	input.normal = normalize(input.normal);
	// Sample the texture to be applied to the current pixel, ie. determine surface color at this pixel
	float3 textureSample = diffuseTexture.Sample(samplerOptions, input.uv).rgb;

	return float4(
		calculateLightColor(input, textureSample, directionalLight1.Direction, directionalLight1.DiffuseColor, directionalLight1.AmbientColor) +
			calculateLightColor(input, textureSample, directionalLight2.Direction, directionalLight2.DiffuseColor, directionalLight2.AmbientColor) +
			calculateLightColor(input, textureSample, directionalLight3.Direction, directionalLight3.DiffuseColor, directionalLight3.AmbientColor) +
			calculateLightColor(input, textureSample, (input.worldPos - pointLight1.Position), pointLight1.DiffuseColor, pointLight1.AmbientColor),
		1.0f);
};

