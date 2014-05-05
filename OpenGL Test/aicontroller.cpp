//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   AIController.cpp
//  Description :   Code for CAIController 
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes
#include "aihivemind.h"
#include "renderentity.h"

// This Include
#include "aicontroller.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CAIController class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CAIController::CAIController()
: m_fMovementSpeed(0.0f)
, m_fRotationSpeed(0.0f)
, m_eAIType(AI_INVALID)
, m_pHivemind(0)
{
	m_vecWaypoint *= 0.0f;
}
/**
*
* CAIController class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CAIController::~CAIController()
{

}
/**
*
* CAIController class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns true
*
*/
bool 
CAIController::Initialise(CAIHiveMind* _pHivemind, CRenderEntity* _pEntity, float _fMovementSpeed, float _fRotationSpeed)
{
	bool bResult = false;

	m_pHivemind = _pHivemind;
	m_pEntity = _pEntity;
	m_fMovementSpeed = _fMovementSpeed;
	m_fRotationSpeed = _fRotationSpeed;

	return true;
}
/**
*
* CAIController class Process
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
void 
CAIController::Process(float _fDeltaTime, D3DXVECTOR3& _rAvoidance)
{
	D3DXVECTOR3 vecCurrentPos = m_pEntity->GetPosition();
	D3DXVECTOR3 vecToWaypoint = m_vecWaypoint - vecCurrentPos;
	D3DXVECTOR3 vecForward = m_pEntity->GetForward();
	vecToWaypoint.y = 0.0f;
	vecForward += vecToWaypoint * m_fRotationSpeed * _fDeltaTime;
	D3DXVec3Normalize(&vecForward, &vecForward);
	//Generate a new waypoint
	float fDistanceToWaypoint = D3DXVec3LengthSq(&vecToWaypoint);
	if(fDistanceToWaypoint < 0.5f)
	{
		m_vecWaypoint = m_pHivemind->GetRandomWaypoint();
		//m_vecWaypoint = GetRandomVector(D3DXVECTOR3(-10.0f, -10.0f, -10.0f), D3DXVECTOR3(10.0f, 10.0f, 10.0f));
	}
	vecCurrentPos += vecForward * m_fMovementSpeed * _fDeltaTime;
	vecCurrentPos += _rAvoidance * m_fMovementSpeed * _fDeltaTime;
	m_pEntity->SetPosition(vecCurrentPos);
	m_pEntity->SetForward(vecForward);
	m_pEntity->SetRotation(D3DXVECTOR3(0.0f, atan2(vecForward.x, vecForward.z), 0.0f));
}
/**
*
* CAIController class GetRandomVector Generates a random vector in the specified range
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
D3DXVECTOR3 
CAIController::GetRandomVector(D3DXVECTOR3& _rMin, D3DXVECTOR3& _rMax)
{
	return (D3DXVECTOR3(	_rMin.x + (rand() % static_cast<int>((_rMax.x - _rMin.x) * 100.0f) * 0.01f), 
							_rMin.y + (rand() % static_cast<int>((_rMax.y - _rMin.y) * 100.0f) * 0.01f),
							_rMin.z + (rand() % static_cast<int>((_rMax.z - _rMin.z) * 100.0f) * 0.01f)) );
}
/**
*
* CAIController class SetAIType Specifies the type of AI used
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CAIController::SetAIType(EAIType _eAIType)
{
	m_eAIType = _eAIType;
}
/**
*
* CAIController class GetEntity Returns the entity of the AI
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CRenderEntity* 
CAIController::GetEntity() const
{
	return m_pEntity;
}
/**
*
* CAIController class GetAIType Returns the type of AI used
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
EAIType 
CAIController::GetAIType() const
{
	return m_eAIType;
}