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
Texture2D normalMapTexture	: register(t1);
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
float4 main(VertexToPixelNormalMap input) : SV_TARGET
{
	// The following values will be used to calculate the final light color for each light
	// Normalize the normal of the current pixel. 
	input.normal = normalize(input.normal);
	// Normalizse the tangent of the current pixel.
	input.tangent = normalize(input.tanget);

	// Sample the texture to be applied to the current pixel, ie. determine surface color at this pixel
	float3 textureSample = diffuseTexture.Sample(samplerOptions, input.uv).rgb;
	// Sample and unpack the normal map, converting it from a [0-1] range to a [-1-1] range
	float3 unpackedNormal = normalMapTexture.Sample(samplerOptions, input.uv).rgb * 2 - 1;

	// Re-orthogonalize the normal and tangent vectors
	float3 T = normalize(input.tangent - input.normal * dot(input.tangent, input.normal));
	// Get the bi-tangent as the cross product of the normal and tangent
	float3 B = cross(T, input.normal);
	// Create the TBN matrix/axes
	float3x3 TBN = float3x3(T, B, input.normal);

	// Transform the normal map normal by the TBN
	input.normal = mul(unpackedNormal, TBN);


	return float4(
		calculateLightColor(input, cameraWorldPosition, specularIntensity, textureSample, directionalLight1.Direction, directionalLight1.DiffuseColor, directionalLight1.AmbientColor) +
			calculateLightColor(input, cameraWorldPosition, specularIntensity, textureSample, directionalLight2.Direction, directionalLight2.DiffuseColor, directionalLight2.AmbientColor) +
			calculateLightColor(input, cameraWorldPosition, specularIntensity, textureSample, directionalLight3.Direction, directionalLight3.DiffuseColor, directionalLight3.AmbientColor) +
			calculateLightColor(input, cameraWorldPosition, specularIntensity, textureSample, (input.worldPos - pointLight1.Position), pointLight1.DiffuseColor, pointLight1.AmbientColor),
		1.0f);
};

