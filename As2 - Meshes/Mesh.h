#pragma once

#include <d3d11.h>	// For accessing DirectX elements (ie. ID3DLLBuffer)
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include "Vertex.h"

class Mesh 
{
public:
	// Constructor
	Mesh(Vertex* vertArray, int vertsInArray, int* indxArray, int indxsInArray, Microsoft::WRL::ComPtr<ID3D11Device> deviceObj);

	//Destructor
	~Mesh();

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();

private:	// WILL NEED TO CREATE GETTERS FOR PRIVATE BUFFERS
	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	int indexCount = 0;
};