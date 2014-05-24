

#pragma once

#ifndef __AICLKERNEL_H__
#define __AICLKERNEL_H__

#include "openclkernel.h"

class CAIHiveMind;

class CAICLKernel : public COpenCLKernel
{
//Member functions
public:
	CAICLKernel();
	virtual ~CAICLKernel();
	 
	virtual void SendDataToGPU(CAIHiveMind* _pHiveMind, float _fDeltaTime);
	void RetrieveOpenCLResults(CAIHiveMind* _pHiveMind);

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

#endif