#include "Camera.h"

Camera::Camera() 
{
	//init position, and orientation

	//init aspect ratio, fov, near/far plane, movement speed and mouse look speed

	//UpdateViewMatrix();
	//UpdatePorjectionMatrix();
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