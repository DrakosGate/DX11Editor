//
//
//#pragma once
//
//#ifndef __AICLKERNEL_H__
//#define __AICLKERNEL_H__
//
//#include <CL\opencl.h>
//
//#include "openclkernel.h"
//
//class AIHiveMind;
//class OpenCLContext;
//
//class AICLKernel : public OpenCLKernel
//{
//	//Member functions
//public:
//	AICLKernel();
//	virtual ~AICLKernel();
//
//	virtual void SendDataToGPU( OpenCLContext* _pOpenCL, AIHiveMind* _pHiveMind, float _fDeltaTime );
//	void RetrieveOpenCLResults( OpenCLContext* _pOpenCL, AIHiveMind* _pHiveMind );
//
//	//Member variables
//protected:
//	//Memory 
//	cl_mem m_clInPos;
//	cl_mem m_clInDir;
//	cl_mem m_clInWaypoint;
//	cl_mem m_clAIData;
//
//	cl_mem m_clOutPos;
//	cl_mem m_clOutDir;
//};
//
//#endif