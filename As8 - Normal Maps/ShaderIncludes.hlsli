#ifndef __SHADER_INCLUDES__
#define __SHADER_INCLUDES__

//structs

// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
};

// Struct representing the data we're sending down the pipeline
// - Should match our pixel shader's input (hence the name: Vertex to Pixel)
// - At a minimum, we need a piece of data defined tagged as SV_POSITION
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float4 color		: COLOR;        // RGBA color
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: POSITION;		//pixel position to pass into pixelShader for point light calculations
};

struct VertexToPixelNormalMap
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float4 color		: COLOR;        // RGBA color
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: POSITION;		//pixel position to pass into pixelShader for point light calculations
	float3 tangent		: TANGENT;
};

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


// helper functions

/*
	For directional lights, just use the light's direction as the lightDirection input.
	For point lights, use (input.worldPos - pointLight.position) as the lightDirection input.
*/
float3 calculateLightColor(VertexToPixel input, float3 camWrldPos, float specInt, float3 surfaceColor, float3 lightDirection, float3 lightDiffuseColor, float3 lightAmbientColor)
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
	float3 V = normalize(camWrldPos - input.worldPos);
	float3 R = reflect(normalize(lightDirection), input.normal);
	float spec = specInt * pow(saturate(dot(R, V)), 64.0f);

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


// preprocessor directives


#endif