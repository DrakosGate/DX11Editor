//
//  File Name   :   boundingbox.h
//  Description :   Class of CBoundingBox
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __BOUNDINGBOX_H__
#define __BOUNDINGBOX_H__

// Library Includes

// Local Includes
#include "Vertex.h"

// Types
struct TBoundingBox
{
	TBoundingBox()
	{
		fLeft = 0.0f;
		fRight = 0.0f;
		fTop = 0.0f;
		fBottom = 0.0f;
	}
	float fLeft;
	float fRight;
	float fTop;
	float fBottom;
};

// Constants

// Prototypes

class CBoundingBox
{
public:
	CBoundingBox();
	virtual ~CBoundingBox();

	virtual bool Initialise(TVertex* _pObjectVertices, int _iVertexCount);	
	virtual bool Initialise(TAnimatedVertex* _pObjectVertices, int _iVertexCount);	
	virtual float GetRadius() const;
	TBoundingBox* GetRect();

private:
	CBoundingBox(const CBoundingBox& _krInstanceToCopy);
	const CBoundingBox& operator =(const CBoundingBox& _krInstanceToCopy);

private:
	Math::Vector3 m_vecPosition;
	Math::Vector3 m_vecScale;
	float m_fRadius;
	TBoundingBox m_tBoundingBox;

};

#endif // __BOUNDINGBOX_H__