#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <vector>
#include "Lights.h"
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

	// Light Objects
	DirectionalLight dirLight_01;
	DirectionalLight dirLight_02;
	DirectionalLight dirLight_03;

	PointLight ptLight_01;

	// Camera Objects
	Camera* camera = nullptr;

	// Material Objects
	Material* mat1 = nullptr;	// red
	Material* mat2 = nullptr;	// green
	Material* mat3 = nullptr;	// blue

	Material* mat4 = nullptr;	// cyan
	Material* mat5 = nullptr;	// yellow
	Material* mat6 = nullptr;	// magenta

	Material* mat7 = nullptr;	// white

	// Mesh Objects - Hardcoded
	Mesh* triangleMesh = nullptr;
	Mesh* rectangleMesh = nullptr;
	Mesh* circleMesh = nullptr;

	// Mesh Objects - File loaded
	Mesh* coneMesh = nullptr;
	Mesh* cubeMesh = nullptr;
	Mesh* cylinderMesh = nullptr;
	Mesh* helixMesh = nullptr;
	Mesh* sphereMesh = nullptr;
	Mesh* torusMesh = nullptr;

	// Entity Objects
	GameEntity* triangleEntity_01 = nullptr;
	GameEntity* squareEntity_01 = nullptr;
	GameEntity* circleEntity_01 = nullptr;

	GameEntity* coneEntity_01 = nullptr;
	GameEntity* cubeEntity_01 = nullptr;
	GameEntity* cylinderEntity_01 = nullptr;
	GameEntity* helixEntity_01 = nullptr;
	GameEntity* sphereEntity_01 = nullptr;
	GameEntity* torusEntity_01 = nullptr;

	std::vector<GameEntity*> entityVector = {};
};

