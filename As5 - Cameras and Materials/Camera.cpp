#include "Camera.h"

Camera::Camera(DirectX::XMFLOAT3 initCamPos, DirectX::XMFLOAT3 initCamOri, float aspRat, float fov, float nearPlane, float farPlane, float moveSpd, float lookSpd)
{
	//init position, and orientation
	cameraTransform.SetPosition(initCamPos.x, initCamPos.y, initCamPos.z);
	cameraTransform.SetRotation(initCamOri.x, initCamOri.y, initCamOri.z);

	//init other customization options
	fovAngle = fov;
	nearPlaneDistance = nearPlane;
	farPlaneDistance = farPlane;
	movementSpeed = moveSpd;
	mouseLookSpeed = lookSpd;

	UpdateViewMatrix();
	UpdateProjectionMatrix(aspRat);
}

Camera::~Camera() 
{

}

// Getters
DirectX::XMFLOAT4X4 Camera::GetViewMatrix() 
{
	return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix() 
{
	return projectionMatrix;
}


void Camera::UpdateViewMatrix() 
{
	DirectX::XMStoreFloat4x4(
		&viewMatrix, 
		DirectX::XMMatrixLookToLH(
			DirectX::XMLoadFloat3(&cameraTransform.GetPosition()),	// camera's position
			DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMLoadFloat3(&cameraTransform.GetPitchYawRoll())),	// camera's forward vector, direction for camera to look to
			DirectX::XMVectorSet(0, 1, 0, 0))); // up vector
}

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
	DirectX::XMStoreFloat4x4(
		&projectionMatrix, 
		DirectX::XMMatrixPerspectiveFovLH(fovAngle, aspectRatio, nearPlaneDistance, farPlaneDistance));
}

void Camera::Update(float dt, HWND windowHandle) 
{
	// process user input
	// update transform and view matrix
		// scale camera's speed by dt


	if (GetAsyncKeyState('W') & 0x8000) 
	{
		// Relative Forward
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		// Relative Backward
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		// Relative Left
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		// Relative Right
	}

	if (GetAsyncKeyState('E') & 0x8000)
	{
		// Absolute Up
	}

	if (GetAsyncKeyState('Q') & 0x8000)
	{
		// Absolute Down
	}
}

/* 
	Not required, but for the group project consider adding a 
	Camera::GetTransform() method to have code elsewhere for controlling the camera.
	Would be useful if having multiple cameras in the world.	
*/