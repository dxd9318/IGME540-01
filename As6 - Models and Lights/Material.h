#pragma once
#include "DXCore.h"	// Used for ID3D11 stuff
#include <DirectXMath.h>
//#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include "SimpleShader.h"

class Material 
{
public:
	// Constructor and Destructor
	Material(SimpleVertexShader* vtxShdr, SimplePixelShader* pxlShdr, DirectX::XMFLOAT4 mColTint);
	~Material();

	// Getters
	//Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader();
	//Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader();
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();
	DirectX::XMFLOAT4 GetMaterialColorTint();

	// Setters
	void SetMaterialColorTint(DirectX::XMFLOAT4 inputTint);

private:
	//Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	//Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	DirectX::XMFLOAT4 matColorTint;
};