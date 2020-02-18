#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include "Transform.h"

class Camera 
{
public:
	Camera();
	~Camera();

	// Getters
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();


private:
	Transform cameraTransform;

	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	POINT prevMousePos;

	float fovAngle;
	float nearPlaneDistance;
	float farPlaneDistance;
	float movementSpeed;
	float mouseLookSpeed;
};