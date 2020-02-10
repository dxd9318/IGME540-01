#pragma once
#include "Mesh.h"
#include "Transform.h"

class GameEntity {
public:
	GameEntity(Mesh* inputMesh);
	~GameEntity();

	// Getters
	Mesh* GetMesh();
	Transform* GetTransform();

private:
	Transform entityTransform;
	Mesh* entityMesh;

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer);
};