#pragma once
#include "DXCore.h"	// Used for ID3D11 stuff
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects

class Material 
{
public:
	// Constructor and Destructor
	Material(Microsoft::WRL::ComPtr<ID3D11PixelShader> pxlShdr, Microsoft::WRL::ComPtr<ID3D11VertexShader> vtxShdr, DirectX::XMFLOAT4 mColTint);
	~Material();

	// Getters
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader();
	DirectX::XMFLOAT4 GetMaterialColorTint();

	// Setters
	void SetMaterialColorTint(DirectX::XMFLOAT4 inputTint);

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;

	DirectX::XMFLOAT4 matColorTint;
};