//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
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

class CGrass : public CModel
{
	//Member functions
public:
	CGrass();
	virtual ~CGrass();

	void RecreateGrassMesh(ID3D11Device* _pDevice, D3DXVECTOR3& _rCameraPos, CRenderEntity** _pEntities, int _iNumEntities, float _fDeltaTime);
	
	//Member variables
protected:
	float m_fGrassSpeed;
	float m_fGrassStiffness;
};

#endif //SPRITE_H__