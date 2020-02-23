#pragma once
#include "DXCore.h"	// Used for ID3D11 stuff
#include <DirectXMath.h>
#include "SimpleShader.h"

class Material 
{
public:
	// Constructor and Destructor
	Material(SimpleVertexShader* vtxShdr, SimplePixelShader* pxlShdr, DirectX::XMFLOAT4 mColTint);
	~Material();

	// Getters
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();
	DirectX::XMFLOAT4 GetMaterialColorTint();

	// Setters
	void SetMaterialColorTint(DirectX::XMFLOAT4 inputTint);

private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	DirectX::XMFLOAT4 matColorTint;
};