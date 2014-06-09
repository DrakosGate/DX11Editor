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
, m_fThoughtDelay(0.0f)
{
	m_vecWaypoint *= 0.0f;
	m_vecAStarActivePoint *= 0.0f;
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

	m_iCurrentWaypointIndex = rand() % _pHivemind->GetNavigationGridSize();

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
	//Check if thought process is delayed
	if (m_fThoughtDelay > 0.0f)
	{
		m_fThoughtDelay -= _fDeltaTime;
	}
	
	CheckWaypointReached();
	//Avoid obstacles
	D3DXVECTOR3 vecCurrentPos = m_pEntity->GetPosition();
	vecCurrentPos += _rAvoidance * m_fMovementSpeed * 5.0f * _fDeltaTime;
	m_pEntity->SetPosition(vecCurrentPos);
}
/**
*
* CAIController class ProcessWaypointMovement Moves AI towards its current waypoint
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
void 
CAIController::ProcessWaypointMovement(float _fDeltaTime)
{
	if (m_fThoughtDelay <= 0.0f)
	{
		D3DXVECTOR3 vecForward = m_pEntity->GetForward();
		D3DXVECTOR3 vecCurrentPos = m_pEntity->GetPosition();
		D3DXVECTOR3 vecToWaypoint = m_vecAStarActivePoint - vecCurrentPos;
		vecToWaypoint.y = 0.0f;
		vecForward += vecToWaypoint * m_fRotationSpeed * _fDeltaTime;
		D3DXVec3Normalize(&vecForward, &vecForward);

		vecCurrentPos += vecForward * m_fMovementSpeed * _fDeltaTime;
		m_pEntity->SetPosition(vecCurrentPos);
		m_pEntity->SetForward(vecForward);
	}
}
/**
*
* CAIController class ProcessAStarMovement Moves AI towards its current waypoint using the A* algorithm
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _iPathLength Max path length to search
* @param _fDeltaTime Game time elapsed
*
*/
void
CAIController::ProcessAStarMovement(int _iPathLength, float _fDeltaTime)
{
	if (m_fThoughtDelay <= 0.0f)
	{
		D3DXVECTOR3 vecForward = m_pEntity->GetForward();
		D3DXVECTOR3 vecCurrentPos = m_pEntity->GetPosition();
		D3DXVECTOR3 vecToWaypoint = m_vecAStarActivePoint - vecCurrentPos;
		vecToWaypoint.y = 0.0f;
		vecForward += vecToWaypoint * m_fRotationSpeed * _fDeltaTime;
		D3DXVec3Normalize(&vecForward, &vecForward);

		vecCurrentPos += vecForward * m_fMovementSpeed * _fDeltaTime;
		m_pEntity->SetPosition(vecCurrentPos);
		m_pEntity->SetForward(vecForward);
	}
}
void
CAIController::SetAStarTarget(D3DXVECTOR3& _rVecTarget)
{
	m_vecWaypoint = _rVecTarget;
}
/**
*
* CAIController class Checks if this controller has reached its waypoint. If so, get a new one
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CAIController::CheckWaypointReached()
{
	D3DXVECTOR3 vecCurrentPos = m_pEntity->GetPosition();
	D3DXVECTOR3 vecToWaypoint = m_vecAStarActivePoint - vecCurrentPos;
	vecToWaypoint.y = 0.0f;
	float fDistanceToWaypoint = D3DXVec3LengthSq(&vecToWaypoint);
	if (fDistanceToWaypoint < 0.5f)
	{
		m_fThoughtDelay = 0.0f;
		m_vecAStarActivePoint = m_pHivemind->GetRandomWaypoint();
		//D3DXVECTOR3* pWaypoint = m_pHivemind->GetNextWaypoint(m_vecWaypoint, m_iCurrentWaypointIndex);
		//if (pWaypoint)
		//{
		//	m_vecAStarActivePoint = *pWaypoint;
		//}
	}
}
/**
*
* CAIController class GetCurrentWaypoint Returns the current waypoint for this controller
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
D3DXVECTOR3&
CAIController::GetCurrentWaypoint()
{
	return m_vecWaypoint;
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