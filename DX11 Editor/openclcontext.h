

#pragma once

#ifndef __OPENCLCONTEXT_H__
#define __OPENCLCONTEXT_H__

#include <CL\opencl.h>

class OpenCLContext
{
//Member functions
public:
	OpenCLContext();
	~OpenCLContext();

	virtual void InitialiseOpenCL();
	virtual void LoadProgram(cl_program& _rProgram, cl_kernel& _rKernel, char* _pcCLProgram, char* _pcFunctionName);
	virtual void Run(cl_kernel& _rKernel);
	virtual void WaitForFinish();

	cl_context& GetCLContext();
	cl_command_queue& GetCLCommandQueue();
	cl_event& GetCLEvent();

	void SetCLWorkGroupSize(size_t* _pWorkGroupSize, size_t* _pGlobalWorkSize, int _iNumDimensions);

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
	size_t* m_pGlodalWorkSize;
	int m_iNumDimensions;

	unsigned int m_iDeviceUsed;

	//Memory 
	int m_iArraySize;
};

#endif//__OPENCLCONTEXT_H__