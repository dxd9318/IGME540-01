#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"

class GameEntity {
public:
	// Constructor and Destructor
	GameEntity(Mesh* inputMesh, Material* inputMat);
	~GameEntity();

	// Getters
	Transform* GetTransform();
	Mesh* GetMesh();

private:
	Transform entityTransform;
	Mesh* entityMesh;
	Material* entityMaterial;
};