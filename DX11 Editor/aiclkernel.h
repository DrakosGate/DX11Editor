

#pragma once

#ifndef __AICLKERNEL_H__
#define __AICLKERNEL_H__

#include <CL\opencl.h>

#include "openclkernel.h"

class CAIHiveMind;
class COpenCLContext;

class CAICLKernel : public COpenCLKernel
{
//Member functions
public:
	CAICLKernel();
	virtual ~CAICLKernel();

	virtual void SendDataToGPU(COpenCLContext* _pOpenCL, CAIHiveMind* _pHiveMind, float _fDeltaTime);
	void RetrieveOpenCLResults(COpenCLContext* _pOpenCL, CAIHiveMind* _pHiveMind);

//Member variables
protected:	
	//Memory 
	cl_mem m_clInPos;
	cl_mem m_clInDir;
	cl_mem m_clInWaypoint;
	cl_mem m_clAIData;

	cl_mem m_clOutPos;
	cl_mem m_clOutDir;
};

#endif