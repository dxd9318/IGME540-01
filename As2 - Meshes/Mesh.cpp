#include "Mesh.h"

// For the DirectX Math library
using namespace DirectX;

/*
	Currently the starter code defines geometry and creates the appropriate buffers in the CreateBasicGeometry() method. 
	It also sets the buffers during the draw step, then actually draws with them in Draw().  
	
	Your job is to generalize these steps by moving them to a class specifically designed to create and keep track of the geometry.

	You have two options when architecting a class like this: 
		>> 1. This class will know how to create the requisite buffers, but other than the constructor it’s just a dumb container for that data. 
		It will have a few private variables and some “get” methods.
		
		2. The class will know how to both create and use the buffers.  It will keep track of them with private variables and will have methods 
		for activating the buffers and drawing with them.

*/

/*
	Include the following private variables and functions at a minimum:
		//Two ID3D11Buffer pointers, one for the vertex buffer and one for the index buffer of this mesh
			// >> Or two Microsoft::WRL::ComPtr<ID3D11Buffer> if you want to use smart pointers
		
		//An integer specifying how many indices are in the mesh’s index buffer
			//This is used when drawing
		
		A constructor that creates the two buffers from the appropriate arrays. 
		You should copy, paste and adjust the code from the CreateBasicGeometry() method 
		as necessary. The constructor will need the following parameters:
			//An array of Vertex objects. (If you haven’t used C++ in a while, remember that arrays and pointers are kind of the same thing, so this can 
			//just be a pointer to a Vertex)
			
			//An integer specifying the number of vertices in the vertex array
			
			//An array of unsigned integers for the indices (or a pointer to a single unsigned integer)
			
			//An integer specifying the number of indices in the index array
			
			//A pointer (regular or smart) to an ID3D11Device object, used for creating buffers

		If not using smart pointers, ensure you have a destructor which calls the Release() function on both buffers. Properly cleaning up DirectX objects is 
		your responsibility. Every time you create something, you must release it. Remember that smart pointers can do this for you.
		
		//A GetVertexBuffer() method to return the pointer to the vertex buffer object
		
		//A GetIndexBuffer() method, which does the same thing for the index buffer
		
		//A GetIndexCount() method, which returns the number of indices this mesh contains.
			Remember that we need this information whenever we draw anything in DirectX.
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

Mesh::~Mesh() {}

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetVertexBuffer() 
{
	return vertexBuffer;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetIndexBuffer() 
{
	return indexBuffer;
}

int Mesh::GetIndexCount() 
{
	return indexCount;
}