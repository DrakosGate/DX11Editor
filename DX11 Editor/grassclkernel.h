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
// Local Includes
#include "openclkernel.h"

// Types

// Constants

// Prototypes
class CGrass;
class COpenCLContext;
class CAIHiveMind;
class RenderEntity;

class CGrassCLKernel : public COpenCLKernel
{
//Member functions
public:
	CGrassCLKernel();
	virtual ~CGrassCLKernel();
	 
	virtual void CreateBuffers(COpenCLContext* _pOpenCL, CGrass* _pGrass, std::vector<RenderEntity*>* _pCollisionObjects);
	virtual void SendDataToGPU(COpenCLContext* _pOpenCL, CGrass* _pGrass, std::vector<RenderEntity*>* _pCollisionObjects, float _fDeltaTime);
	void RetrieveOpenCLResults(COpenCLContext* _pOpenCL, CGrass* _pGrass);

//Member variables
protected:	
	//Memory 
	cl_mem m_clInPos;
	cl_mem m_clInDir;
	cl_mem m_clInObjects;
	cl_mem m_clOutDir;

	Math::Vector4* m_pPositions;
	Math::Vector4* m_pDirections;
	Math::Vector4* m_pObjectData;
	Math::Vector4* m_pOutDirections;

	int m_iNumObstacles;
};

#endif//__GRASSCLKERNEL_H__