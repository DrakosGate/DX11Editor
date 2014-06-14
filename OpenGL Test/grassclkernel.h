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

// Local Includes
#include "openclkernel.h"

// Types

// Constants

// Prototypes
class CGrass;
class CAIHiveMind;

class CGrassCLKernel : public COpenCLKernel
{
//Member functions
public:
	CGrassCLKernel();
	virtual ~CGrassCLKernel();
	 
	virtual void SendDataToGPU(CGrass* _pGrass, float _fDeltaTime);
	void RetrieveOpenCLResults(CGrass* _pGrass);

//Member variables
private:	
	//Memory 
	cl_mem m_clInPos;
	cl_mem m_clInDir;
	cl_mem m_clInWaypoint;
	cl_mem m_clAIData;

	cl_mem m_clOutPos;
	cl_mem m_clOutDir;
	int m_iArraySize;
};

#endif//__GRASSCLKERNEL_H__