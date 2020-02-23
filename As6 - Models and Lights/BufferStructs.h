#pragma once
#include <DirectXMath.h>

struct VertexShaderExternalData 
{
	DirectX::XMFLOAT4 colorTint;
	DirectX::XMFLOAT4X4 worldMatrix; // replaces offset of previous line
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4	projectionMatrix;
};