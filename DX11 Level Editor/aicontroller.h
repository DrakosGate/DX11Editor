//
//  File Name   :   AIHiveMind.h
//  Description :   Class of CAIHiveMind
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __AICONTROLLER_H__
#define __AICONTROLLER_H__

// Library Includes

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes
class CRenderEntity;
class CAIHiveMind;

class CAIController
{
public:
	CAIController();
	virtual ~CAIController();

	virtual bool Initialise(CAIHiveMind* _pHivemind, CRenderEntity* _pEntity, float _fMovementSpeed, float _fRotationSpeed);
	void Process(float _fDeltaTime, D3DXVECTOR3& _rAvoidance);
	void ProcessWaypointMovement(float _fDeltaTime);
	void ProcessAStarMovement(int _iPathLength, float _fDeltaTime);
	void SetAStarTarget(D3DXVECTOR3& _rVecTarget);
	void CheckWaypointReached();
	
	D3DXVECTOR3& GetCurrentWaypoint();
	void SetAIType(EAIType _eAIType);
	CRenderEntity* GetEntity() const;
	EAIType GetAIType() const;

private:
	CAIController(const CAIController& _krInstanceToCopy);
	const CAIController& operator =(const CAIController& _krInstanceToCopy);

private:
	CAIHiveMind* m_pHivemind;
	CRenderEntity* m_pEntity;
	D3DXVECTOR3 m_vecWaypoint;

	D3DXVECTOR3 m_vecAStarActivePoint;
	float m_fMovementSpeed;
	float m_fRotationSpeed;
	float m_fThoughtDelay;
	int m_iCurrentWaypointIndex;

	EAIType m_eAIType;
	
};

#endif // __AICONTROLLER_H__