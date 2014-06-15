//
//  File Name   :   AIHiveMind.h
//  Description :   Class of CAIHiveMind
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __AIHIVEMIND_H__
#define __AIHIVEMIND_H__

// Library Includes
#include <vector>

// Local Includes
#include "defines.h"

class CAIHiveMind;
// Types
struct TAIDescription
{
	TAIDescription()
	{
		fMovementSpeed = 0.0f;
		fRotationSpeed = 0.0f;
	}
	TAIDescription(float _fMovement, float _fRotation)
	{
		fMovementSpeed = _fMovement;
		fRotationSpeed = _fRotation;
	}
	float fMovementSpeed;
	float fRotationSpeed;
};
struct TAIThreadData
{
	TAIThreadData(CAIHiveMind* _pHivemind, int _iAIIndex, float _fDeltaTime)
	{
		pThis = _pHivemind;
		iAIIndex = _iAIIndex;
		fDeltaTime = _fDeltaTime;
	}
	CAIHiveMind* pThis;
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
	D3DXVECTOR3 vecPosition;
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

class CShader;
class CRenderEntity;
class CAIController;
class CPointSprite;
class CEntityManager;
class CThreadPool;
class CAICLKernel;
class COpenCLContext;

class CAIHiveMind
{
public:
	CAIHiveMind();
	virtual ~CAIHiveMind();

	virtual bool Initialise(COpenCLContext* _pCLKernel);
	void Process(COpenCLContext* _pCLKernel, CThreadPool* _pCThreadPool, float _fDeltaTime);
	void ProcessIndividualAIMovement(int _iAIIndex, float _fDeltaTime);
	void ProcessIndividualAIController(int _iAIIndex, float _fDeltaTime);
	void ProcessOpenCLKernel(COpenCLContext* _pCLKernel, float _fDeltaTime);
	void AddAI(CRenderEntity* _pEntity, EAIType _eAIType);
	void AddStaticObject(ID3D11Device* _pDevice, CRenderEntity* _pObject);

	void CreateNavigationGrid(ID3D11Device* _pDevice, CEntityManager* _pEntityManager, CShader* _pShader, float _fGridScale, int _iWidth, int _iHeight);
	D3DXVECTOR3& GetRandomWaypoint() const;
	D3DXVECTOR3* GetNextWaypoint(D3DXVECTOR3& _rVecTarget, int& _iCurrentWaypoint);
	float GetAStarNodeValue(TGridNode* _pCurrentNode, TGridNode* _pPreviousNode, D3DXVECTOR3& _rVecTarget, int _iTreeDepth);
	TGridNode* GetNavigationGrid();
	CAIController* GetAI(int _iIndex) const;
	int GetNavigationGridSize() const;
	int GetAICount() const;

	TAIDescription& GetAIDesc(EAIType _eAIType);
	void ClearHivemind();
	void ChangeProcessingMethod(EProcessingMethod _eProcessingMethod);
	void RecalculateNavGrid(ID3D11Device* _pDevice);

private:
	CAIHiveMind(const CAIHiveMind& _krInstanceToCopy);
	const CAIHiveMind& operator =(const CAIHiveMind& _krInstanceToCopy);

private:
	EProcessingMethod m_eProcessingMethod;

	CAICLKernel* m_pCLKernel;
	CAIController** m_pAI;
	std::vector<CRenderEntity*> m_vecStaticObstacles;
	int m_iNumAI;

	TAIDescription* m_pAIDescriptions;
	std::vector<TAIThreadData*> m_vecThreadData;

	TGridNode* m_pNavigationGrid;
	CPointSprite* m_pNavigationGridMesh;
	int m_iGridSize;
	int m_iWidth;
	int m_iHeight;

	
};

#endif // __AIHIVEMIND_H__