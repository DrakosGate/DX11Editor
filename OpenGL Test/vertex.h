//
//  File Name   :   vertex.h
//  Description :   Class of Vertex
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef VERTEX_H
#define VERTEX_H

// Library Includes
#include <D3DX10math.h>

// Local Includes

// Types

// Constants

// Prototypes

struct TVertex
{
	TVertex()
	{
		pos *= 0.0f;
		colour = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		normal *= 0.0f;
		tangent *= 0.0f;
		binormal *= 0.0f;
		texC *= 0.0f;
	}
	TVertex(D3DXVECTOR3& _rPos, D3DXCOLOR& _rColour, D3DXVECTOR3& _rNormal, D3DXVECTOR3& _rTangent, D3DXVECTOR3& _rBiNormal, D3DXVECTOR2& _rTexC)
	{
		pos = _rPos;
		colour = _rColour;
		normal = _rNormal;
		tangent = _rTangent;
		binormal = _rBiNormal;
		texC = _rTexC;
	}
	D3DXVECTOR3 pos;
	D3DXCOLOR colour;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 binormal;
	D3DXVECTOR2 texC;
};
struct TPointSpriteVertex
{
	TPointSpriteVertex()
	{
		pos *= 0.0f;
		dir *= 0.0f;
		scale = D3DXVECTOR2(1.0f, 1.0f);
		colour = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		rotation = 0.0f;
		iTextureID = 0;
	}
	TPointSpriteVertex(D3DXVECTOR3& _rPos, D3DXVECTOR3& _rDirection, D3DXVECTOR2& _rScale, D3DXCOLOR& _rColour, float _fRotation, int _iTextureID)
	{
		pos = _rPos;
		dir = _rDirection;
		scale = _rScale;
		colour = _rColour;
		rotation = _fRotation;
		iTextureID = _iTextureID;
	}
	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir;
	D3DXVECTOR2 scale;
	D3DXCOLOR colour;
	float rotation;
	int iTextureID;
};
struct TAnimatedVertex
{
	TAnimatedVertex()
	{
		pos *= 0.0f;
		normal *= 0.0f;
		tangent *= 0.0f;
		binormal *= 0.0f;
		texC *= 0.0f;
		vecIDs *= 0.0f;
		vecWeights *= 0.0f;
	}
	TAnimatedVertex(D3DXVECTOR3& _rPos, 
					D3DXVECTOR3& _rNormal, 
					D3DXVECTOR3& _rTangent, 
					D3DXVECTOR3& _rBiNormal, 
					D3DXVECTOR2& _rTexC,
					D3DXVECTOR4& _rIDs,
					D3DXVECTOR4& _rWeights)
	{
		pos = _rPos;
		normal = _rNormal;
		tangent = _rTangent;
		binormal = _rBiNormal;
		texC = _rTexC;
		vecIDs = _rIDs;
		vecWeights = _rWeights;
	}
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 binormal;
	D3DXVECTOR2 texC;
	D3DXVECTOR4 vecIDs;
	D3DXVECTOR4 vecWeights;
};

#endif // VERTEX_H