//
//  File Name   :   model.h
//  Description :   Class of CModel
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


#pragma once

#ifndef __GRASS_H__
#define __GRASS_H__

// Library Includes

// Local Includes
#include "model.h"

// Types

// Constants

// Prototypes
class CGrassCLKernel;
class CResourceManager;

class CGrass : public CModel
{
	//Member functions
public:
	CGrass();
	virtual ~CGrass();
	bool Initialise(ID3D11Device* _pDevice, CResourceManager* _pResourceManager, int _iTerrainWidth, int _iTerrainHeight, float _fScale, D3DXVECTOR2& _rVecTiling, D3DXCOLOR& _rColour, int _iProcessingDivisionSize);
	
	void ProcessOpenCL(float _fDeltaTime);
	void SendCollisionData(std::vector<CRenderEntity*>* _pCollisionObjects);
	void ProcessGrassSection(int _iSection, float _fDeltaTime);
	void RecreateGrassMesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, D3DXVECTOR3& _rCameraPos, std::vector<CRenderEntity*>& _pEntities, float _fDeltaTime);
	
	//Member variables
protected:
	std::vector<CRenderEntity*>* m_pCollisionObjects;
	CGrassCLKernel* m_pGrassCLKernel;

	float m_fGrassSpeed;
	float m_fGrassStiffness;

	int m_iDivisionSize;
};

#endif //SPRITE_H__