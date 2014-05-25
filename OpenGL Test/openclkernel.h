

#pragma once

#ifndef __OPENCLKERNEL_H__
#define __OPENCLKERNEL_H__

#include <CL\opencl.h>

class COpenCLKernel
{
//Member functions
public:
	COpenCLKernel();
	~COpenCLKernel();

	void InitialiseOpenCL();
	void LoadProgram(char* _pcCLProgram, char* _pcFunctionName);
	void SendDataToGPU();
	void Run();
	void RetrieveOpenCLResults();

protected:
	void BuildExecutable();
	cl_int GetPlatformID(cl_platform_id* _pPlatformID);
	char* GetCLFileContents(const char* _pcFilename, int& _iLength);
	const char* GetErrorString(cl_int _iError);

//Member variables
protected:	
	cl_context m_clContext;
	cl_command_queue m_clCommandQueue;
	cl_program m_clProgram;
	cl_kernel m_clKernel;
	
	cl_platform_id m_clPlatformID;
	cl_device_id* m_pDevices;

	cl_uint m_iNumDevices;
	cl_event m_clEvent;

	size_t workGroupSize[1];
	unsigned int m_iDeviceUsed;

	//Memory 
	cl_mem m_clArrayA;
	cl_mem m_clArrayB;
	cl_mem m_clArrayOut;
	int m_iArraySize;
};

#endif