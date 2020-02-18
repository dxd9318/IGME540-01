#pragma once
#include <DirectXMath.h>

class Transform {
public:
	Transform();
	~Transform();

	// Getters
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetPitchYawRoll();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT4X4 GetWorldMatrix();

	// Setters
	void SetPosition(float x, float y, float z);
	void SetRotation(float pitch, float yaw, float roll);
	void SetScale(float x, float y, float z);

	// Transformers
	void MoveAbsolute(float x, float y, float z);
	void Rotate(float pitch, float yaw, float roll);
	void Scale(float x, float y, float z);

	// Relative
	void MoveRelative(float x, float y, float z);
	DirectX::XMVECTOR currentDirection;
	DirectX::XMVECTOR currentRotation;
	DirectX::XMFLOAT3 relativePosition;

private:
	DirectX::XMFLOAT4X4 transformWorldMatrix; // represents entity's current position, rotation and scale.
	DirectX::XMFLOAT3 transformPosition;
	DirectX::XMFLOAT3 transformScale;
	DirectX::XMFLOAT3 transformRotation;

	bool matrixDirty; // if any transform field is updated, this will too to signal the matrix to update

	
};