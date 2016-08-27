

#pragma once

#ifndef __OPENCLKERNEL_H__
#define __OPENCLKERNEL_H__

#include <CL\opencl.h>

class OpenCLContext;

class OpenCLKernel
{
//Member functions
public:
	OpenCLKernel();
	virtual ~OpenCLKernel();

	cl_kernel& GetCLKernel();
	cl_program& GetCLProgram();

//Member variables
protected:	
	cl_kernel m_clKernel;
	cl_program m_clProgram;

	size_t* m_pWorkGroup;
	size_t* m_pGlobalGroup;
	int m_iArraySize;
};

#endif//__OPENCLKERNEL_H__