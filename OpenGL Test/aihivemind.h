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
struct TGridNode
{
	D3DXVECTOR3 vecPosition;
	bool bIsActive;
};
// Constants

// Prototypes
struct ID3D11Device;

class CShader;
class CRenderEntity;
class CAIController;
class CPointSprite;
class CEntityManager;

class CAIHiveMind
{
public:
	CAIHiveMind();
	virtual ~CAIHiveMind();

	virtual bool Initialise();
	void Process(float _fDeltaTime);
	void AddAI(CRenderEntity* _pEntity, EAIType _eAIType);
	void AddStaticObject(ID3D11Device* _pDevice, CRenderEntity* _pObject);

	void CreateNavigationGrid(ID3D11Device* _pDevice, CEntityManager* _pEntityManager, CShader* _pShader, float _fGridScale, int _iWidth, int _iHeight);
	D3DXVECTOR3& GetRandomWaypoint() const;
	TGridNode* GetNavigationGrid();
	int GetNavigationGridSize() const;

	TAIDescription& GetAIDesc(EAIType _eAIType);
	
private:
	CAIHiveMind(const CAIHiveMind& _krInstanceToCopy);
	const CAIHiveMind& operator =(const CAIHiveMind& _krInstanceToCopy);

private:
	CAIController** m_pAI;
	std::vector<CRenderEntity*> m_vecStaticObstacles;
	int m_iNumAI;

	TAIDescription* m_pAIDescriptions;

	TGridNode* m_pNavigationGrid;
	CPointSprite* m_pNavigationGridMesh;
	int m_iGridSize;
	int m_iWidth;
	int m_iHeight;

	
};

#endif // __AIHIVEMIND_H__