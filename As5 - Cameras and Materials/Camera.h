#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include "Transform.h"

class Camera 
{
public:
	Camera(DirectX::XMFLOAT3 initCamPos, DirectX::XMFLOAT3 initCamOri, float aspRat, float fov, float nearPlane, float farPlane, float moveSpd, float lookSpd);
	~Camera();

	// Getters
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float aspectRatio);

	void Update(float dt, HWND windowHandle);

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