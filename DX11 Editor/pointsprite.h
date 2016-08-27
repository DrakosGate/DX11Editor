//
//  File Name   :   pointsprite.h
//  Description :   Class of PointSprite
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


#pragma once

#ifndef __POINTSPRITE_H__
#define __POINTSPRITE_H__

// Library Includes
#include <vector>

// Local Includes
#include "model.h"

// Types

// Constants

// Prototypes

class PointSprite : public Model
{
	//Member functions
public:
	PointSprite();
	virtual ~PointSprite();

	virtual bool Initialise();
	virtual void AddPointSprite( ID3D11Device* _pDevice, Math::Vector3& _rPosition, Math::Vector3& _rDirection, Math::Vector2& _rScale, Math::Colour& _rColour, float _fRotation, int _iTextureID );
	virtual void Draw( ID3D11DeviceContext* _pDevice );

	virtual void CreateVertexBuffer( ID3D11Device* _pDevice );
	virtual void RefreshBuffers( ID3D11Device* _pDevice );

	TPointSpriteVertex* GetPointSprite( int _iIndex );

private:
	PointSprite( const PointSprite& _krInstanceToCopy );
	const PointSprite& operator =( const PointSprite& _krInstanceToCopy );

	//Member variables
protected:
	std::vector< TPointSpriteVertex* > m_pPointSpriteVertices;
	TPointSpriteVertex* m_pverts;
};

#endif //__POINTSPRITE_H__