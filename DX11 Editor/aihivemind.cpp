//
//  File Name   :   AIHiveMind.cpp
//  Description :   Code for AIHiveMind 
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes
#include "shader.h"
#include "renderentity.h"
#include "aicontroller.h"
#include "pointsprite.h"
#include "entitymanager.h"
#include "threadpool.h"
//#include "aiclkernel.h"
#include "openclcontext.h"

// This Include
#include "aihivemind.h"

// Static Variables

// Static Function Prototypes

// Implementation
AIHiveMind::AIHiveMind()
	: m_eProcessingMethod( PROCESSING_SEQUENTIAL )
	, m_pAI( 0 )
	, m_iNumAI( 0 )
	, m_pAIDescriptions( 0 )
	, m_pNavigationGrid( 0 )
	, m_pNavigationGridMesh( 0 )
	, m_iGridSize( 0 )
	, m_iAStarDepth( 0 )
{

}

AIHiveMind::~AIHiveMind()
{
	//Delete thread data
	for( unsigned int iThread = 0; iThread < m_vecThreadData.size(); ++iThread )
	{
		delete m_vecThreadData[ iThread ];
		m_vecThreadData[ iThread ] = 0;
	}
	m_vecThreadData.clear();
	m_vecStaticObstacles.clear();
	//if( m_pCLKernel )
	//{
	//	delete m_pCLKernel;
	//	m_pCLKernel = 0;
	//}
	if( m_pNavigationGrid )
	{
		delete[] m_pNavigationGrid;
		m_pNavigationGrid = 0;
		delete m_pNavigationGridMesh;
		m_pNavigationGridMesh = 0;
	}
	if( m_pAIDescriptions )
	{
		delete[] m_pAIDescriptions;
		m_pAIDescriptions = 0;
	}
	if( m_pAI )
	{
		for( int iAI = 0; iAI < m_iNumAI; ++iAI )
		{
			delete m_pAI[ iAI ];
			m_pAI[ iAI ] = 0;
		}
		delete[] m_pAI;
		m_pAI = 0;
	}
}

bool
AIHiveMind::Initialise( int _iAStarDepth )
{
	bool bResult = false;
	m_iAStarDepth = _iAStarDepth;

	//Create AI descriptions
	m_pAIDescriptions = new TAIDescription[ AI_MAX ];
	m_pAIDescriptions[ AI_HUMAN ] = TAIDescription( 1.5f, 2.0f );
	m_pAIDescriptions[ AI_CHICKEN ] = TAIDescription( 1.0f, 10.5f );

	// AI pathfinding OpenCL Kernel setup
	//m_pCLKernel = new AICLKernel();
	//_pCLKernel->LoadProgram( m_pCLKernel->GetCLProgram(), m_pCLKernel->GetCLKernel(), "Assets/OpenCLKernels/ai.cl", "ProcessAI" );

	return true;
}

void
AIHiveMind::Process( ThreadPool* _pThreadPool, float _fDeltaTime )
{
	switch( m_eProcessingMethod )
	{
	case PROCESSING_SEQUENTIAL:
	{
		for( int iAI = 0; iAI < m_iNumAI; ++iAI )
		{
			//m_pAI[iAI]->ProcessWaypointMovement(_fDeltaTime);
			m_pAI[ iAI ]->ProcessAStarMovement( 10, _fDeltaTime );
			ProcessIndividualAIController( iAI, _fDeltaTime );
		}
		break;
	}
	//case PROCESSING_OPENCL:
	//{
	//	ProcessOpenCLKernel( _pCLKernel, _fDeltaTime );
	//	for( int iAI = 0; iAI < m_iNumAI; ++iAI )
	//	{
	//		ProcessIndividualAIController( iAI, _fDeltaTime );
	//	}
	//	break;
	//}
	case PROCESSING_THREADPOOL:
	{
		for( int iAI = 0; iAI < m_iNumAI; ++iAI )
		{
			m_vecThreadData[ iAI ]->fDeltaTime = _fDeltaTime;
			_pThreadPool->AddJobToPool( &AIProcessingThread, m_vecThreadData[ iAI ] );
		}
		break;
	}
	default:
		break;
	};
	//std::function<void(void*)> aiFunction = ThreadedAI;
	//ProcessOpenCLKernel(_fDeltaTime);
	////	std::function<void(AIHiveMind&, int, float)> aiFunction = &AIHiveMind::ProcessIndividualAIController;
	//for (int iAI = 0; iAI < m_iNumAI; ++iAI)
	//{
	//	//	//aiFunction = std::function<void(int, float)>(ProcessIndividualAIController(iAI, _fDeltaTime));
	//	//	//Add AI processing to thread pool
	//	ProcessIndividualAIController(iAI, _fDeltaTime);
	//	//	//_pThreadPool->AddJobToPool(aiFunction, &TAIThreadData(this, iAI, _fDeltaTime));
	//}
}

