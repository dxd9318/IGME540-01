#pragma once
#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects

class Material 
{
public:
	// Ctor and Dtor
	Material();
	~Material();

	// Getters
	DirectX::XMFLOAT4 GetMaterialColorTint();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetVertexShader();
	
	// Setters
	void SetMaterialColorTint();

private:
	DirectX::XMFLOAT4 matColorTint;

	// MIGHT HAVE TO COMMENT OUT IN GAME.H <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
};