#include "Mesh.h"

using namespace DirectX; // For the DirectX Math library

/*
	A constructor that creates vertex and index buffers from the appropriate arrays. Code pulled from the CreateBasicGeometry() method in Game.cpp.
	Parameters:
		- An array of Vertex objects
		- An integer specifying the number of vertices in the vertex array
		- An array of unsigned integers for the indices (or a pointer to a single unsigned integer)
		- An integer specifying the number of indices in the index array
		- A pointer (regular or smart) to an ID3D11Device object, used for creating buffers
*/
Mesh::Mesh(Vertex* vertArray, int vertsInArray, int* indxArray, int indxsInArray, Microsoft::WRL::ComPtr<ID3D11Device> deviceObj)
{
	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertsInArray;       // vertsInArray = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertArray;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	deviceObj->CreateBuffer(&vbd, &initialVertexData, vertexBuffer.GetAddressOf());



	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * indxsInArray;         // indxsInArray = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indxArray;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	deviceObj->CreateBuffer(&ibd, &initialIndexData, indexBuffer.GetAddressOf());

	indexCount = indxsInArray;
}

/*
	Buffers in this class are allocated using COMPtrs, a form of smart pointer. This means memory is already auto-released for us.
*/
Mesh::~Mesh() {}

/*
	Returns the pointer to the vertex buffer object (VBO).
*/
Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetVertexBuffer() { return vertexBuffer; }

/*
	Returns the pointer to the index buffer object (IBO).
*/
Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetIndexBuffer() { return indexBuffer; }

/*
	Returns the number of indices in this mesh's index buffer. 
	Remember that we need this information whenever we draw anything in DirectX.
*/
int Mesh::GetIndexCount() { return indexCount; }