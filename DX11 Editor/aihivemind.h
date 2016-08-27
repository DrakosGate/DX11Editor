//
//  File Name   :   AIHiveMind.h
//  Description :   Class of AIHiveMind
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __AIHIVEMIND_H__
#define __AIHIVEMIND_H__

// Library Includes
#include <vector>

// Local Includes
#include "mathlibrary.h"

class AIHiveMind;
// Types
struct TAIDescription
{
	TAIDescription()
	{
		fMovementSpeed = 0.0f;
		fRotationSpeed = 0.0f;
	}
	TAIDescription( float _fMovement, float _fRotation )
	{
		fMovementSpeed = _fMovement;
		fRotationSpeed = _fRotation;
	}
	float fMovementSpeed;
	float fRotationSpeed;
};
struct TAIThreadData
{
	TAIThreadData( AIHiveMind* _pHivemind, int _iAIIndex, float _fDeltaTime )
	{
		pThis = _pHivemind;
		iAIIndex = _iAIIndex;
		fDeltaTime = _fDeltaTime;
	}
	AIHiveMind* pThis;
	int iAIIndex;
	float fDeltaTime;
};
struct TGridNode
{
	TGridNode()
	{
		pUp = 0;
		pDown = 0;
		pLeft = 0;
		pRight = 0;
	}
	Math::Vector3 vecPosition;
	bool bIsActive;

	TGridNode* pUp;
	TGridNode* pDown;
	TGridNode* pLeft;
	TGridNode* pRight;
};
enum ENavigationMethod
{
	NAVIGATION_INVALID = -1,
	NAVIGATION_RANDOMWAYPOINT,
	NAVIGATION_ASTAR,
	NAVIGATION_MAX
};

// Constants

// Prototypes
struct ID3D11Device;

class Shader;
class RenderEntity;
class AIController;
class PointSprite;
class EntityManager;
class ThreadPool;
//class AICLKernel;

class AIHiveMind
{
public:
	AIHiveMind();
	virtual ~AIHiveMind();

	virtual bool Initialise( int _iAStarDepth );
	void Process( ThreadPool* _pCThreadPool, float _fDeltaTime );
	void ProcessIndividualAIMovement( int _iAIIndex, float _fDeltaTime );
	void ProcessIndividualAIController( int _iAIIndex, float _fDeltaTime );
	//void ProcessOpenCLKernel( float _fDeltaTime );
	void AddAI( RenderEntity* _pEntity, EAIType _eAIType );
	void AddStaticObject( ID3D11Device* _pDevice, RenderEntity* _pObject );

	void CreateNavigationGrid( ID3D11Device* _pDevice, EntityManager* _pEntityManager, Shader* _pShader, float _fGridScale, int _iWidth, int _iHeight );
	Math::Vector3& GetRandomWaypoint() const;
	Math::Vector3* GetNextWaypoint( Math::Vector3& _rVecTarget, int& _iCurrentWaypoint );
	float GetAStarNodeValue( TGridNode* _pCurrentNode, TGridNode* _pPreviousNode, Math::Vector3& _rVecTarget, int _iTreeDepth );
	TGridNode* GetNavigationGrid();
	AIController* GetAI( int _iIndex ) const;
	int GetNavigationGridSize() const;
	int GetAICount() const;

	TAIDescription& GetAIDesc( EAIType _eAIType );
	void ClearHivemind();
	void ChangeProcessingMethod( EProcessingMethod _eProcessingMethod );
	void RecalculateNavGrid( ID3D11Device* _pDevice );

private:
	AIHiveMind( const AIHiveMind& _krInstanceToCopy );
	const AIHiveMind& operator =( const AIHiveMind& _krInstanceToCopy );

private:
	EProcessingMethod m_eProcessingMethod;

	//AICLKernel* m_pCLKernel;
	AIController** m_pAI;
	std::vector<RenderEntity*> m_vecStaticObstacles;
	int m_iNumAI;

	TAIDescription* m_pAIDescriptions;
	std::vector<TAIThreadData*> m_vecThreadData;

	TGridNode* m_pNavigationGrid;
	PointSprite* m_pNavigationGridMesh;
	int m_iGridSize;
	int m_iWidth;
	int m_iHeight;
	int m_iAStarDepth;

};

#endif // __AIHIVEMIND_H__