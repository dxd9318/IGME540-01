#include "Transform.h"

// Constructor
Transform::Transform() 
{
	// Initialize transform values and world matrix
	transformPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	transformScale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	transformRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(&transformWorldMatrix, DirectX::XMMatrixIdentity());

	matrixDirty = false;
}

// Destructor
Transform::~Transform() 
{

}

// Setters
void Transform::SetPosition(float x, float y, float z) 
{
	transformPosition = DirectX::XMFLOAT3(x, y, z);

	if (matrixDirty == false) matrixDirty = true;
}

void Transform::SetRotation(float pitch, float yaw, float roll) 
{
	transformRotation = DirectX::XMFLOAT3(pitch, yaw, roll);

	if (matrixDirty == false) matrixDirty = true;
}

void Transform::SetScale(float x, float y, float z) 
{
	transformScale = DirectX::XMFLOAT3(x, y, z);

	if (matrixDirty == false) matrixDirty = true;
}

// Getters
DirectX::XMFLOAT3 Transform::GetPosition() { return transformPosition; }

DirectX::XMFLOAT3 Transform::GetPitchYawRoll() { return transformRotation; }

DirectX::XMFLOAT3 Transform::GetScale() { return transformScale; }

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix() 
{
	if (matrixDirty) {
		DirectX::XMMATRIX tempTranslation = DirectX::XMMatrixTranslation(
			transformPosition.x,
			transformPosition.y,
			transformPosition.z);

		DirectX::XMMATRIX tempRotation = DirectX::XMMatrixRotationRollPitchYaw(
			transformRotation.x,	// pitch
			transformRotation.y,	// yaw
			transformRotation.z);	// roll

		DirectX::XMMATRIX tempScale = DirectX::XMMatrixScaling(
			transformScale.x,
			transformScale.y,
			transformScale.z);

		// Applies translation, then rotation, then scale (matrix math)
		DirectX::XMMATRIX tempWorld = tempScale * tempRotation * tempTranslation;

		DirectX::XMStoreFloat4x4(&transformWorldMatrix, tempWorld);

		matrixDirty = false;	//reset to false
	}

	return transformWorldMatrix;
}

// Transformers
void Transform::MoveAbsolute(float x, float y, float z) 
{
	transformPosition = DirectX::XMFLOAT3(
		transformPosition.x + x, 
		transformPosition.y + y, 
		transformPosition.z + z);
}

void Transform::Rotate(float pitch, float yaw, float roll) 
{
	transformRotation = DirectX::XMFLOAT3(
		transformRotation.x + pitch,
		transformRotation.y + yaw,
		transformRotation.z + roll);
}

void Transform::Scale(float x, float y, float z) 
{
	transformScale = DirectX::XMFLOAT3(
		transformScale.x * x,
		transformScale.y * y,
		transformScale.z * z);
}