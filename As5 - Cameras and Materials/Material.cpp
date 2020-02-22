#include "Material.h"

// Constructor
Material::Material(Microsoft::WRL::ComPtr<ID3D11PixelShader> pxlShdr, Microsoft::WRL::ComPtr<ID3D11VertexShader> vtxShdr, DirectX::XMFLOAT4 mColTint)
{
	vertexShader = vtxShdr;
	pixelShader = pxlShdr;

	matColorTint = mColTint;
}

// Destructor
Material::~Material() {}

// Getters
Microsoft::WRL::ComPtr<ID3D11VertexShader> Material::GetVertexShader() { return vertexShader; }

Microsoft::WRL::ComPtr<ID3D11PixelShader> Material::GetPixelShader() { return pixelShader; }

DirectX::XMFLOAT4 Material::GetMaterialColorTint() { return matColorTint; }

// Setters
void Material::SetMaterialColorTint(DirectX::XMFLOAT4 inputTint) { matColorTint = inputTint; }
