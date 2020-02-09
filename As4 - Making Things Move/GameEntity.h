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

	void Draw();
};