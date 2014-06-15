

#pragma once

#ifndef __OPENCLCONTEXT_H__
#define __OPENCLCONTEXT_H__

#include <CL\opencl.h>

class COpenCLContext
{
//Member functions
public:
	COpenCLContext();
	~COpenCLContext();

	virtual void InitialiseOpenCL();
	virtual void CreateBuffers();
	virtual void LoadProgram(cl_program& _rProgram, cl_kernel& _rKernel, char* _pcCLProgram, char* _pcFunctionName);
	virtual void SendDataToGPU();
	virtual void Run(cl_kernel& _rKernel);
	virtual void RetrieveOpenCLResults();
	virtual void WaitForFinish();

	cl_context& GetCLContext();
	cl_command_queue& GetCLCommandQueue();
	cl_event& GetCLEvent();

	void SetCLWorkGroupSize(size_t* _pWorkGroupSize, int _iNumDimensions);

protected:
	void BuildExecutable(cl_program& _rProgram);
	cl_int GetPlatformID(cl_platform_id* _pPlatformID);
	char* GetCLFileContents(const char* _pcFilename, int& _iLength);
	const char* GetErrorString(cl_int _iError);

//Member variables
protected:	
	cl_context m_clContext;
	cl_command_queue m_clCommandQueue;
	
	cl_platform_id m_clPlatformID;
	cl_device_id* m_pDevices;

	cl_uint m_iNumDevices;
	cl_event m_clEvent;

	size_t* m_pWorkGroupSize;
	size_t m_iGlodalWorkSize[1];
	int m_iNumDimensions;

	unsigned int m_iDeviceUsed;

	//Memory 
	int m_iArraySize;
};

#endif//__OPENCLCONTEXT_H__