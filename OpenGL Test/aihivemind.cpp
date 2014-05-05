//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
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

// This Include
#include "aihivemind.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CAIHiveMind class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CAIHiveMind::CAIHiveMind()
: m_pAI(0)
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
	m_pAIDescriptions[AI_CREATURE] = TAIDescription(0.3f, 0.1f);
	
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
CAIHiveMind::Process(float _fDeltaTime)
{
	D3DXVECTOR3 vecAvoidance;
	for(int iAI = 0; iAI < m_iNumAI; ++iAI)
	{
		vecAvoidance *= 0.0f;
		for(int iOther = iAI; iOther < m_iNumAI; ++iOther)
		{
			if(m_pAI[iAI]->GetEntity()->HasCollided(m_pAI[iOther]->GetEntity()))
			{
				vecAvoidance += (m_pAI[iAI]->GetEntity()->GetPosition() - m_pAI[iOther]->GetEntity()->GetPosition()) * 0.75f;
			}
		}
		for(unsigned int iStatic = 0; iStatic < m_vecStaticObstacles.size(); ++iStatic)
		{
			if(m_pAI[iAI]->GetEntity()->HasCollided(m_vecStaticObstacles[iStatic]))
			{
				vecAvoidance += (m_pAI[iAI]->GetEntity()->GetPosition() - m_vecStaticObstacles[iStatic]->GetPosition()) * 0.75f;
			}
		}
		vecAvoidance.y = 0.0f;
		m_pAI[iAI]->Process(_fDeltaTime, vecAvoidance);
	}
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

	D3DXCOLOR deactiveColour(1.5f, 0.1f, 0.1f, 0.8f);
	int iCurrentObstacle = 0;
	bool bHasDeactivatedGrid = false;
	for (int iHeight = 0; iHeight < m_iHeight; ++iHeight)
	{
		for (int iWidth = 0; iWidth < m_iWidth; ++iWidth)
		{
			if (m_pNavigationGrid[iCurrentObstacle].bIsActive)
			{
				D3DXVECTOR3 vecToObstacle = _pObject->GetPosition() - m_pNavigationGrid[iCurrentObstacle].vecPosition;
				vecToObstacle.y = 0.0f;
				if (D3DXVec3LengthSq(&vecToObstacle) < _pObject->GetRadius() * _pObject->GetRadius())
				{
					//Deactivate this grid element
					m_pNavigationGrid[iCurrentObstacle].bIsActive = false;
					//m_pNavigationGridMesh->GetPointSprite(iCurrentObstacle)->colour = deactiveColour;
					bHasDeactivatedGrid = true;
				}
			}
			++iCurrentObstacle;
		}
	}
	if (bHasDeactivatedGrid)
	{
		m_pNavigationGridMesh->RefreshBuffers(_pDevice);
	}
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

			if (iCurrentGrid > 0.0f && iWidth > 0)
			{
				m_pNavigationGridMesh->AddPointSprite(_pDevice, m_pNavigationGrid[iCurrentGrid].vecPosition, m_pNavigationGrid[iCurrentGrid - 1].vecPosition - m_pNavigationGrid[iCurrentGrid].vecPosition, D3DXVECTOR2(100.0f, 100.0f), lineColour, 0.0f, 0);
			}
			if (iHeight > 0)
			{
				m_pNavigationGridMesh->AddPointSprite(_pDevice, m_pNavigationGrid[iCurrentGrid].vecPosition, m_pNavigationGrid[((iHeight - 1) * m_iHeight) + iWidth].vecPosition - m_pNavigationGrid[iCurrentGrid].vecPosition, D3DXVECTOR2(100.0f, 100.0f), lineColour, 0.0f, 0);
			}
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
int
CAIHiveMind::GetNavigationGridSize() const
{
	return m_iGridSize;
}