//
//  File Name   :   model.h
//  Description :   Class of Model
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


#pragma once

#ifndef __GRASS_H__
#define __GRASS_H__

// Library Includes

// Local Includes
#include "model.h"
#include "networkdefines.h"

// Types

// Constants

// Prototypes
//class CGrassCLKernel;
class OpenCLContext;
class ResourceManager;
class Network;

class Grass : public Model
{
	//Member functions
public:
	Grass();
	virtual ~Grass();
	bool Initialise(ID3D11Device* _pDevice, ResourceManager* _pResourceManager, int _iGrassDimensions, float _fScale, Math::Vector2& _rVecTiling, Math::Colour& _rColour, int _iProcessingDivisionSize);
	
	void ProcessDistrubuted(float _fDeltaTime);
	void ProcessOpenCL(float _fDeltaTime);
	void SendCollisionData(std::vector<RenderEntity*>* _pCollisionObjects);
	void ProcessGrassSection(int _iSection, float _fDeltaTime);
	void RecreateGrassMesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, Math::Vector3& _rCameraPos, std::vector<RenderEntity*>& _pEntities, float _fDeltaTime);
	
	int GetDimensionSize() const;

	//Member variables
protected:
	Network* m_pNetwork;

	std::vector<RenderEntity*>* m_pCollisionObjects;
	//CGrassCLKernel* m_pGrassCLKernel;
	Math::Vector3* m_pGrassVelocities;

	float m_fGrassSpeed;
	float m_fGrassStiffness;

	int m_iGrassDimensions;
	int m_iDivisionSize;
};

#endif //SPRITE_H__