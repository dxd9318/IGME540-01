
struct DirectionalLight
{
	float3 AmbientColor;
	// padding would go here
	float3 DiffuseColor;
	// padding would go here
	float3 Direction;
};

cbuffer ExternalPSData : register(b0) 
{
	DirectionalLight directionalLight;
}

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
};

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
	// Normalized direction TO the light
	float3 lightDir = normalize(-directionalLight.Direction);

	input.normal = normalize(input.normal);

	// Calculate light amount (Normal-dot-LightDirection)
	float dotResult = saturate(dot(input.normal, lightDir));	// clamped between 0 and 1

	// Calculate final pixel color
	// dotResult * diffuseColor * colorTint + ambientColor * colorTint
	float3 finalColor = dotResult * directionalLight.DiffuseColor * input.color + directionalLight.AmbientColor * input.color;

	return float4(finalColor, 1);

	//return float4(input.normal, 1);
	//return float4(directionalLight.DiffuseColor, 1);

	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	//return input.color;
}