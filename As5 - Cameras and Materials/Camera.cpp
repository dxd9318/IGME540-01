#include "Camera.h"

// Constructor
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

	prevMousePos = { 0,0 };

	UpdateViewMatrix();
	UpdateProjectionMatrix(aspRat);
}

// Destructor
Camera::~Camera() {}

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
			DirectX::XMVector3Rotate(
				DirectX::XMVectorSet(0, 0, 1, 0), 
				DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&cameraTransform.GetPitchYawRoll()))),	// camera's forward vector, direction for camera to look to
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

	// Keyboard input for camera movement
	if (GetAsyncKeyState('W') & 0x8000) 
	{
		// Relative Forward
		cameraTransform.MoveRelative(0.0f, 0.0f, 5.0f * dt);
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		// Relative Backward
		cameraTransform.MoveRelative(0.0f, 0.0f, -5.0f * dt);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		// Relative Left
		cameraTransform.MoveRelative(-5.0f * dt, 0.0f, 0.0f);
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		// Relative Right
		cameraTransform.MoveRelative(5.0f * dt, 0.0f, 0.0f);
	}

	if (GetAsyncKeyState('E') & 0x8000)
	{
		// Absolute Up
		cameraTransform.MoveAbsolute(0.0f, 5.0f * dt, 0.0f);
	}

	if (GetAsyncKeyState('Q') & 0x8000)
	{
		// Absolute Down
		cameraTransform.MoveAbsolute(0.0f, -5.0f * dt, 0.0f);
	}


	// Mouse input for camera rotation
	POINT mousePos = {};
	GetCursorPos(&mousePos);	// fills the mousePos struct with its coords
	ScreenToClient(windowHandle, &mousePos);	// makes point relative to client window

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) 
	{
		// rotate camera based on how much mouse has moved
		float deltaCamPosX = prevMousePos.x - mousePos.x; 
		float deltaCamPosY = prevMousePos.y - mousePos.y;

		// rotate camera((mouseLookSpeed * dt * -deltaCamPosY) as pitch,  (mouseLookSpeed * dt * -deltaCamPosX)as yaw, 0 as roll)
		cameraTransform.Rotate(mouseLookSpeed * dt * -deltaCamPosY, mouseLookSpeed * dt * -deltaCamPosX, 0.0f);
	}

	// Any input requires the view matrix to be updated
	UpdateViewMatrix();

	// Update prevMousePos using mousePos
	prevMousePos = mousePos;
}

/* 
	Not required, but for the group project consider adding a 
	Camera::GetTransform() method to have code elsewhere for controlling the camera.
	Would be useful if having multiple cameras in the world.	
*/