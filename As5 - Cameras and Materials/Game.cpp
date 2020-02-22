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
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object

	// RELEASE CAMERA HERE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	delete camera;
	camera = nullptr;

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
	mat1 = new Material(pixelShader, vertexShader, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f));
	mat2 = new Material(pixelShader, vertexShader, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));
	mat3 = new Material(pixelShader, vertexShader, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f));

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

	// --------------------------------------------------------------------------------------
	// Initialize constant buffers
	// Get size of cbuffer as the next multiple of 16
	unsigned int size = sizeof(VertexShaderExternalData);
	size = (size + 15) / 16 * 16;

	// Describe the constant buffer
	D3D11_BUFFER_DESC cbDesc	= {}; // Sets struct to all zeros
	cbDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth			= size; // Must be a multiple of 16
	cbDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cbDesc.Usage				= D3D11_USAGE_DYNAMIC;

	// Feed D3D the desc and buffer location to actually create the buffer
	device->CreateBuffer(&cbDesc, 0, &vsConstantBuffer);
	// --------------------------------------------------------------------------------------
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
	// Blob for reading raw data
	// - This is a simplified way of handling raw data
	ID3DBlob* shaderBlob;

	// Read our compiled vertex shader code into a blob
	// - Essentially just "open the file and plop its contents here"
	D3DReadFileToBlob(
		GetFullPathTo_Wide(L"VertexShader.cso").c_str(), // Using a custom helper for file paths
		&shaderBlob);

	// Create a vertex shader from the information we
	// have read into the blob above
	// - A blob can give a pointer to its contents, and knows its own size
	device->CreateVertexShader(
		shaderBlob->GetBufferPointer(), // Get a pointer to the blob's contents
		shaderBlob->GetBufferSize(),	// How big is that data?
		0,								// No classes in this shader
		vertexShader.GetAddressOf());	// The address of the ID3D11VertexShader*


	// Create an input layout that describes the vertex format
	// used by the vertex shader we're using
	//  - This is used by the pipeline to know how to interpret the raw data
	//     sitting inside a vertex buffer
	//  - Doing this NOW because it requires a vertex shader's byte code to verify against!
	//  - Luckily, we already have that loaded (the blob above)
	D3D11_INPUT_ELEMENT_DESC inputElements[2] = {};

	// Set up the first element - a position, which is 3 float values
	inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// Most formats are described as color channels; really it just means "Three 32-bit floats"
	inputElements[0].SemanticName = "POSITION";							// This is "POSITION" - needs to match the semantics in our vertex shader input!
	inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// How far into the vertex is this?  Assume it's after the previous element

	// Set up the second element - a color, which is 4 more float values
	inputElements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;			// 4x 32-bit floats
	inputElements[1].SemanticName = "COLOR";							// Match our vertex shader input!
	inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// After the previous element

	// Create the input layout, verifying our description against actual shader code
	device->CreateInputLayout(
		inputElements,					// An array of descriptions
		2,								// How many elements in that array
		shaderBlob->GetBufferPointer(),	// Pointer to the code of a shader that uses this layout
		shaderBlob->GetBufferSize(),	// Size of the shader code that uses this layout
		inputLayout.GetAddressOf());	// Address of the resulting ID3D11InputLayout*



	// Read and create the pixel shader
	//  - Reusing the same blob here, since we're done with the vert shader code
	D3DReadFileToBlob(
		GetFullPathTo_Wide(L"PixelShader.cso").c_str(), // Using a custom helper for file paths
		&shaderBlob);

	device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		0,
		pixelShader.GetAddressOf());
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

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	// MOVED TO DRAW LOOP BELOW
	//context->VSSetShader(vertexShader.Get(), 0, 0);
	//context->PSSetShader(pixelShader.Get(), 0, 0);

	// Ensure the pipeline knows how to interpret the data (numbers)
	// from the vertex buffer.  
	// - If all of your 3D models use the exact same vertex layout,
	//    this could simply be done once in Init()
	// - However, this isn't always the case (but might be for this course)
	context->IASetInputLayout(inputLayout.Get());

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	//  - for this demo, this step *could* simply be done once during Init(),
	//    but I'm doing it here because it's often done multiple times per frame
	//    in a larger application/game
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	VertexShaderExternalData vsData; // For assigning data to cbuffer for VS in the following loop

	for (int i = 0; i < entityVector.size(); i++) 
	{
		context->IASetVertexBuffers(0, 1, entityVector[i]->GetMesh()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(entityVector[i]->GetMesh()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		// Assigning data to Constant Buffer for Vertex Shader
		context->VSSetShader(entityVector[i]->GetMaterial()->GetVertexShader().Get(), 0, 0);
		context->PSSetShader(entityVector[i]->GetMaterial()->GetPixelShader().Get(), 0, 0);

		//vsData.colorTint = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
		vsData.colorTint = entityVector[i]->GetMaterial()->GetMaterialColorTint();
		vsData.worldMatrix = entityVector[i]->GetTransform()->GetWorldMatrix();
		vsData.viewMatrix = camera->GetViewMatrix();
		vsData.projectionMatrix = camera->GetProjectionMatrix();

		D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
		context->Map(vsConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);

		memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));

		context->Unmap(vsConstantBuffer.Get(), 0);

		context->VSSetConstantBuffers(
			0,		// Which slot (register) to bind the buffer to?
			1,		// How many are we activating? Can do multiple at once
			vsConstantBuffer.GetAddressOf()	// Array of buffers (or the address of one)
		);

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