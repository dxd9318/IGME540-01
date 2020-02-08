#pragma once
#include <DirectXMath.h>

class Transform {
public:

private:
	DirectX::XMFLOAT4X4 transformWorldMatrix; // represents entity's current position, rotation and scale.
	DirectX::XMFLOAT3 transformPosition;
	DirectX::XMFLOAT3 transformScale;
	DirectX::XMFLOAT3 transformRotation;
};