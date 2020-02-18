#pragma once

#include <d3d11.h>	// For accessing DirectX elements (ie. ID3DLLBuffer)
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include "Vertex.h"

/*
	A class specifically designed to create and keep track of geometry.
	Creates the requisite vertex and index buffers, but other than the constructor it’s just a dumb container for that data.
*/
class Mesh 
{
public:
	// Constructor
	Mesh(Vertex* vertArray, int vertsInArray, int* indxArray, int indxsInArray, Microsoft::WRL::ComPtr<ID3D11Device> deviceObj);

	//Destructor
	~Mesh();

	// Getters for private members
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();

private:
	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	int indexCount = 0;
};