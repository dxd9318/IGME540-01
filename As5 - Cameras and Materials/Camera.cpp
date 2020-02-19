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

	//UpdateViewMatrix();
	//UpdateProjectionMatrix();
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

}

void Camera::UpdateProjectionMatrix() 
{

}