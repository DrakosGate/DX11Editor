//
//  File Name   :   performancegraph.h
//  Description :   Class of CPerformanceGraph
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


#pragma once

#ifndef __PERFORMANCEGRAPH_H__
#define __PERFORMANCEGRAPH_H__

// Library Includes

// Local Includes
#include "pointsprite.h"

// Types

class CPerformanceGraph : public CPointSprite
{
	//Member functions
public:
	CPerformanceGraph();
	virtual ~CPerformanceGraph();

	virtual bool Initialise(ID3D11Device* _pDevice, D3DXVECTOR3& _rPosition, D3DXVECTOR3& _rScale, int _iNumNodes);

	void Draw(ID3D11DeviceContext* _pDevice);
	void SetGraphRange(float _fMinRange, float _fMaxRange);
	void AddNode(ID3D11Device* _pDevice, float _fValue);
	float GetMin() const;
	float GetMax() const;

protected:
	D3DXVECTOR3 m_vecGraphPos;
	D3DXVECTOR3 m_vecGraphScale;

	float* m_pNodeValues;
	int m_iNumNodes;

	float m_fMinRange;
	float m_fMaxRange;
	float m_fHeightPercentage;

	float m_fScaleOffset;
};

#endif //PERFORMANCEGRAPH_H__