void
AIHiveMind::ProcessIndividualAIMovement( int _iAIIndex, float _fDeltaTime )
{
	m_pAI[ _iAIIndex ]->ProcessWaypointMovement( _fDeltaTime );
}

void
AIHiveMind::ProcessIndividualAIController( int _iAIIndex, float _fDeltaTime )
{
	Math::Vector3 vecAvoidance;
	vecAvoidance *= 0.0f;
	//for (int iOther = _iAIIndex; iOther < m_iNumAI; ++iOther)
	//{
	//	if (m_pAI[_iAIIndex]->GetEntity()->HasCollided(m_pAI[iOther]->GetEntity()))
	//	{
	//		vecAvoidance += (m_pAI[_iAIIndex]->GetEntity()->GetPosition() - m_pAI[iOther]->GetEntity()->GetPosition()) * 0.5f;
	//	}
	//}
	for( unsigned int iStatic = 0; iStatic < m_vecStaticObstacles.size(); ++iStatic )
	{
		if( m_pAI[ _iAIIndex ]->GetEntity()->HasCollided( m_vecStaticObstacles[ iStatic ] ) )
		{
			vecAvoidance += ( m_pAI[ _iAIIndex ]->GetEntity()->GetPosition() - m_vecStaticObstacles[ iStatic ]->GetPosition() ) * 0.5f;
		}
	}
	vecAvoidance.y = 0.0f;
	m_pAI[ _iAIIndex ]->Process( _fDeltaTime, vecAvoidance );
}

//void
//AIHiveMind::ProcessOpenCLKernel( float _fDeltaTime )
//{
//	//m_pCLKernel->SendDataToGPU( _pCLKernel, this, _fDeltaTime );
//	//_pCLKernel->Run( m_pCLKernel->GetCLKernel() );
//	//m_pCLKernel->RetrieveOpenCLResults( _pCLKernel, this );
//}

void
AIHiveMind::AddAI( RenderEntity* _pEntity, EAIType _eAIType )
{
	++m_iNumAI;
	AIController** pOldArray = m_pAI;
	m_pAI = new AIController*[ m_iNumAI ];
	for( int iAI = 0; iAI < m_iNumAI - 1; ++iAI )
	{
		m_pAI[ iAI ] = pOldArray[ iAI ];
	}
	//Add new AI
	TAIDescription* pThisAI = &m_pAIDescriptions[ _eAIType ];
	m_pAI[ m_iNumAI - 1 ] = new AIController();
	m_pAI[ m_iNumAI - 1 ]->Initialise( this, _pEntity, pThisAI->fMovementSpeed, pThisAI->fRotationSpeed );
	m_pAI[ m_iNumAI - 1 ]->SetAIType( _eAIType );

	m_vecThreadData.push_back( new TAIThreadData( this, m_iNumAI - 1, 0.0f ) );

	delete[] pOldArray;
	pOldArray = 0;
}

void
AIHiveMind::AddStaticObject( ID3D11Device* _pDevice, RenderEntity* _pObject )
{
	m_vecStaticObstacles.push_back( _pObject );
	//Check navigation grid for grid sections in range of this obstacle

	RecalculateNavGrid( _pDevice );
}

