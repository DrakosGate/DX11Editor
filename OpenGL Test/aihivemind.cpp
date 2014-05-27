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
	m_pAIDescriptions[AI_HUMAN] = TAIDescription(1.0f, 1.0f);
	m_pAIDescriptions[AI_CHICKEN] = TAIDescription(0.3f, 0.1f);

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
	for (int iOther = _iAIIndex; iOther < m_iNumAI; ++iOther)
	{
		if (m_pAI[_iAIIndex]->GetEntity()->HasCollided(m_pAI[iOther]->GetEntity()))
		{
			vecAvoidance += (m_pAI[_iAIIndex]->GetEntity()->GetPosition() - m_pAI[iOther]->GetEntity()->GetPosition()) * 0.75f;
		}
	}
	for (unsigned int iStatic = 0; iStatic < m_vecStaticObstacles.size(); ++iStatic)
	{
		if (m_pAI[_iAIIndex]->GetEntity()->HasCollided(m_vecStaticObstacles[iStatic]))
		{
			vecAvoidance += (m_pAI[_iAIIndex]->GetEntity()->GetPosition() - m_vecStaticObstacles[iStatic]->GetPosition()) * 0.75f;
		}
	}
	vecAvoidance.y = 0.0f;
	m_pAI[_iAIIndex]->Process(_fDeltaTime, vecAvoidance);
}
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

			m_pNavigationGridMesh->AddPointSprite(_pDevice, m_pNavigationGrid[iCurrentGrid].vecPosition, D3DXVECTOR3(0.0f, 0.1f, 0.0f), D3DXVECTOR2(100.0f, 100.0f), lineColour, 0.0f, 0);
			
			++iCurrentGrid;
		}
	}
	_pEntityManager->AddEntity(m_pNavigationGridMesh, SCENE_DEBUG);
}
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
TGridNode*
CAIHiveMind::GetNavigationGrid()
{
	return m_pNavigationGrid;
}
CAIController* 
CAIHiveMind::GetAI(int _iIndex) const
{
	return m_pAI[_iIndex];
}
int
CAIHiveMind::GetNavigationGridSize() const
{
	return m_iGridSize;
}
int 
CAIHiveMind::GetAICount() const
{
	return m_iNumAI;
}
TAIDescription& 
CAIHiveMind::GetAIDesc(EAIType _eAIType)
{
	return m_pAIDescriptions[_eAIType];
}
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
void
CAIHiveMind::ChangeProcessingMethod(EProcessingMethod _eProcessingMethod)
{
	m_eProcessingMethod = _eProcessingMethod;
}
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