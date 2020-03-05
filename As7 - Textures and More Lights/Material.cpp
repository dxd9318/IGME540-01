#include "Material.h"

// Constructor
Material::Material(
	SimpleVertexShader* vtxShdr,
	SimplePixelShader* pxlShdr,
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV,
	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSS,
	DirectX::XMFLOAT4 mColTint,
	float mSpec)
{
	vertexShader = vtxShdr;
	pixelShader = pxlShdr;

	matSRV = mSRV;
	matSamplerState = mSS;

	matColorTint = mColTint;
	matSpecularity = mSpec;
}

// Destructor
Material::~Material() {}

// Getters
SimpleVertexShader* Material::GetVertexShader() { return vertexShader; }

SimplePixelShader* Material::GetPixelShader() { return pixelShader; }

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetSRV() { return matSRV; }

Microsoft::WRL::ComPtr<ID3D11SamplerState> Material::GetSamplerState() { return matSamplerState; }

DirectX::XMFLOAT4 Material::GetMaterialColorTint() { return matColorTint; }

float Material::GetMaterialSpecularity() { return matSpecularity; }

// Setters
void Material::SetMaterialColorTint(DirectX::XMFLOAT4 inputTint) { matColorTint = inputTint; }