void
AIHiveMind::CreateNavigationGrid( ID3D11Device* _pDevice, EntityManager* _pEntityManager, Shader* _pShader, float _fGridScale, int _iWidth, int _iHeight )
{
	m_iWidth = _iWidth;
	m_iHeight = _iHeight;
	Math::Vector3 vecGridScale = Math::Vector3( 1.0f / static_cast<float>( m_iWidth ), 1.0f, 1.0f / static_cast<float>( m_iHeight ) ) * _fGridScale;

	m_iGridSize = _iWidth * _iHeight;
	m_pNavigationGrid = new TGridNode[ m_iGridSize ];
	m_pNavigationGridMesh = new PointSprite();
	m_pNavigationGridMesh->SetObjectShader( _pShader );
	m_pNavigationGridMesh->SetRadius( FLT_MAX );

	Math::Colour lineColour( 0.2f, 0.2f, 0.8f, 0.2f );

	int iCurrentGrid = 0;
	for( int iHeight = 0; iHeight < m_iHeight; ++iHeight )
	{
		for( int iWidth = 0; iWidth < m_iWidth; ++iWidth )
		{
			m_pNavigationGrid[ iCurrentGrid ].vecPosition = Math::Vector3( static_cast<float>( iWidth ) - m_iWidth * 0.5f, 0.01f, static_cast<float>( iHeight ) - m_iHeight * 0.5f );
			m_pNavigationGrid[ iCurrentGrid ].vecPosition = Math::Vector3( m_pNavigationGrid[ iCurrentGrid ].vecPosition.x * vecGridScale.x, m_pNavigationGrid[ iCurrentGrid ].vecPosition.y, m_pNavigationGrid[ iCurrentGrid ].vecPosition.z * vecGridScale.z );
			m_pNavigationGrid[ iCurrentGrid ].bIsActive = true;

			//Inform this node of the surrounding nodes
			if( iHeight > 0 )
			{
				m_pNavigationGrid[ iCurrentGrid ].pUp = &m_pNavigationGrid[ iCurrentGrid - m_iHeight ];
			}
			if( iHeight < m_iHeight - 1 )
			{
				m_pNavigationGrid[ iCurrentGrid ].pDown = &m_pNavigationGrid[ iCurrentGrid + m_iHeight ];
			}
			if( iWidth > 0 )
			{
				m_pNavigationGrid[ iCurrentGrid ].pLeft = &m_pNavigationGrid[ iCurrentGrid - 1 ];
			}
			if( iWidth < m_iWidth - 1 )
			{
				m_pNavigationGrid[ iCurrentGrid ].pRight = &m_pNavigationGrid[ iCurrentGrid + 1 ];
			}
			//Create a point sprite to display this node
			m_pNavigationGridMesh->AddPointSprite( _pDevice, m_pNavigationGrid[ iCurrentGrid ].vecPosition, Math::Vector3( 0.0f, 0.1f, 0.0f ), Math::Vector2( 100.0f, 100.0f ), lineColour, 0.0f, 0 );
			++iCurrentGrid;
		}
	}
	_pEntityManager->AddEntity( m_pNavigationGridMesh, SCENE_DEBUG );
}

Math::Vector3&
AIHiveMind::GetRandomWaypoint() const
{
	bool bActivePositionFound = false;
	int iRandomIndex = 0;
	while( bActivePositionFound == false )
	{
		iRandomIndex = rand() % m_iGridSize;
		bActivePositionFound = m_pNavigationGrid[ iRandomIndex ].bIsActive;
	}
	return ( m_pNavigationGrid[ iRandomIndex ].vecPosition );
}

