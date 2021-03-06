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
Transform::~Transform() {}

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

void Transform::MoveRelative(float x, float y, float z) 
{
	// change in position (ie. direction vector) we want to apply to this transform
	DirectX::XMVECTOR relativeForward = DirectX::XMVectorSet(x, y, z, 0.0f);

	// tranform's current rotation, to be applied to the above direction vector
	DirectX::XMVECTOR currentRotation = DirectX::XMQuaternionRotationRollPitchYaw(transformRotation.x, transformRotation.y, transformRotation.z);	

	// update relativeForward to account for the transform's rotation
	relativeForward = DirectX::XMVector3Rotate(relativeForward, currentRotation);	

	// add relativeForward to current position, then update current position
	DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&transformPosition), relativeForward);
	DirectX::XMStoreFloat3(&transformPosition, newPosition);
}