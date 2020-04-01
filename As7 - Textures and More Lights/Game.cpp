#include "Game.h"
#include "Vertex.h"

// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// RELEASE GAMEENTITIES HERE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	delete triangleEntity_01;
	triangleEntity_01 = nullptr;

	delete squareEntity_01;
	squareEntity_01 = nullptr;

	delete circleEntity_01;
	circleEntity_01 = nullptr;


	delete coneEntity_01;
	coneEntity_01 = nullptr;

	delete cubeEntity_01;
	cubeEntity_01 = nullptr;

	delete cylinderEntity_01;
	cylinderEntity_01 = nullptr;

	delete helixEntity_01;
	helixEntity_01 = nullptr;

	delete sphereEntity_01;
	sphereEntity_01 = nullptr;

	delete torusEntity_01;
	torusEntity_01 = nullptr;

	entityVector.clear();

	// RELEASE MESH POINTERS HERE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	delete triangleMesh;
	triangleMesh = nullptr;

	delete rectangleMesh;
	rectangleMesh = nullptr;

	delete circleMesh;
	circleMesh = nullptr;


	delete coneMesh;
	coneMesh = nullptr;

	delete cubeMesh;
	cubeMesh = nullptr;

	delete cylinderMesh;
	cylinderMesh = nullptr;

	delete helixMesh;
	helixMesh = nullptr;
	
	delete sphereMesh;
	sphereMesh = nullptr;

	delete torusMesh;
	torusMesh = nullptr;

	// RELEASE MATERIAL POINTERS HERE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	delete mat1;
	mat1 = nullptr;

	delete mat2;
	mat2 = nullptr;

	delete mat3;
	mat3 = nullptr;

	delete mat4;
	mat4 = nullptr;

	delete mat5;
	mat5 = nullptr;

	delete mat6;
	mat6 = nullptr;

	delete mat7;
	mat7 = nullptr;

	// RELEASE CAMERA HERE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	delete camera;
	camera = nullptr;

	// RELEASE SHADERS HERE  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	delete vertexShader;
	vertexShader = nullptr;

	delete pixelShader;
	pixelShader = nullptr;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();

	// --------------------------------------------------------------------------------------
	// Init Directional Lights
	// Yellow light, points towards camera startPos from behind geometry
	dirLight_01.ambientColor = DirectX::XMFLOAT3(0.1f, 0.1f, 0.2f);
	dirLight_01.diffuseColor = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
	dirLight_01.direction = DirectX::XMFLOAT3(0.0f, 0.0f, -3.0f);

	// Cyan light, points down from above geometry
	dirLight_02.ambientColor = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	dirLight_02.diffuseColor = DirectX::XMFLOAT3(0.0f, 0.5f, 1.0f);
	dirLight_02.direction = DirectX::XMFLOAT3(0.0f, -3.0f, 0.0f);

	// Pinkish light, points up from below geometry
	dirLight_03.ambientColor = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	dirLight_03.diffuseColor = DirectX::XMFLOAT3(0.6f, 0.3f, 0.3f);
	dirLight_03.direction = DirectX::XMFLOAT3(0.0f, 2.0f, 0.0f);

	// Init Point Lights
	// Green light, points right from left of geometry
	ptLight_01.ambientColor = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	ptLight_01.diffuseColor = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	ptLight_01.position = DirectX::XMFLOAT3(-3.0f, 0.0f, 0.0f);

	// --------------------------------------------------------------------------------------
	// Init Camera
	camera = new Camera(
		DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f),	// camera start position
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.01f),	// camera start orientation
		(float)(this->width / this->height),	// aspect ratio
		DirectX::XM_PIDIV4,						// field of view
		0.01f,		// near plane distance
		1000.0f,	// far plane distance
		0.05f,		// move speed
		2.0f);		// mouse look speed

	// --------------------------------------------------------------------------------------
	// Load textures
	DirectX::CreateWICTextureFromFile(
		device.Get(), 
		context.Get(), 
		GetFullPathTo_Wide(L"../../Assets/Textures/bamboo.jpg").c_str(), 
		0, 
		srv_01.GetAddressOf());

	DirectX::CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../Assets/Textures/ground-plants.jpg").c_str(),
		0,
		srv_02.GetAddressOf());

	DirectX::CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../Assets/Textures/wrinkled-fabric.jpg").c_str(),
		0,
		srv_03.GetAddressOf());

	// Init sampler state
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&sampDesc, samplerState.GetAddressOf());

	// --------------------------------------------------------------------------------------
	// Initialize Material pointer objects
	mat1 = new Material(vertexShader, pixelShader, srv_02, samplerState, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f), 1.0f);	// red
	mat2 = new Material(vertexShader, pixelShader, srv_02, samplerState, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f), 0.7f);	// green
	mat3 = new Material(vertexShader, pixelShader, srv_02, samplerState, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f), 0.5f);	// blue

	mat4 = new Material(vertexShader, pixelShader, srv_01, samplerState, DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f), 0.0f);	// cyan
	mat5 = new Material(vertexShader, pixelShader, srv_01, samplerState, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), 1.0f);	// yellow
	mat6 = new Material(vertexShader, pixelShader, srv_01, samplerState, DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 0.0f), 1.0f);	// magenta

	mat7 = new Material(vertexShader, pixelShader, srv_03, samplerState, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f), 1.0f);	// white
	
	// --------------------------------------------------------------------------------------
	// Initialize Mesh pointer objects
	// Create some temporary variables to represent colors
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Mesh object 1 - Basic Triangle	// REMEMBER, CLOCKWISE!!!
	Vertex triangleVertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.25f, -0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.25f, -0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
	};
	int triangleIndices[] = { 0, 1, 2 };
	triangleMesh = new Mesh(triangleVertices, 3, triangleIndices, 3, device);

	// Mesh object 2 - Rectangle
	Vertex rectangleVertices[] =
	{
		{ XMFLOAT3(-0.25f, +0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.25f, +0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.25f, -0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.25f, -0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
	};
	int rectangleIndices[] = { 0, 1, 2, 0, 2, 3 };	//0, 1, 2	//0, 2, 3
	rectangleMesh = new Mesh(rectangleVertices, 4, rectangleIndices, 6, device);

	// Mesh object 3 - Circle (12 triangles, 13 vertices)
	// variables for some trig values for repeated use, and readability
	float cosPiOverThree = XMScalarCos(XM_PI / 3);	// Value = (1/2)
	float sinPiOverThree = XMScalarSin(XM_PI / 3);	// Value = (sqrt(3)/2)

	Vertex circleVertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },										// 0 - Center
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },										// 1 - Top
		{ XMFLOAT3(cosPiOverThree * 0.5f, sinPiOverThree * 0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },		// pi/3
		{ XMFLOAT3(sinPiOverThree * 0.5f, cosPiOverThree * 0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },		// pi/6
		{ XMFLOAT3(+0.5f, +0.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },										// 4 - Right
		{ XMFLOAT3(sinPiOverThree * 0.5f, cosPiOverThree * -0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(cosPiOverThree * 0.5f, sinPiOverThree * -0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.0f, -0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },										// 7 - Bottom
		{ XMFLOAT3(cosPiOverThree * -0.5f, sinPiOverThree * -0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(sinPiOverThree * -0.5f, cosPiOverThree * -0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.5f, +0.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },										// 10 - Left
		{ XMFLOAT3(sinPiOverThree * -0.5f, cosPiOverThree * 0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(cosPiOverThree * -0.5f, sinPiOverThree * 0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },

		/*
			X = cosine of angle
			Y = sine of angle
			r = 0.5;

			(2*pi*r)/12 -> pi*r/6
			increment slices by pi*r/6

			//https://docs.microsoft.com/en-us/windows/win32/dxmath/ovw-xnamath-reference-functions-scalar

			XMScalarSin - computes sine of radian angle
			XMScalarCos - computes the cosine of radian angle
			XM_PI - Pi
			
		*/
	};
	int circleIndices[] = { 
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 8,
		0, 8, 9,
		0, 9, 10,
		0, 10, 11,
		0, 11, 12,
		0, 12, 1,
	};
	circleMesh = new Mesh(circleVertices, 13, circleIndices, 36, device);

	coneMesh = new Mesh(GetFullPathTo("../../Assets/Models/cone.obj").c_str(), device);
	cubeMesh = new Mesh(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device);
	cylinderMesh = new Mesh(GetFullPathTo("../../Assets/Models/cylinder.obj").c_str(), device);
	helixMesh = new Mesh(GetFullPathTo("../../Assets/Models/helix.obj").c_str(), device);
	sphereMesh = new Mesh(GetFullPathTo("../../Assets/Models/sphere.obj").c_str(), device);
	torusMesh = new Mesh(GetFullPathTo("../../Assets/Models/torus.obj").c_str(), device);

	// --------------------------------------------------------------------------------------
	// Create and store entities
	triangleEntity_01 = new GameEntity(triangleMesh, mat1);
	squareEntity_01 = new GameEntity(rectangleMesh, mat2);
	circleEntity_01 = new GameEntity(circleMesh, mat3);

	coneEntity_01 = new GameEntity(coneMesh, mat4);
	cubeEntity_01 = new GameEntity(cubeMesh, mat7);
	cylinderEntity_01 = new GameEntity(cylinderMesh, mat5);
	helixEntity_01 = new GameEntity(helixMesh, mat7);
	sphereEntity_01 = new GameEntity(sphereMesh, mat6);
	torusEntity_01 = new GameEntity(torusMesh, mat7);

	entityVector.push_back(triangleEntity_01);
	entityVector.push_back(squareEntity_01);
	entityVector.push_back(circleEntity_01);
	
	entityVector.push_back(coneEntity_01);
	entityVector.push_back(cubeEntity_01);
	entityVector.push_back(cylinderEntity_01);
	entityVector.push_back(helixEntity_01);
	entityVector.push_back(sphereEntity_01);
	entityVector.push_back(torusEntity_01);

	// --------------------------------------------------------------------------------------
	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device.Get(), context.Get(),
		GetFullPathTo_Wide(L"VertexShader.cso").c_str());
	pixelShader = new SimplePixelShader(device.Get(), context.Get(),
		GetFullPathTo_Wide(L"PixelShader.cso").c_str());
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// if camera has been initialized, update its projection matrix using window's current aspect ratio
	if (camera != nullptr) 
	{
		camera->UpdateProjectionMatrix((float)(this->width / this->height));
	}
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	// triangle 1	// rotating
	static float rotateAngle = 0;
	entityVector[0]->GetTransform()->SetPosition(-1.5f, 1.25f, 0.1f);
	entityVector[0]->GetTransform()->SetScale(0.75f, 0.75f, 0.75f);
	entityVector[0]->GetTransform()->SetRotation(0.0f, 0.0f, rotateAngle);
	rotateAngle += XM_PI / 20000;

	// rectangle 1	// shifting
	entityVector[1]->GetTransform()->SetPosition(-1.5f + (cos(rotateAngle) / 7.5f), 0.0f, 0.2f);
	entityVector[1]->GetTransform()->SetScale(0.75f, 0.75f, 1.0f);

	// circle 1		// pulsating
	float scaleChange = sin(rotateAngle) / 2.5f;
	entityVector[2]->GetTransform()->SetPosition(-1.5f, -1.25f, 0.3f);
	entityVector[2]->GetTransform()->SetScale(scaleChange, scaleChange, 1.0f);


	// Cone 1
	entityVector[3]->GetTransform()->SetPosition(-0.5f, 1.0f, 0.5f);
	entityVector[3]->GetTransform()->SetScale(0.5f, 0.5f, 0.5f);

	// Cube 1
	entityVector[4]->GetTransform()->SetPosition(0.25f, 1.0f, 0.5f);
	entityVector[4]->GetTransform()->SetScale(0.5f, 0.5f, 0.5f);

	// Cylinder 1
	entityVector[5]->GetTransform()->SetPosition(1.0f, 1.0f, 0.5f);
	entityVector[5]->GetTransform()->SetScale(0.5f, 0.5f, 0.5f);

	// Helix 1
	entityVector[6]->GetTransform()->SetPosition(-0.5f, -1.0f, 0.5f);
	entityVector[6]->GetTransform()->SetScale(0.25f, 0.25f, 0.25f);

	// Sphere 1
	entityVector[7]->GetTransform()->SetPosition(0.25f, -1.0f, 0.5f);
	entityVector[7]->GetTransform()->SetScale(0.5f, 0.5f, 0.5f);

	// Torus 1
	entityVector[8]->GetTransform()->SetPosition(1.0f, -1.0f, 0.5f);
	entityVector[8]->GetTransform()->SetScale(0.5f, 0.5f, 0.5f);
	entityVector[8]->GetTransform()->SetRotation(-XM_PIDIV4, 0.0f, 0.0f);

	camera->Update(deltaTime, this->hWnd);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (int i = 0; i < entityVector.size(); i++) 
	{
		context->IASetVertexBuffers(0, 1, entityVector[i]->GetMesh()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(entityVector[i]->GetMesh()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		// Set the vertex and pixel shaders to use for the next Draw() command
			//  - These don't technically need to be set every frame
			//  - Once you start applying different shaders to different objects,
			//    you'll need to swap the current shaders before each draw
		entityVector[i]->GetMaterial()->GetVertexShader()->SetShader();
		entityVector[i]->GetMaterial()->GetPixelShader()->SetShader();

		// Assigning data to Constant Buffer for Vertex Shader
		SimpleVertexShader* vs = entityVector[i]->GetMaterial()->GetVertexShader();
		vs->SetFloat4("colorTint", entityVector[i]->GetMaterial()->GetMaterialColorTint());
		vs->SetMatrix4x4("world", entityVector[i]->GetTransform()->GetWorldMatrix());
		vs->SetMatrix4x4("view", camera->GetViewMatrix());
		vs->SetMatrix4x4("projection", camera->GetProjectionMatrix());

		vs->CopyAllBufferData();

		pixelShader->SetSamplerState("samplerOptions", entityVector[i]->GetMaterial()->GetSamplerState().Get());
		pixelShader->SetShaderResourceView("diffuseTexture", entityVector[i]->GetMaterial()->GetSRV().Get());

		// Set directional light data in pixel shader
		pixelShader->SetData(
			"directionalLight1",			// The name of the (eventual) variable in the shader
			&dirLight_01,					// The address of the data to set
			sizeof(DirectionalLight));		// The size of data to set

		pixelShader->SetData(
			"directionalLight2",			// The name of the (eventual) variable in the shader
			&dirLight_02,					// The address of the data to set
			sizeof(DirectionalLight));		// The size of data to set

		pixelShader->SetData(
			"directionalLight3",			// The name of the (eventual) variable in the shader
			&dirLight_03,					// The address of the data to set
			sizeof(DirectionalLight));		// The size of data to set

		// Set point light data in pixel shader
		pixelShader->SetData(
			"pointLight1",				// The name of the (eventual) variable in the shader
			&ptLight_01,				// The address of the data to set
			sizeof(PointLight));		// The size of data to set

		// Values for calculating material specularity
		pixelShader->SetFloat("specularIntensity", entityVector[i]->GetMaterial()->GetMaterialSpecularity());
		pixelShader->SetFloat3("cameraWorldPosition", camera->GetTransform().GetPosition());

		pixelShader->CopyAllBufferData();

		// Finally, do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER

		context->DrawIndexed(
			entityVector[i]->GetMesh()->GetIndexCount(), // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}