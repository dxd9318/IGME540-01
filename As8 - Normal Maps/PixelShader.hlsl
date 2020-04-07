#include "ShaderIncludes.hlsli"

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
		calculateLightColor(input, cameraWorldPosition, specularIntensity, textureSample, directionalLight1.Direction, directionalLight1.DiffuseColor, directionalLight1.AmbientColor) +
			calculateLightColor(input, cameraWorldPosition, specularIntensity, textureSample, directionalLight2.Direction, directionalLight2.DiffuseColor, directionalLight2.AmbientColor) +
			calculateLightColor(input, cameraWorldPosition, specularIntensity, textureSample, directionalLight3.Direction, directionalLight3.DiffuseColor, directionalLight3.AmbientColor) +
			calculateLightColor(input, cameraWorldPosition, specularIntensity, textureSample, (input.worldPos - pointLight1.Position), pointLight1.DiffuseColor, pointLight1.AmbientColor),
		1.0f);
};

