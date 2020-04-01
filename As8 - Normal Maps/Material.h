#pragma once
#include "DXCore.h"	// Used for ID3D11 stuff
#include <DirectXMath.h>
#include "SimpleShader.h"
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include "WICTextureLoader.h"

class Material 
{
public:
	// Constructor and Destructor
	Material(
		SimpleVertexShader* vtxShdr, 
		SimplePixelShader* pxlShdr, 
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV, 
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mSS, 
		DirectX::XMFLOAT4 mColTint, 
		float mSpec);
	~Material();

	// Getters
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRV();
	Microsoft::WRL::ComPtr<ID3D11SamplerState> GetSamplerState();
	DirectX::XMFLOAT4 GetMaterialColorTint();
	float GetMaterialSpecularity();

	// Setters
	void SetMaterialColorTint(DirectX::XMFLOAT4 inputTint);

private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> matSRV;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> matSamplerState;

	DirectX::XMFLOAT4 matColorTint;
	float matSpecularity; //specular intensity, from 0 to 1
};