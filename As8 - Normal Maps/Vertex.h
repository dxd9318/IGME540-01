#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT3 Position;	    // The position of the vertex
	//DirectX::XMFLOAT4 Color;        // The color of the vertex
	DirectX::XMFLOAT3 Normal;		// The normal of the vertex
	DirectX::XMFLOAT2 UV;			// The UV coordinates of the vertex
	DirectX::XMFLOAT3 Tangent;		// The tangent vector of the vertex, for normal mapping
};