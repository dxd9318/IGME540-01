#include "GameEntity.h"

// Constructor
GameEntity::GameEntity(Mesh* inputMesh)
{
	Mesh* entityMesh = inputMesh;
}

// Destructor
GameEntity::~GameEntity() 
{
	/*
		There isn't really much for a destructor to do here. As the Mesh object is not 
		instantiated by this class, it should NOT be deleted by this class! In general, a class 
		shouldn't delete an object it didn't create.
		
		Case in point: say a Mesh pointer is shared between two Game Entities; if the destructor 
		of each entity tried to delete that Mesh pointer, the first would succeed but the second 
		would crash, as the Mesh has already been deleted.
	*/
}

// Getters
Mesh* GameEntity::GetMesh() { return entityMesh; }

Transform* GameEntity::GetTransform() 
{
	/* 
		Note that, even though you're probably not storing the Transform as a pointer, you might 
		still want to return a pointer (the address of the transform field). This is one way to 
		ensure that when accessing the transform from outside the class, you're changing the 
		entity's actual transform and not a copy of it.
	*/
	return &entityTransform; 
}

//void GameEntity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer)
//{
//}