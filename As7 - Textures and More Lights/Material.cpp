#include "Material.h"

// Constructor
Material::Material(SimpleVertexShader* vtxShdr, SimplePixelShader* pxlShdr, DirectX::XMFLOAT4 mColTint, float mSpec)
{
	vertexShader = vtxShdr;
	pixelShader = pxlShdr;

	matColorTint = mColTint;
	matSpecularity = mSpec;
}

// Destructor
Material::~Material() {}

// Getters
SimpleVertexShader* Material::GetVertexShader() { return vertexShader; }

SimplePixelShader* Material::GetPixelShader() { return pixelShader; }

DirectX::XMFLOAT4 Material::GetMaterialColorTint() { return matColorTint; }

float Material::GetMaterialSpecularity() { return matSpecularity; }

// Setters
void Material::SetMaterialColorTint(DirectX::XMFLOAT4 inputTint) { matColorTint = inputTint; }
