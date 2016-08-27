//
//  File Name   :   performancegraph.h
//  Description :   Class of PerformanceGraph
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

class PerformanceGraph : public PointSprite
{
//Member functions
public:
	PerformanceGraph();
	virtual ~PerformanceGraph();

	virtual bool Initialise(ID3D11Device* _pDevice, Math::Vector3& _rPosition, Math::Vector3& _rScale, int _iNumNodes);
	void LogPerformance(char* _pcLogFilename, char* _pcLogDescription, int _iLogAfterFrames);
	void OutputLog();

	void Draw(ID3D11DeviceContext* _pDevice);
	void SetGraphRange(float _fMinRange, float _fMaxRange);
	void AddNode(ID3D11Device* _pDevice, float _fValue);
	float GetMin() const;
	float GetMax() const;

//Member variables
protected:
	Math::Vector3 m_vecGraphPos;
	Math::Vector3 m_vecGraphScale;

	float* m_pNodeValues;
	int m_iNumNodes;

	float m_fMinRange;
	float m_fMaxRange;
	float m_fHeightPercentage;

	float m_fScaleOffset;

	//Performance Log
	std::vector<float> m_performanceLog;
	char* m_pcLogFilename;
	char* m_pcLogDescription;
	bool m_bDoLogPerformance;
	int m_iLogCount;
};

#endif //PERFORMANCEGRAPH_H__