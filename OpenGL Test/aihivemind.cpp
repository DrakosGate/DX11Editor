//
//  File Name   :   AIHiveMind.cpp
//  Description :   Code for CAIHiveMind 
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
#include "aiclkernel.h"

// This Include
#include "aihivemind.h"

// Static Variables

// Static Function Prototypes

// Implementation
void ThreadedAI(void* _pParameters)
{
	TAIThreadData* pParam = reinterpret_cast<TAIThreadData*>(_pParameters);
	pParam->pThis->ProcessIndividualAIController(pParam->iAIIndex, pParam->fDeltaTime);
}
/**
*
* CAIHiveMind class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CAIHiveMind::CAIHiveMind()
: m_eProcessingMethod(PROCESSING_SEQUENTIAL)
, m_pAI(0)
, m_iNumAI(0)
, m_pAIDescriptions(0)
, m_pNavigationGrid(0)
, m_pNavigationGridMesh(0)
, m_iGridSize(0)
{

}
/**
*
* CAIHiveMind class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CAIHiveMind::~CAIHiveMind()
{
	m_vecStaticObstacles.clear();
	if (m_pCLKernel)
	{
		delete m_pCLKernel;
		m_pCLKernel = 0;
	}
	if (m_pNavigationGrid)
	{
		delete[] m_pNavigationGrid;
		m_pNavigationGrid = 0;
		delete m_pNavigationGridMesh;
		m_pNavigationGridMesh = 0;
	}
	if(m_pAIDescriptions)
	{
		delete[] m_pAIDescriptions;
		m_pAIDescriptions = 0;
	}
	if(m_pAI)
	{
		for(int iAI = 0; iAI < m_iNumAI; ++iAI)
		{
			delete m_pAI[iAI];
			m_pAI[iAI] = 0;
		}
		delete[] m_pAI;
		m_pAI = 0;
	}
}
/**
*
* CAIHiveMind class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns true
*
*/
bool 
CAIHiveMind::Initialise()
{
	bool bResult = false;

	//Create AI descriptions
	m_pAIDescriptions = new TAIDescription[AI_MAX];
	m_pAIDescriptions[AI_HUMAN] = TAIDescription(1.5f, 2.0f);
	m_pAIDescriptions[AI_CHICKEN] = TAIDescription(1.0f, 0.5f);

	m_pCLKernel = new CAICLKernel();
	m_pCLKernel->InitialiseOpenCL();
	m_pCLKernel->LoadProgram("OpenCLKernels/ai.cl", "ProcessAI");
	
	return true;
}
/**
*
* CAIHiveMind class Process
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
void 
CAIHiveMind::Process(CThreadPool* _pThreadPool, float _fDeltaTime)
{
	switch (m_eProcessingMethod)
	{
	case PROCESSING_SEQUENTIAL:
		{
			for (int iAI = 0; iAI < m_iNumAI; ++iAI)
			{
				m_pAI[iAI]->ProcessWaypointMovement(_fDeltaTime);
				//m_pAI[iAI]->ProcessAStarMovement(10, _fDeltaTime);
				ProcessIndividualAIController(iAI, _fDeltaTime);
			}					 
			break;
		}
	case PROCESSING_OPENCL:
		{
			ProcessOpenCLKernel(_fDeltaTime);
			for (int iAI = 0; iAI < m_iNumAI; ++iAI)
			{
				ProcessIndividualAIController(iAI, _fDeltaTime);
			}
			break;
		}
	case PROCESSING_THREADPOOL:
		{
			for (int iAI = 0; iAI < m_iNumAI; ++iAI)
			{
			//  _pThreadPool->AddJob();
			}
			break;
		}
	default:
		break;
	};
	//std::function<void(void*)> aiFunction = ThreadedAI;
	//ProcessOpenCLKernel(_fDeltaTime);
	////	std::function<void(CAIHiveMind&, int, float)> aiFunction = &CAIHiveMind::ProcessIndividualAIController;
	//for (int iAI = 0; iAI < m_iNumAI; ++iAI)
	//{
	//	//	//aiFunction = std::function<void(int, float)>(ProcessIndividualAIController(iAI, _fDeltaTime));
	//	//	//Add AI processing to thread pool
	//	ProcessIndividualAIController(iAI, _fDeltaTime);
	//	//	//_pThreadPool->AddJobToPool(aiFunction, &TAIThreadData(this, iAI, _fDeltaTime));
	//}
}
/**
*
* CAIHiveMind class Processes an individual AI controller based on the Index given
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
void
CAIHiveMind::ProcessIndividualAIController(int _iAIIndex, float _fDeltaTime)
{
	D3DXVECTOR3 vecAvoidance;
	vecAvoidance *= 0.0f;
	//for (int iOther = _iAIIndex; iOther < m_iNumAI; ++iOther)
	//{
	//	if (m_pAI[_iAIIndex]->GetEntity()->HasCollided(m_pAI[iOther]->GetEntity()))
	//	{
	//		vecAvoidance += (m_pAI[_iAIIndex]->GetEntity()->GetPosition() - m_pAI[iOther]->GetEntity()->GetPosition()) * 0.5f;
	//	}
	//}
	//for (unsigned int iStatic = 0; iStatic < m_vecStaticObstacles.size(); ++iStatic)
	//{
	//	if (m_pAI[_iAIIndex]->GetEntity()->HasCollided(m_vecStaticObstacles[iStatic]))
	//	{
	//		vecAvoidance += (m_pAI[_iAIIndex]->GetEntity()->GetPosition() - m_vecStaticObstacles[iStatic]->GetPosition()) * 0.5f;
	//	}
	//}
	vecAvoidance.y = 0.0f;
	m_pAI[_iAIIndex]->Process(_fDeltaTime, vecAvoidance);
}
/**
*
* CAIHiveMind class Sends data, processes calculation and receives data from OpenCL
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed since last frame
*
*/
void
CAIHiveMind::ProcessOpenCLKernel(float _fDeltaTime)
{
	m_pCLKernel->SendDataToGPU(this, _fDeltaTime);
	m_pCLKernel->Run();
	m_pCLKernel->RetrieveOpenCLResults(this);
}
/**
*
* CAIHiveMind class AddAI
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pEntity New AI added
*
*/
void 
CAIHiveMind::AddAI(CRenderEntity* _pEntity, EAIType _eAIType)
{
	++m_iNumAI;
	CAIController** pOldArray = m_pAI;
	m_pAI = new CAIController*[m_iNumAI];
	for(int iAI = 0; iAI < m_iNumAI - 1; ++iAI)
	{
		m_pAI[iAI] = pOldArray[iAI];
	}
	//Add new AI
	TAIDescription* pThisAI = &m_pAIDescriptions[_eAIType];
	m_pAI[m_iNumAI - 1] = new CAIController();
	m_pAI[m_iNumAI - 1]->Initialise(this, _pEntity, pThisAI->fMovementSpeed, pThisAI->fRotationSpeed);
	m_pAI[m_iNumAI - 1]->SetAIType(_eAIType);

	delete[] pOldArray;
	pOldArray = 0;
}
/**
*
* CAIHiveMind class AddStaticObject Adds a new static object in the scene
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rPos Position of static object
* @param _fRadius Radius of static object
*
*/
void 
CAIHiveMind::AddStaticObject(ID3D11Device* _pDevice, CRenderEntity* _pObject)
{
	m_vecStaticObstacles.push_back(_pObject);
	//Check navigation grid for grid sections in range of this obstacle

	RecalculateNavGrid(_pDevice);
}
/**
*
* CAIHiveMind class Creates a navigation grid object and model
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CAIHiveMind::CreateNavigationGrid(ID3D11Device* _pDevice, CEntityManager* _pEntityManager, CShader* _pShader, float _fGridScale, int _iWidth, int _iHeight)
{
	m_iWidth = _iWidth;
	m_iHeight = _iHeight;
	D3DXVECTOR3 vecGridScale = D3DXVECTOR3(1.0f / static_cast<float>(m_iWidth), 1.0f, 1.0f / static_cast<float>(m_iHeight)) * _fGridScale;

	m_iGridSize = _iWidth * _iHeight;
	m_pNavigationGrid = new TGridNode[m_iGridSize];
	m_pNavigationGridMesh = new CPointSprite();
	m_pNavigationGridMesh->SetObjectShader(_pShader);
	m_pNavigationGridMesh->SetRadius(FLT_MAX);

	D3DXCOLOR lineColour(0.2f, 0.2f, 0.8f, 0.2f);

	int iCurrentGrid = 0;
	for (int iHeight = 0; iHeight < m_iHeight; ++iHeight)
	{
		for (int iWidth = 0; iWidth < m_iWidth; ++iWidth)
		{
			m_pNavigationGrid[iCurrentGrid].vecPosition = D3DXVECTOR3(static_cast<float>(iWidth)-m_iWidth * 0.5f, 0.01f, static_cast<float>(iHeight)-m_iHeight * 0.5f);
			m_pNavigationGrid[iCurrentGrid].vecPosition = D3DXVECTOR3(m_pNavigationGrid[iCurrentGrid].vecPosition.x * vecGridScale.x, m_pNavigationGrid[iCurrentGrid].vecPosition.y, m_pNavigationGrid[iCurrentGrid].vecPosition.z * vecGridScale.z);
			m_pNavigationGrid[iCurrentGrid].bIsActive = true;

			//Inform this node of the surrounding nodes
			if (iHeight > 0)
			{
				m_pNavigationGrid[iCurrentGrid].pUp = &m_pNavigationGrid[iCurrentGrid - m_iHeight];
			}
			if (iHeight < m_iHeight - 1)
			{
				m_pNavigationGrid[iCurrentGrid].pDown = &m_pNavigationGrid[iCurrentGrid + m_iHeight];
			}
			if (iWidth > 0)
			{
				m_pNavigationGrid[iCurrentGrid].pLeft = &m_pNavigationGrid[iCurrentGrid - 1];
			}
			if (iWidth < m_iWidth - 1)
			{
				m_pNavigationGrid[iCurrentGrid].pRight = &m_pNavigationGrid[iCurrentGrid + 1];
			}
			//Create a point sprite to display this node
			m_pNavigationGridMesh->AddPointSprite(_pDevice, m_pNavigationGrid[iCurrentGrid].vecPosition, D3DXVECTOR3(0.0f, 0.1f, 0.0f), D3DXVECTOR2(100.0f, 100.0f), lineColour, 0.0f, 0);
			++iCurrentGrid;
		}
	}
	_pEntityManager->AddEntity(m_pNavigationGridMesh, SCENE_DEBUG);
}
/**
*
* CAIHiveMind class Returns a random waypoint
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
D3DXVECTOR3&
CAIHiveMind::GetRandomWaypoint() const
{
	bool bActivePositionFound = false;
	int iRandomIndex = 0;
	while (bActivePositionFound == false)
	{
		iRandomIndex = rand() % m_iGridSize;
		bActivePositionFound = m_pNavigationGrid[iRandomIndex].bIsActive;
	}
	return (m_pNavigationGrid[iRandomIndex].vecPosition);
}
/**
*
* CAIHiveMind class Returns the next A* waypoint
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return
*
*/
D3DXVECTOR3*
CAIHiveMind::GetNextWaypoint(D3DXVECTOR3& _rVecTarget, int& _iCurrentWaypoint)
{
	float fBestValue = FLT_MAX;
	int iBestWaypoint = 0;
	int iTreeSearchDepth = 10;

	float fInactivePenalty = 500000;
	//Check UP
	if (m_pNavigationGrid[_iCurrentWaypoint].pUp)
	{
		float fUpValue = GetAStarNodeValue(m_pNavigationGrid[_iCurrentWaypoint].pUp, &m_pNavigationGrid[_iCurrentWaypoint], _rVecTarget, iTreeSearchDepth);
		fUpValue += (m_pNavigationGrid[_iCurrentWaypoint].pUp->bIsActive == false) * fInactivePenalty;
		fBestValue = fUpValue;
		iBestWaypoint = _iCurrentWaypoint - m_iHeight;
	}
	//Check DOWN
	if (m_pNavigationGrid[_iCurrentWaypoint].pDown)
	{
		float fDownValue = GetAStarNodeValue(m_pNavigationGrid[_iCurrentWaypoint].pDown, &m_pNavigationGrid[_iCurrentWaypoint], _rVecTarget, iTreeSearchDepth);
		fDownValue += (m_pNavigationGrid[_iCurrentWaypoint].pDown->bIsActive == false) * fInactivePenalty;
		if (fDownValue < fBestValue)
		{
			fBestValue = fDownValue;
			iBestWaypoint = _iCurrentWaypoint + m_iHeight;
		}
	}
	//Check LEFT
	if (m_pNavigationGrid[_iCurrentWaypoint].pLeft)
	{
		float fLeftValue = GetAStarNodeValue(m_pNavigationGrid[_iCurrentWaypoint].pLeft, &m_pNavigationGrid[_iCurrentWaypoint], _rVecTarget, iTreeSearchDepth);
		fLeftValue += (m_pNavigationGrid[_iCurrentWaypoint].pLeft->bIsActive == false) * fInactivePenalty;
		if (fLeftValue < fBestValue)
		{
			fBestValue = fLeftValue;
			iBestWaypoint = _iCurrentWaypoint - 1;
		}
	}
	//Check RIGHT
	if (m_pNavigationGrid[_iCurrentWaypoint].pRight)
	{
		float fRightValue = GetAStarNodeValue(m_pNavigationGrid[_iCurrentWaypoint].pRight, &m_pNavigationGrid[_iCurrentWaypoint], _rVecTarget, iTreeSearchDepth);
		fRightValue += (m_pNavigationGrid[_iCurrentWaypoint].pRight->bIsActive == false) * fInactivePenalty;
		if (fRightValue < fBestValue)
		{
			fBestValue = fRightValue;
			iBestWaypoint = _iCurrentWaypoint + 1;
		}
	}
	_iCurrentWaypoint = iBestWaypoint;
	return (&m_pNavigationGrid[_iCurrentWaypoint].vecPosition);
}
/**
*
* CAIHiveMind class Calculates the value of the current node using A*
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
float
CAIHiveMind::GetAStarNodeValue(TGridNode* _pCurrentNode, TGridNode* _pPreviousNode, D3DXVECTOR3& _rVecTarget, int _iTreeDepth)
{
	float fChildValue = 0.0f;
	float fNodeValue = 0.0f;
	if(_iTreeDepth >= 0)
	{
		//Check if the target node has been reached
		if (D3DXVec3LengthSq(&(_rVecTarget - _pCurrentNode->vecPosition)) > 0.5f)
		{
			//Find best child node
			TGridNode* pNextNode = 0;
			//Calculate value of this node
			float fCostFromStart = 0.0f;
			if (_pPreviousNode)
			{
				fCostFromStart = D3DXVec3LengthSq(&(_pCurrentNode->vecPosition - _pPreviousNode->vecPosition));
			}
			float fCostToDestination = D3DXVec3LengthSq(&(_rVecTarget - _pCurrentNode->vecPosition));
			fNodeValue = fCostFromStart + fCostToDestination;

			//Check UP
			if (_pCurrentNode->pUp)
			{
				float fUpValue = GetAStarNodeValue(_pCurrentNode->pUp, _pCurrentNode, _rVecTarget, _iTreeDepth - 1);
				fChildValue = fUpValue;
				pNextNode = _pCurrentNode->pUp;
			}
			//Check DOWN
			if (_pCurrentNode->pDown)
			{
				float fDownValue = GetAStarNodeValue(_pCurrentNode->pDown, _pCurrentNode, _rVecTarget, _iTreeDepth - 1);
				if (fDownValue < fChildValue)
				{
					fChildValue = fDownValue;
					pNextNode = _pCurrentNode->pDown;
				}
			}
			//Check LEFT
			if (_pCurrentNode->pLeft)
			{
				float fLeftValue = GetAStarNodeValue(_pCurrentNode->pLeft, _pCurrentNode, _rVecTarget, _iTreeDepth - 1);
				if (fLeftValue < fChildValue)
				{
					fChildValue = fLeftValue;
					pNextNode = _pCurrentNode->pLeft;
				}
			}
			//Check RIGHT
			if (_pCurrentNode->pRight)
			{
				float fRightValue = GetAStarNodeValue(_pCurrentNode->pRight, _pCurrentNode, _rVecTarget, _iTreeDepth - 1);
				if (fRightValue < fChildValue)
				{
					fChildValue = fRightValue;
					pNextNode = _pCurrentNode->pRight;
				}
			}
		}
	}

	return (fNodeValue + fChildValue);
}
/**
*
* CAIHiveMind class Returns a pointer to the navigation grid
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the navigation grid
*
*/
TGridNode*
CAIHiveMind::GetNavigationGrid()
{
	return m_pNavigationGrid;
}
/**
*
* CAIHiveMind class Returns the specified AI controller
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the AI controller
*
*/
CAIController* 
CAIHiveMind::GetAI(int _iIndex) const
{
	return m_pAI[_iIndex];
}
/**
*
* CAIHiveMind class Returns the size of the navigation grid
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the grid size
*
*/
int
CAIHiveMind::GetNavigationGridSize() const
{
	return m_iGridSize;
}
/**
*
* CAIHiveMind class Returns the number of AI controllers
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the AI controller count
*
*/
int 
CAIHiveMind::GetAICount() const
{
	return m_iNumAI;
}
/**
*
* CAIHiveMind class Returns a struct defining a specific AI
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the AI description struct
*
*/
TAIDescription& 
CAIHiveMind::GetAIDesc(EAIType _eAIType)
{
	return m_pAIDescriptions[_eAIType];
}
/**
*
* CAIHiveMind class Clears the hivemind of all controllers and obstacles
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CAIHiveMind::ClearHivemind()
{
	if (m_pAI)
	{
		for (int iAI = 0; iAI < m_iNumAI; ++iAI)
		{
			delete m_pAI[iAI];
			m_pAI[iAI] = 0;
		}
		m_iNumAI = 0;
		delete[] m_pAI;
		m_pAI = 0;
	}
	m_vecStaticObstacles.clear();
}
/**
*
* CAIHiveMind class Change the processing method used for AI calculation
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _eProcessingMethod Processing method
*
*/
void
CAIHiveMind::ChangeProcessingMethod(EProcessingMethod _eProcessingMethod)
{
	m_eProcessingMethod = _eProcessingMethod;
}
/**
*
* CAIHiveMind class Recalculates the navigation grid (usually called after changes have been made)
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DX11 Device
*
*/
void
CAIHiveMind::RecalculateNavGrid(ID3D11Device* _pDevice)
{
	D3DXCOLOR deactiveColour(1.5f, 0.1f, 0.1f, 0.5f);
	D3DXCOLOR lineColour(0.2f, 0.2f, 0.8f, 0.2f);

	int iCurrentGrid = 0;
	for (int iHeight = 0; iHeight < m_iHeight; ++iHeight)
	{
		for (int iWidth = 0; iWidth < m_iWidth; ++iWidth)
		{
			m_pNavigationGrid[iCurrentGrid].bIsActive = true;
			m_pNavigationGridMesh->GetPointSprite(iCurrentGrid)->colour = lineColour;
			for (unsigned int iObstalce = 0; iObstalce < m_vecStaticObstacles.size(); ++iObstalce)
			{
				D3DXVECTOR3 vecToObstacle = m_vecStaticObstacles[iObstalce]->GetPosition() - m_pNavigationGrid[iCurrentGrid].vecPosition;
				vecToObstacle.y = 0.0f;
				if (D3DXVec3LengthSq(&vecToObstacle) < m_vecStaticObstacles[iObstalce]->GetRadius() * 0.2f)
				{
					//Deactivate this grid element
					m_pNavigationGrid[iCurrentGrid].bIsActive = false;
					m_pNavigationGridMesh->GetPointSprite(iCurrentGrid)->colour = deactiveColour;
				}
			}
			++iCurrentGrid;
		}
	}
	m_pNavigationGridMesh->RefreshBuffers(_pDevice);
}