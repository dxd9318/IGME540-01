#pragma once
#include <DirectXMath.h>

struct VertexShaderExternalData 
{
	DirectX::XMFLOAT4 colorTint;
	//DirectX::XMFLOAT3 offset;
	DirectX::XMFLOAT4X4 worldMatrix; // replaces offset of previous line
};