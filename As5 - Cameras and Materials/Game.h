#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <vector>
#include "Camera.h"
#include "GameEntity.h" // Has Mesh.h included
#include "BufferStructs.h"

class Game 
	: public DXCore
{

public:
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

	// Camera Object
	Camera* camera = nullptr;

	// Mesh objects
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

	// Shaders and shader-related constructs
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer;
};

