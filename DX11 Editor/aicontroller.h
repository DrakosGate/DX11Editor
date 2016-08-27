//
//  File Name   :   AIHiveMind.h
//  Description :   Class of AIHiveMind
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
class RenderEntity;
class AIHiveMind;

class AIController
{
public:
	AIController();
	virtual ~AIController();

	virtual bool Initialise(AIHiveMind* _pHivemind, RenderEntity* _pEntity, float _fMovementSpeed, float _fRotationSpeed);
	void Process(float _fDeltaTime, Math::Vector3& _rAvoidance);
	void ProcessWaypointMovement(float _fDeltaTime);
	void ProcessAStarMovement(int _iPathLength, float _fDeltaTime);
	void SetAStarTarget(Math::Vector3& _rVecTarget);
	void CheckWaypointReached();
	
	Math::Vector3& GetCurrentWaypoint();
	void SetAIType(EAIType _eAIType);
	RenderEntity* GetEntity() const;
	EAIType GetAIType() const;

private:
	AIController(const AIController& _krInstanceToCopy);
	const AIController& operator =(const AIController& _krInstanceToCopy);

private:
	AIHiveMind* m_pHivemind;
	RenderEntity* m_pEntity;
	Math::Vector3 m_vecWaypoint;

	Math::Vector3 m_vecAStarActivePoint;
	float m_fMovementSpeed;
	float m_fRotationSpeed;
	float m_fThoughtDelay;
	int m_iCurrentWaypointIndex;

	EAIType m_eAIType;
	
};

#endif // __AICONTROLLER_H__