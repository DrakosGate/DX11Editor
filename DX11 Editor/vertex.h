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
#include "mathlibrary.h"

// Local Includes

// Types

// Constants

// Prototypes

struct TVertex
{
	struct SourceType
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 colour;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 binormal;
		DirectX::XMFLOAT2 texC;
	};

	TVertex()
	{
		pos *= 0.0f;
		colour = Math::Colour( 1.0f, 1.0f, 1.0f, 1.0f );
		normal *= 0.0f;
		tangent *= 0.0f;
		binormal *= 0.0f;
		texC *= 0.0f;
	}
	TVertex( Math::Vector3& _rPos, Math::Colour& _rColour, Math::Vector3& _rNormal, Math::Vector3& _rTangent, Math::Vector3& _rBiNormal, Math::Vector2& _rTexC )
	{
		pos = _rPos;
		colour = _rColour;
		normal = _rNormal;
		tangent = _rTangent;
		binormal = _rBiNormal;
		texC = _rTexC;
	}

	SourceType GetSourceType()
	{
		SourceType source;
		ZeroMemory( &source, sizeof( SourceType ) );
		source.pos = pos.data;
		source.colour = colour.data;
		source.normal = normal.data;
		source.tangent = tangent.data;
		source.binormal = binormal.data;
		source.texC = texC.data;
		return source;
	}

	Math::Vector3 pos;
	Math::Colour colour;
	Math::Vector3 normal;
	Math::Vector3 tangent;
	Math::Vector3 binormal;
	Math::Vector2 texC;
};
struct TPointSpriteVertex
{
	struct SourceType
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 dir;
		DirectX::XMFLOAT2 scale;
		DirectX::XMFLOAT4 colour;
		float rotation;
		int iTextureID;
	};

	TPointSpriteVertex()
	{
		pos *= 0.0f;
		dir *= 0.0f;
		scale = Math::Vector2( 1.0f, 1.0f );
		colour = Math::Colour( 1.0f, 1.0f, 1.0f, 1.0f );
		rotation = 0.0f;
		iTextureID = 0;
	}
	TPointSpriteVertex( Math::Vector3& _rPos, Math::Vector3& _rDirection, Math::Vector2& _rScale, Math::Colour& _rColour, float _fRotation, int _iTextureID )
	{
		pos = _rPos;
		dir = _rDirection;
		scale = _rScale;
		colour = _rColour;
		rotation = _fRotation;
		iTextureID = _iTextureID;
	}
	
	SourceType GetSourceType()
	{
		SourceType source;
		ZeroMemory( &source, sizeof( SourceType ) );
		source.pos = pos.data;
		source.dir = dir.data;
		source.scale = scale.data;
		source.colour = colour.data;
		source.rotation = rotation;
		source.iTextureID = iTextureID;
		return source;
	}
	
	Math::Vector3 pos;
	Math::Vector3 dir;
	Math::Vector2 scale;
	Math::Colour colour;
	float rotation;
	int iTextureID;
};
struct TFontVertex
{
	struct SourceType
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 scale;
		DirectX::XMFLOAT4 colour;
		DirectX::XMFLOAT2 uvTopLeft;
		DirectX::XMFLOAT2 uvBottomRight;
	};

	TFontVertex()
	{
		pos *= 0.0f;
		scale = Math::Vector2( 1.0f, 1.0f );
		colour = Math::Colour( 1.0f, 1.0f, 1.0f, 1.0f );
		uvTopLeft = Math::Vector2( 0.0f, 0.0f );
		uvBottomRight = Math::Vector2( 0.0f, 0.0f );
	}
	TFontVertex( Math::Vector3& _rPos, Math::Vector2& _rScale, Math::Colour& _rColour, Math::Vector2& _uvTopLeft, Math::Vector2& _uvBottomRight )
	{
		pos = _rPos;
		scale = _rScale;
		colour = _rColour;
		uvTopLeft = _uvTopLeft;
		uvBottomRight = _uvBottomRight;
	}

	SourceType GetSourceType()
	{
		SourceType source;
		ZeroMemory( &source, sizeof( SourceType ) );
		source.pos = pos.data;
		source.scale = scale.data;
		source.colour = colour.data;
		source.uvTopLeft = uvTopLeft.data;
		source.uvBottomRight = uvBottomRight.data;
		return source;
	}

	Math::Vector3 pos;
	Math::Vector2 scale;
	Math::Colour colour;
	Math::Vector2 uvTopLeft;
	Math::Vector2 uvBottomRight;
};
struct TAnimatedVertex
{
	struct SourceType
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 binormal;
		DirectX::XMFLOAT2 texC;
		DirectX::XMFLOAT4 vecIDs;
		DirectX::XMFLOAT4 vecWeights;
	};

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
	TAnimatedVertex( Math::Vector3& _rPos,
		Math::Vector3& _rNormal,
		Math::Vector3& _rTangent,
		Math::Vector3& _rBiNormal,
		Math::Vector2& _rTexC,
		Math::Vector4& _rIDs,
		Math::Vector4& _rWeights )
	{
		pos = _rPos;
		normal = _rNormal;
		tangent = _rTangent;
		binormal = _rBiNormal;
		texC = _rTexC;
		vecIDs = _rIDs;
		vecWeights = _rWeights;
	}

	SourceType GetSourceType()
	{
		SourceType source;
		ZeroMemory( &source, sizeof( SourceType ) );
		source.pos = pos.data;
		source.normal = normal.data;
		source.tangent = tangent.data;
		source.binormal = binormal.data;
		source.texC = texC.data;
		source.vecIDs = vecIDs.data;
		source.vecWeights = vecWeights.data;
		return source;
	}
	
	Math::Vector3 pos;
	Math::Vector3 normal;
	Math::Vector3 tangent;
	Math::Vector3 binormal;
	Math::Vector2 texC;
	Math::Vector4 vecIDs;
	Math::Vector4 vecWeights;
};

#endif // VERTEX_H