Math::Vector3*
AIHiveMind::GetNextWaypoint( Math::Vector3& _rVecTarget, int& _iCurrentWaypoint )
{
	float fBestValue = FLT_MAX;
	int iBestWaypoint = 0;
	int iTreeSearchDepth = m_iAStarDepth;

	float fInactivePenalty = 500000;
	//Check UP
	if( m_pNavigationGrid[ _iCurrentWaypoint ].pUp )
	{
		float fUpValue = GetAStarNodeValue( m_pNavigationGrid[ _iCurrentWaypoint ].pUp, &m_pNavigationGrid[ _iCurrentWaypoint ], _rVecTarget, iTreeSearchDepth );
		fUpValue += ( m_pNavigationGrid[ _iCurrentWaypoint ].pUp->bIsActive == false ) * fInactivePenalty;
		fBestValue = fUpValue;
		iBestWaypoint = _iCurrentWaypoint - m_iHeight;
	}
	//Check DOWN
	if( m_pNavigationGrid[ _iCurrentWaypoint ].pDown )
	{
		float fDownValue = GetAStarNodeValue( m_pNavigationGrid[ _iCurrentWaypoint ].pDown, &m_pNavigationGrid[ _iCurrentWaypoint ], _rVecTarget, iTreeSearchDepth );
		fDownValue += ( m_pNavigationGrid[ _iCurrentWaypoint ].pDown->bIsActive == false ) * fInactivePenalty;
		if( fDownValue < fBestValue )
		{
			fBestValue = fDownValue;
			iBestWaypoint = _iCurrentWaypoint + m_iHeight;
		}
	}
	//Check LEFT
	if( m_pNavigationGrid[ _iCurrentWaypoint ].pLeft )
	{
		float fLeftValue = GetAStarNodeValue( m_pNavigationGrid[ _iCurrentWaypoint ].pLeft, &m_pNavigationGrid[ _iCurrentWaypoint ], _rVecTarget, iTreeSearchDepth );
		fLeftValue += ( m_pNavigationGrid[ _iCurrentWaypoint ].pLeft->bIsActive == false ) * fInactivePenalty;
		if( fLeftValue < fBestValue )
		{
			fBestValue = fLeftValue;
			iBestWaypoint = _iCurrentWaypoint - 1;
		}
	}
	//Check RIGHT
	if( m_pNavigationGrid[ _iCurrentWaypoint ].pRight )
	{
		float fRightValue = GetAStarNodeValue( m_pNavigationGrid[ _iCurrentWaypoint ].pRight, &m_pNavigationGrid[ _iCurrentWaypoint ], _rVecTarget, iTreeSearchDepth );
		fRightValue += ( m_pNavigationGrid[ _iCurrentWaypoint ].pRight->bIsActive == false ) * fInactivePenalty;
		if( fRightValue < fBestValue )
		{
			fBestValue = fRightValue;
			iBestWaypoint = _iCurrentWaypoint + 1;
		}
	}
	_iCurrentWaypoint = iBestWaypoint;
	return ( &m_pNavigationGrid[ _iCurrentWaypoint ].vecPosition );
}

float
AIHiveMind::GetAStarNodeValue( TGridNode* _pCurrentNode, TGridNode* _pPreviousNode, Math::Vector3& _rVecTarget, int _iTreeDepth )
{
	//KEY AREA: Find A* values for a specific node
	float fChildValue = 0.0f;
	float fNodeValue = 0.0f;
	if( _iTreeDepth >= 0 )
	{
		//Check if the target node has been reached
		if( ( _rVecTarget - _pCurrentNode->vecPosition ).LengthSq() > 0.5f )
		{
			//Find best child node
			TGridNode* pNextNode = 0;
			//Calculate value of this node
			float fCostFromStart = 0.0f;
			if( _pPreviousNode )
			{
				fCostFromStart = ( _pCurrentNode->vecPosition - _pPreviousNode->vecPosition ).LengthSq();
			}
			float fCostToDestination = ( _rVecTarget - _pCurrentNode->vecPosition ).LengthSq();
			fNodeValue = fCostFromStart + fCostToDestination;

			//Check UP
			if( _pCurrentNode->pUp )
			{
				float fUpValue = GetAStarNodeValue( _pCurrentNode->pUp, _pCurrentNode, _rVecTarget, _iTreeDepth - 1 );
				fChildValue = fUpValue;
				pNextNode = _pCurrentNode->pUp;
			}
			//Check DOWN
			if( _pCurrentNode->pDown )
			{
				float fDownValue = GetAStarNodeValue( _pCurrentNode->pDown, _pCurrentNode, _rVecTarget, _iTreeDepth - 1 );
				if( fDownValue < fChildValue )
				{
					fChildValue = fDownValue;
					pNextNode = _pCurrentNode->pDown;
				}
			}
			//Check LEFT
			if( _pCurrentNode->pLeft )
			{
				float fLeftValue = GetAStarNodeValue( _pCurrentNode->pLeft, _pCurrentNode, _rVecTarget, _iTreeDepth - 1 );
				if( fLeftValue < fChildValue )
				{
					fChildValue = fLeftValue;
					pNextNode = _pCurrentNode->pLeft;
				}
			}
			//Check RIGHT
			if( _pCurrentNode->pRight )
			{
				float fRightValue = GetAStarNodeValue( _pCurrentNode->pRight, _pCurrentNode, _rVecTarget, _iTreeDepth - 1 );
				if( fRightValue < fChildValue )
				{
					fChildValue = fRightValue;
					pNextNode = _pCurrentNode->pRight;
				}
			}
		}
	}

	return ( fNodeValue + fChildValue );
}

