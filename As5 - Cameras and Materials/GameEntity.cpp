#include "GameEntity.h"

// Constructor
GameEntity::GameEntity(Mesh* inputMesh, Material* inputMat)
{
	entityMesh = inputMesh;
	entityMaterial = inputMat;
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
/*
	Note that, even though you're probably not storing the Transform as a pointer, you might
	still want to return a pointer (the address of the transform field). This is one way to
	ensure that when accessing the transform from outside the class, you're changing the
	entity's actual transform and not a copy of it.
*/
Transform* GameEntity::GetTransform() { return &entityTransform; }

Mesh* GameEntity::GetMesh() { return entityMesh; }

Material* GameEntity::GetMaterial() { return entityMaterial; }