//
//  File Name   :   level.h
//  Description :   Class of CLevel
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __GRASSCLKERNEL_H__
#define __GRASSCLKERNEL_H__

// Library Includes
#include <vector>
#include <D3DX10.h>

// Local Includes
#include "openclkernel.h"

// Types

// Constants

// Prototypes
class CGrass;
class COpenCLContext;
class CAIHiveMind;
class CRenderEntity;

class CGrassCLKernel : public COpenCLKernel
{
//Member functions
public:
	CGrassCLKernel();
	virtual ~CGrassCLKernel();
	 
	virtual void CreateBuffers(COpenCLContext* _pOpenCL, CGrass* _pGrass, std::vector<CRenderEntity*>* _pCollisionObjects);
	virtual void SendDataToGPU(COpenCLContext* _pOpenCL, CGrass* _pGrass, std::vector<CRenderEntity*>* _pCollisionObjects, float _fDeltaTime);
	void RetrieveOpenCLResults(COpenCLContext* _pOpenCL, CGrass* _pGrass);

//Member variables
protected:	
	//Memory 
	cl_mem m_clInPos;
	cl_mem m_clInDir;
	cl_mem m_clInObjects;
	cl_mem m_clOutDir;

	D3DXVECTOR4* m_pPositions;
	D3DXVECTOR4* m_pDirections;
	D3DXVECTOR4* m_pObjectData;
	D3DXVECTOR4* m_pOutDirections;

	int m_iNumObstacles;
};

#endif//__GRASSCLKERNEL_H__