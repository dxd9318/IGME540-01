#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <vector>
#include "Camera.h"
#include "Material.h"
#include "GameEntity.h" // Has Mesh.h included

class Game 
	: public DXCore
{

public:
	// Constructor and Destructor
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 

	// Shaders and shader-related constructs
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// Camera Object
	Camera* camera = nullptr;

	// Material Objects
	Material* mat1 = nullptr;
	Material* mat2 = nullptr;
	Material* mat3 = nullptr;

	// Mesh Objects
	Mesh* triangleMesh = nullptr;
	Mesh* rectangleMesh = nullptr;
	Mesh* circleMesh = nullptr;

	// Entity Objects
	GameEntity* triangleEntity_01 = nullptr;
	GameEntity* squareEntity_01 = nullptr;
	GameEntity* squareEntity_02 = nullptr;
	GameEntity* circleEntity_01 = nullptr;
	GameEntity* circleEntity_02 = nullptr;
	std::vector<GameEntity*> entityVector = {};
};

