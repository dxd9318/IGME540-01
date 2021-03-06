#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <vector>
#include "GameEntity.h"
//#include "Mesh.h"	// Geometry data will be held in the mesh class, so Game will need to pull from it
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
	/*
		The following code and functionality was moved to the Mesh class.

		//void CreateBasicGeometry();

		//// Note the usage of ComPtr below
		////  - This is a smart pointer for objects that abide by the
		////    Component Object Mode, which DirectX objects do
		////  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

		//// Buffers to hold actual geometry data
		//Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		//Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	*/

	// Mesh objects
	Mesh* triangleMesh = nullptr;
	Mesh* rectangleMesh = nullptr;
	Mesh* circleMesh = nullptr;

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

