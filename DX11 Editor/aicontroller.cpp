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
* AIController class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
AIController::AIController()
	: m_fMovementSpeed( 0.0f )
	, m_fRotationSpeed( 0.0f )
	, m_eAIType( AI_INVALID )
	, m_pHivemind( 0 )
	, m_fThoughtDelay( 0.0f )
{
	m_vecWaypoint *= 0.0f;
	m_vecAStarActivePoint *= 0.0f;
}
/**
*
* AIController class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
AIController::~AIController()
{

}
/**
*
* AIController class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns true
*
*/
bool
AIController::Initialise( AIHiveMind* _pHivemind, RenderEntity* _pEntity, float _fMovementSpeed, float _fRotationSpeed )
{
	bool bResult = false;

	m_pHivemind = _pHivemind;
	m_pEntity = _pEntity;
	m_fMovementSpeed = _fMovementSpeed;
	m_fRotationSpeed = _fRotationSpeed;

	m_iCurrentWaypointIndex = rand() % _pHivemind->GetNavigationGridSize();
	m_vecWaypoint = m_pHivemind->GetRandomWaypoint();
	Math::Vector3* pWaypoint = m_pHivemind->GetNextWaypoint( m_vecWaypoint, m_iCurrentWaypointIndex );
	if( pWaypoint )
	{
		m_vecAStarActivePoint = *pWaypoint;
	}

	return true;
}
/**
*
* AIController class Process
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
void
AIController::Process( float _fDeltaTime, Math::Vector3& _rAvoidance )
{
	//Check if thought process is delayed
	if( m_fThoughtDelay > 0.0f )
	{
		m_fThoughtDelay -= _fDeltaTime;
	}

	CheckWaypointReached();
	//Avoid obstacles
	Math::Vector3 vecCurrentPos = m_pEntity->GetPosition();
	vecCurrentPos += _rAvoidance * m_fMovementSpeed * 5.0f * _fDeltaTime;
	m_pEntity->SetPosition( vecCurrentPos );
}
/**
*
* AIController class ProcessWaypointMovement Moves AI towards its current waypoint
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
void
AIController::ProcessWaypointMovement( float _fDeltaTime )
{
	if( m_fThoughtDelay <= 0.0f )
	{
		Math::Vector3 vecForward = m_pEntity->GetForward();
		Math::Vector3 vecCurrentPos = m_pEntity->GetPosition();
		Math::Vector3 vecToWaypoint = m_vecWaypoint - vecCurrentPos;
		vecToWaypoint.y = 0.0f;
		vecForward += vecToWaypoint * m_fRotationSpeed * _fDeltaTime;
		vecForward = Math::Normalise( vecForward );

		vecCurrentPos += vecForward * m_fMovementSpeed * _fDeltaTime;
		m_pEntity->SetPosition( vecCurrentPos );
		m_pEntity->SetForward( vecForward );
	}
}
/**
*
* AIController class ProcessAStarMovement Moves AI towards its current waypoint using the A* algorithm
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _iPathLength Max path length to search
* @param _fDeltaTime Game time elapsed
*
*/
void
AIController::ProcessAStarMovement( int _iPathLength, float _fDeltaTime )
{
	if( m_fThoughtDelay <= 0.0f )
	{
		Math::Vector3 vecForward = m_pEntity->GetForward();
		Math::Vector3 vecCurrentPos = m_pEntity->GetPosition();
		Math::Vector3 vecToWaypoint = m_vecAStarActivePoint - vecCurrentPos;
		vecToWaypoint.y = 0.0f;
		vecForward += vecToWaypoint * m_fRotationSpeed * _fDeltaTime;
		vecForward = Math::Normalise( vecForward );

		vecCurrentPos += vecForward * m_fMovementSpeed * _fDeltaTime;
		m_pEntity->SetPosition( vecCurrentPos );
		m_pEntity->SetForward( vecForward );
	}
}
void
AIController::SetAStarTarget( Math::Vector3& _rVecTarget )
{
	m_vecWaypoint = _rVecTarget;
}
/**
*
* AIController class Checks if this controller has reached its waypoint. If so, get a new one
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
AIController::CheckWaypointReached()
{
	//Check target waypoint
	Math::Vector3 vecCurrentPos = m_pEntity->GetPosition();
	Math::Vector3 vecToWaypoint = m_vecWaypoint - vecCurrentPos;
	vecToWaypoint.y = 0.0f;
	float fDistanceToWaypoint = vecToWaypoint.LengthSq();
	if( fDistanceToWaypoint < 0.5f )
	{
		m_fThoughtDelay = 0.0f;
		m_vecWaypoint = m_pHivemind->GetRandomWaypoint();
		Math::Vector3* pWaypoint = m_pHivemind->GetNextWaypoint( m_vecWaypoint, m_iCurrentWaypointIndex );
		if( pWaypoint )
		{
			m_vecAStarActivePoint = *pWaypoint;
		}
	}
	if( true )
	{
		//Also check current A* waypoint
		vecCurrentPos = m_pEntity->GetPosition();
		vecToWaypoint = m_vecAStarActivePoint - vecCurrentPos;
		vecToWaypoint.y = 0.0f;
		fDistanceToWaypoint = vecToWaypoint.LengthSq();
		if( fDistanceToWaypoint < 0.5f )
		{
			m_fThoughtDelay = 0.0f;
			Math::Vector3* pWaypoint = m_pHivemind->GetNextWaypoint( m_vecWaypoint, m_iCurrentWaypointIndex );
			if( pWaypoint )
			{
				m_vecAStarActivePoint = *pWaypoint;
			}
		}
	}
}
/**
*
* AIController class GetCurrentWaypoint Returns the current waypoint for this controller
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
Math::Vector3&
AIController::GetCurrentWaypoint()
{
	return m_vecWaypoint;
}
/**
*
* AIController class SetAIType Specifies the type of AI used
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
AIController::SetAIType( EAIType _eAIType )
{
	m_eAIType = _eAIType;
}
/**
*
* AIController class GetEntity Returns the entity of the AI
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
RenderEntity*
AIController::GetEntity() const
{
	return m_pEntity;
}
/**
*
* AIController class GetAIType Returns the type of AI used
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
EAIType
AIController::GetAIType() const
{
	return m_eAIType;
}