#include "Material.h"

// Constructor
Material::Material(SimpleVertexShader* vtxShdr, SimplePixelShader* pxlShdr, DirectX::XMFLOAT4 mColTint)
{
	vertexShader = vtxShdr;
	pixelShader = pxlShdr;

	matColorTint = mColTint;
}

// Destructor
Material::~Material() {}

// Getters
SimpleVertexShader* Material::GetVertexShader() { return vertexShader; }

SimplePixelShader* Material::GetPixelShader() { return pixelShader; }

DirectX::XMFLOAT4 Material::GetMaterialColorTint() { return matColorTint; }

// Setters
void Material::SetMaterialColorTint(DirectX::XMFLOAT4 inputTint) { matColorTint = inputTint; }
