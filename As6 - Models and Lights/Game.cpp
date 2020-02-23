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

	delete squareEntity_02;
	squareEntity_02 = nullptr;

	delete circleEntity_01;
	circleEntity_01 = nullptr;

	delete circleEntity_02;
	circleEntity_02 = nullptr;

	entityVector.clear();

	// RELEASE MESH POINTERS HERE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	delete triangleMesh;
	triangleMesh = nullptr;

	delete rectangleMesh;
	rectangleMesh = nullptr;

	delete circleMesh;
	circleMesh = nullptr;

	// RELEASE MATERIAL POINTERS HERE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	delete mat1;
	mat1 = nullptr;

	delete mat2;
	mat2 = nullptr;

	delete mat3;
	mat3 = nullptr;

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
	// Init Camera
	camera = new Camera(
		DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f),	// camera start position
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.01f),	// camera start orientation
		(float)(this->width / this->height),	// aspect ratio
		DirectX::XM_PIDIV4,						// field of view
		0.01f,		// near plane distance
		1000.0f,	// far plane distance
		0.05f,		// move speed
		0.5f);		// mouse look speed

	// --------------------------------------------------------------------------------------
	// Initialize Material pointer objects
	mat1 = new Material(vertexShader, pixelShader, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f));
	mat2 = new Material(vertexShader, pixelShader, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));
	mat3 = new Material(vertexShader, pixelShader, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f));

	// --------------------------------------------------------------------------------------
	// Initialize Mesh pointer objects
	// Create some temporary variables to represent colors
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Mesh object 1 - Basic Triangle	// REMEMBER, CLOCKWISE!!!
	Vertex triangleVertices[] =
	{
		{ XMFLOAT3(-0.75f, +1.0f, +0.0f), red },
		{ XMFLOAT3(-0.5f, +0.5f, +0.0f), blue },
		{ XMFLOAT3(-1.0f, +0.5f, +0.0f), green },
	};
	int triangleIndices[] = { 0, 1, 2 };
	triangleMesh = new Mesh(triangleVertices, 3, triangleIndices, 3, device);

	// Mesh object 2 - Rectangle
	Vertex rectangleVertices[] =
	{
		{ XMFLOAT3(+0.5f, +1.0f, +0.0f), red },
		{ XMFLOAT3(+1.0f, +1.0f, +0.0f), blue },
		{ XMFLOAT3(+1.0f, +0.5f, +0.0f), red },
		{ XMFLOAT3(+0.5f, +0.5f, +0.0f), green },
	};
	int rectangleIndices[] = { 0, 1, 2, 0, 2, 3 };	//0, 1, 2	//0, 2, 3
	rectangleMesh = new Mesh(rectangleVertices, 4, rectangleIndices, 6, device);

	// Mesh object 3 - Circle (12 triangles, 13 vertices)
	// variables for some trig values for repeated use, and readability
	float cosPiOverThree = XMScalarCos(XM_PI / 3);	// Value = (1/2)
	float sinPiOverThree = XMScalarSin(XM_PI / 3);	// Value = (sqrt(3)/2)

	Vertex circleVertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.0f, +0.0f), green },	// 0 - Center
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f), red },		// 1 - Top
		{ XMFLOAT3(cosPiOverThree * 0.5f, sinPiOverThree * 0.5f, +0.0f), blue },	// pi/3
		{ XMFLOAT3(sinPiOverThree * 0.5f, cosPiOverThree * 0.5f, +0.0f), red },		// pi/6
		{ XMFLOAT3(+0.5f, +0.0f, +0.0f), blue },	// 4 - Right
		{ XMFLOAT3(sinPiOverThree * 0.5f, cosPiOverThree * -0.5f, +0.0f), red },
		{ XMFLOAT3(cosPiOverThree * 0.5f, sinPiOverThree * -0.5f, +0.0f), blue },
		{ XMFLOAT3(+0.0f, -0.5f, +0.0f), red },		// 7 - Bottom
		{ XMFLOAT3(cosPiOverThree * -0.5f, sinPiOverThree * -0.5f, +0.0f), blue },
		{ XMFLOAT3(sinPiOverThree * -0.5f, cosPiOverThree * -0.5f, +0.0f), red },
		{ XMFLOAT3(-0.5f, +0.0f, +0.0f), blue },	// 10 - Left
		{ XMFLOAT3(sinPiOverThree * -0.5f, cosPiOverThree * 0.5f, +0.0f), red },
		{ XMFLOAT3(cosPiOverThree * -0.5f, sinPiOverThree * 0.5f, +0.0f), blue },

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

	// --------------------------------------------------------------------------------------
	// Create and store entities
	triangleEntity_01 = new GameEntity(triangleMesh, mat1);
	squareEntity_01 = new GameEntity(rectangleMesh, mat2);
	squareEntity_02 = new GameEntity(rectangleMesh, mat3);
	circleEntity_01 = new GameEntity(circleMesh, mat1);
	circleEntity_02 = new GameEntity(circleMesh, mat2);

	entityVector.push_back(triangleEntity_01);
	entityVector.push_back(squareEntity_01);
	entityVector.push_back(squareEntity_02);
	entityVector.push_back(circleEntity_01);
	entityVector.push_back(circleEntity_02);

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

	// triangle 1
	entityVector[0]->GetTransform()->SetScale(0.5f, 0.75f, 1.0f);

	// rectangle 1
	entityVector[1]->GetTransform()->SetPosition(0.0f, -1.0f, 1.0f);
	entityVector[1]->GetTransform()->SetRotation(0.0f, 0.0f, XM_PIDIV4);
	entityVector[1]->GetTransform()->SetScale(0.25f, 0.25f, 1.0f);

	// rect 2
	static float decrementX = 0;
	entityVector[2]->GetTransform()->SetPosition(decrementX, 0.0f, 2.0f);
	decrementX -= 0.000005f;
	entityVector[2]->GetTransform()->SetScale(1.0f, 0.5f, 1.0f);

	// circle 1
	entityVector[3]->GetTransform()->SetPosition(0.5f, -0.75f, 3.0f);
	entityVector[3]->GetTransform()->SetScale(0.15f, 0.15f, 1.0f);
	static float rotateAngle = 0;
	entityVector[3]->GetTransform()->SetRotation(0.0f, 0.0f, rotateAngle);
	rotateAngle += XM_PI / 20000;

	// circle 2 
	float scaleChange = sin(rotateAngle) / 10;
	entityVector[4]->GetTransform()->SetPosition(0.75f, 0.75f, 4.0f);
	entityVector[4]->GetTransform()->SetScale(scaleChange, scaleChange, 1.0f);

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