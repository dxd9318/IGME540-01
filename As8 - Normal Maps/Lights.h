#pragma once
#include <DirectXMath.h>

struct DirectionalLight 
{
	/*
		Recall that vectors in constant buffers cannot cross 16-byte boundaries, 
		so the 3-component vectors need some padding between them. This will occur 
		automatically by the HLSL compiler, so we need to match it here.
	*/
	DirectX::XMFLOAT3 ambientColor;
	float padding1;
	DirectX::XMFLOAT3 diffuseColor;
	float padding2;
	DirectX::XMFLOAT3 direction;
};

struct PointLight 
{
	DirectX::XMFLOAT3 ambientColor;
	float padding1;
	DirectX::XMFLOAT3 diffuseColor;
	float padding2;
	DirectX::XMFLOAT3 position;
};