TGridNode*
AIHiveMind::GetNavigationGrid()
{
	return m_pNavigationGrid;
}

AIController*
AIHiveMind::GetAI( int _iIndex ) const
{
	return m_pAI[ _iIndex ];
}

int
AIHiveMind::GetNavigationGridSize() const
{
	return m_iGridSize;
}

int
AIHiveMind::GetAICount() const
{
	return m_iNumAI;
}

TAIDescription&
AIHiveMind::GetAIDesc( EAIType _eAIType )
{
	return m_pAIDescriptions[ _eAIType ];
}

void
AIHiveMind::ClearHivemind()
{
	if( m_pAI )
	{
		for( int iAI = 0; iAI < m_iNumAI; ++iAI )
		{
			delete m_pAI[ iAI ];
			m_pAI[ iAI ] = 0;
		}
		m_iNumAI = 0;
		delete[] m_pAI;
		m_pAI = 0;
	}
	m_vecStaticObstacles.clear();
}

void
AIHiveMind::ChangeProcessingMethod( EProcessingMethod _eProcessingMethod )
{
	m_eProcessingMethod = _eProcessingMethod;
}

void
AIHiveMind::RecalculateNavGrid( ID3D11Device* _pDevice )
{
	Math::Colour deactiveColour( 1.5f, 0.1f, 0.1f, 0.5f );
	Math::Colour lineColour( 0.2f, 0.2f, 0.8f, 0.2f );

	int iCurrentGrid = 0;
	for( int iHeight = 0; iHeight < m_iHeight; ++iHeight )
	{
		for( int iWidth = 0; iWidth < m_iWidth; ++iWidth )
		{
			m_pNavigationGrid[ iCurrentGrid ].bIsActive = true;
			m_pNavigationGridMesh->GetPointSprite( iCurrentGrid )->colour = lineColour;
			for( unsigned int iObstalce = 0; iObstalce < m_vecStaticObstacles.size(); ++iObstalce )
			{
				if( m_vecStaticObstacles[ iObstalce ]->DoDraw() )
				{
					Math::Vector3 vecToObstacle = m_vecStaticObstacles[ iObstalce ]->GetPosition() - m_pNavigationGrid[ iCurrentGrid ].vecPosition;
					vecToObstacle.y = 0.0f;
					if( vecToObstacle.LengthSq() < m_vecStaticObstacles[ iObstalce ]->GetRadius() * m_vecStaticObstacles[ iObstalce ]->GetScale().x * 0.25f )
					{
						//Deactivate this grid element
						m_pNavigationGrid[ iCurrentGrid ].bIsActive = false;
						m_pNavigationGridMesh->GetPointSprite( iCurrentGrid )->colour = deactiveColour;
					}
				}
			}
			++iCurrentGrid;
		}
	}
	m_pNavigationGridMesh->RefreshBuffers( _pDevice );
}