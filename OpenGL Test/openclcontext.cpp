#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

#include "openclcontext.h"

COpenCLContext::COpenCLContext()
{

}
COpenCLContext::~COpenCLContext()
{
	printf("==== Cleaning Up OpenCL ====\n");
	if (m_clCommandQueue)
	{
		clReleaseCommandQueue(m_clCommandQueue);

		clReleaseContext(m_clContext);
		delete[] m_pDevices;
	}
}
void
COpenCLContext::InitialiseOpenCL()
{
	cl_int iError;
	printf("==== Initialising OpenCL ====\n");

	iError = GetPlatformID(&m_clPlatformID);
	iError = clGetDeviceIDs(m_clPlatformID, CL_DEVICE_TYPE_GPU, 0, NULL, &m_iNumDevices);
	
	m_pDevices = new cl_device_id[m_iNumDevices];
	iError = clGetDeviceIDs(m_clPlatformID, CL_DEVICE_TYPE_GPU, m_iNumDevices, m_pDevices, NULL);

	m_clContext = clCreateContext(0, 1, m_pDevices, NULL, NULL, &iError);
	printf("-\tCreating OpenCL Context: %s\n", GetErrorString(iError));

	m_iDeviceUsed = 0;
	m_clCommandQueue = clCreateCommandQueue(m_clContext, m_pDevices[m_iDeviceUsed], 0, &iError);
}
void
COpenCLContext::LoadProgram(cl_program& _rProgram, cl_kernel& _rKernel, char* _pcCLProgram, char* _pcFunctionName)
{
	cl_int iError = 0;
	printf("-\tLoading OpenCL Program: %s\n", _pcCLProgram);
	size_t programLength;

	int iStringLength = 0;
	char* pCLSource = GetCLFileContents(_pcCLProgram, iStringLength);
	programLength = static_cast<size_t>(iStringLength);

	_rProgram = clCreateProgramWithSource(m_clContext, 1, (const char**)&pCLSource, &programLength, &iError);
	printf("-\tCreating Program with source: %s\n", GetErrorString(iError));

	BuildExecutable(_rProgram);

	_rKernel = clCreateKernel(_rProgram, _pcFunctionName, &iError);
	printf("-\tCreating OpenCL Kernel %s: %s\n", _pcFunctionName, GetErrorString(iError));

	CreateBuffers();

	delete[] pCLSource;
	pCLSource = 0;
}
void COpenCLContext::CreateBuffers()
{
	//cl_int iError = 0;
	//m_clArrayA = clCreateBuffer(m_clContext, CL_MEM_READ_ONLY, sizeof(float)* m_iArraySize, NULL, &iError);
	//printf("-\tA Buffer: %s\n", GetErrorString(iError));
	//m_clArrayB = clCreateBuffer(m_clContext, CL_MEM_READ_ONLY, sizeof(float)* m_iArraySize, NULL, &iError);
	//printf("-\tB Buffer: %s\n", GetErrorString(iError));
	//m_clArrayOut = clCreateBuffer(m_clContext, CL_MEM_WRITE_ONLY, sizeof(float)* m_iArraySize, NULL, &iError);
	//printf("-\tOut Buffer: %s\n", GetErrorString(iError));
}
void
COpenCLContext::SendDataToGPU()
{
	//cl_int iError = 0;
	//m_iArraySize = 20;
	//float* pArrayA = new float[m_iArraySize];
	//float* pArrayB = new float[m_iArraySize];
	//for (int i = 0; i < m_iArraySize; ++i)
	//{
	//	pArrayA[i] = i * 1.0f;
	//	pArrayB[i] = i * 2.0f;
	//}
	//
	////Fill array B buffer
	//clEnqueueWriteBuffer(m_clCommandQueue, m_clArrayA, CL_TRUE, 0, sizeof(float)* m_iArraySize, pArrayA, 0, NULL, &m_clEvent);
	//clEnqueueWriteBuffer(m_clCommandQueue, m_clArrayB, CL_TRUE, 0, sizeof(float)* m_iArraySize, pArrayB, 0, NULL, &m_clEvent);
	//clReleaseEvent(m_clEvent);
	//
	//iError = clSetKernelArg(m_clKernel, 0, sizeof(cl_mem), (void*)&m_clArrayA);
	//printf("-\tA: %s\n", GetErrorString(iError));
	//iError = clSetKernelArg(m_clKernel, 1, sizeof(cl_mem), (void*)&m_clArrayB);
	//printf("-\tB: %s\n", GetErrorString(iError));
	//iError = clSetKernelArg(m_clKernel, 2, sizeof(cl_mem), (void*)&m_clArrayOut);
	//printf("-\tOut: %s\n", GetErrorString(iError));
	////Wait for this to finish before continuing
	//clFinish(m_clCommandQueue);
	//
	////Set work group size to the size of this calculation
	//workGroupSize[0] = m_iArraySize;
	//
	//delete[] pArrayA;
	//delete[] pArrayB;
}
void
COpenCLContext::Run(cl_kernel& _rKernel)
{
	//Run Kernel
	cl_int iError = 0;
	iError = clEnqueueNDRangeKernel(m_clCommandQueue, _rKernel, m_iNumDimensions, NULL, m_pWorkGroupSize, NULL, 0, NULL, &m_clEvent);

	
	clReleaseEvent(m_clEvent);
	//WaitForFinish();
	/*
	//Run Kernel
	cl_int iError = 0;
	iError = clEnqueueNDRangeKernel(m_clCommandQueue,	//Command Queue
									_rKernel,			//Kernel
									m_iNumDimensions,	//Number of work dimensions
									NULL,				//Global work offset
									m_iGlodalWorkSize,	//Global work size
									m_pWorkGroupSize,	//Local work size
									0,					//Number of events in wait list
									NULL,				//Wait list
									&m_clEvent);		//Event
	
	clReleaseEvent(m_clEvent);
	WaitForFinish();
	*/
}
void 
COpenCLContext::RetrieveOpenCLResults()
{
	//Retrieve data from calculations
	//float* pResult = new float[m_iArraySize];
	//cl_int iError = clEnqueueReadBuffer(m_clCommandQueue, m_clArrayOut, CL_TRUE, 0, sizeof(float)* m_iArraySize, pResult, 0, NULL, &m_clEvent);
	//printf("-\tReading from buffer: %s\n", GetErrorString(iError));
	//clReleaseEvent(m_clEvent);
	//printf("Result:\n");
	//for (int i = 0; i < m_iArraySize; ++i)
	//{
	//	printf("%g\n", pResult[i]);
	//}
	//delete[] pResult;
}
void
COpenCLContext::WaitForFinish()
{
	//Wait for this to finish
	clFinish(m_clCommandQueue);
}
cl_context&
COpenCLContext::GetCLContext()
{
	return m_clContext;
}
cl_command_queue&
COpenCLContext::GetCLCommandQueue()
{
	return m_clCommandQueue;
}
cl_event& 
COpenCLContext::GetCLEvent()
{
	return m_clEvent;
}
void
COpenCLContext::SetCLWorkGroupSize(size_t* _pWorkGroupSize, int _iNumDimensions)
{
	m_iGlodalWorkSize[0] = 1;
	for (int iGroup = 0; iGroup < _iNumDimensions; ++iGroup)
	{
		m_iGlodalWorkSize[0] *= _pWorkGroupSize[iGroup];
	}
	m_pWorkGroupSize = _pWorkGroupSize;
	m_iNumDimensions = _iNumDimensions;
}
void 
COpenCLContext::BuildExecutable(cl_program& _rProgram)
{
	cl_int iError = 0;
	iError = clBuildProgram(_rProgram, 0, NULL, NULL, NULL, NULL);
	printf("-\tBuilding program executable: %s\n", GetErrorString(iError));

	cl_build_status buildStats;
	iError = clGetProgramBuildInfo(_rProgram, m_pDevices[m_iDeviceUsed], CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &buildStats, NULL);
	printf("-\tProgram build statistics: %s\n", GetErrorString(iError));

	char* pBuildLog;
	size_t valueSize;
	iError = clGetProgramBuildInfo(_rProgram, m_pDevices[m_iDeviceUsed], CL_PROGRAM_BUILD_LOG, 0, NULL, &valueSize);
	pBuildLog = new char[valueSize + 1];
	iError = clGetProgramBuildInfo(_rProgram, m_pDevices[m_iDeviceUsed], CL_PROGRAM_BUILD_LOG, valueSize, pBuildLog, NULL);
	pBuildLog[valueSize] = '\0';
	printf("BUILD LOG: %s", pBuildLog);

	delete[] pBuildLog;
	pBuildLog = 0;
}
cl_int 
COpenCLContext::GetPlatformID(cl_platform_id* _pPlatformID)
{
	char cBuffer[1024];
	cl_uint iNumPlatforms;
	cl_platform_id* pPlatformIDs;
	cl_int iError;
	*_pPlatformID = NULL;
	
	iError = clGetPlatformIDs(0, NULL, &iNumPlatforms);
	if (iError == CL_SUCCESS)
	{
		if (iNumPlatforms == 0)
		{
			printf("ERROR: No OpenCL platform found!!\n");
		}
		else
		{
			pPlatformIDs = (cl_platform_id*)malloc(iNumPlatforms * sizeof(cl_platform_id));
			if (pPlatformIDs == NULL)
			{
				printf("ERROR: Could not allocate memory for cl_platform ID\n\n");
			}
			iError = clGetPlatformIDs(iNumPlatforms, pPlatformIDs, NULL);
			printf("\tAvailable OpenCL Platforms: \n");
			for (unsigned int iPlatform = 0; iPlatform < iNumPlatforms; ++iPlatform)
			{
				iError = clGetPlatformInfo(pPlatformIDs[iPlatform], CL_PLATFORM_NAME, 1024, &cBuffer, NULL);
				if (iError == CL_SUCCESS)
				{
					printf("\tPlatform Found: %s", cBuffer);
					if (strstr(cBuffer, "NVIDIA") != NULL)
					{
						*_pPlatformID = pPlatformIDs[iPlatform];
						break;
					}
				}
			}
			if (*_pPlatformID == NULL)
			{
				printf("ERROR: Could not find NVIDIA platform - using default platform instead\n");
				*_pPlatformID = pPlatformIDs[0];
			}
			free(pPlatformIDs);
		}
	}
	else
	{

	}
	return CL_SUCCESS;
}
char*
COpenCLContext::GetCLFileContents(const char* _pcFilename, int& _iLength)
{
	std::ifstream fileIn;
	fileIn.open(_pcFilename);

	char* pcBuffer = 0;
	std::string sFileString;

	if (fileIn.is_open())
	{
		char* lineBuffer = new char[256];
		while (fileIn.eof() == false)
		{
			//Read current file line into the line buffer
			fileIn.getline(lineBuffer, 256);	
			sFileString += lineBuffer;	//Add this line to the kernel string
		}
		delete[] lineBuffer;
		lineBuffer = 0;

		_iLength = sFileString.length() + 1;
		sFileString[_iLength - 1] = '\0';
		
		//Copy the kernel string to the file buffer
		pcBuffer = new char[_iLength];
		sprintf_s(pcBuffer, _iLength, "%s", sFileString.c_str());

		fileIn.close();
	}
	else
	{
		printf("Could not open OpenCL file %s", _pcFilename);
	}
	return pcBuffer;
}
const char* 
COpenCLContext::GetErrorString(cl_int _iError)
{
	static const char* errorString[] = {
		"CL_SUCCESS",
		"CL_DEVICE_NOT_FOUND",
		"CL_DEVICE_NOT_AVAILABLE",
		"CL_COMPILER_NOT_AVAILABLE",
		"CL_MEM_OBJECT_ALLOCATION_FAILURE",
		"CL_OUT_OF_RESOURCES",
		"CL_OUT_OF_HOST_MEMORY",
		"CL_PROFILING_INFO_NOT_AVAILABLE",
		"CL_MEM_COPY_OVERLAP",
		"CL_IMAGE_FORMAT_MISMATCH",
		"CL_IMAGE_FORMAT_NOT_SUPPORTED",
		"CL_BUILD_PROGRAM_FAILURE",
		"CL_MAP_FAILURE",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"CL_INVALID_VALUE",
		"CL_INVALID_DEVICE_TYPE",
		"CL_INVALID_PLATFORM",
		"CL_INVALID_DEVICE",
		"CL_INVALID_CONTEXT",
		"CL_INVALID_QUEUE_PROPERTIES",
		"CL_INVALID_COMMAND_QUEUE",
		"CL_INVALID_HOST_PTR",
		"CL_INVALID_MEM_OBJECT",
		"CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
		"CL_INVALID_IMAGE_SIZE",
		"CL_INVALID_SAMPLER",
		"CL_INVALID_BINARY",
		"CL_INVALID_BUILD_OPTIONS",
		"CL_INVALID_PROGRAM",
		"CL_INVALID_PROGRAM_EXECUTABLE",
		"CL_INVALID_KERNEL_NAME",
		"CL_INVALID_KERNEL_DEFINITION",
		"CL_INVALID_KERNEL",
		"CL_INVALID_ARG_INDEX",
		"CL_INVALID_ARG_VALUE",
		"CL_INVALID_ARG_SIZE",
		"CL_INVALID_KERNEL_ARGS",
		"CL_INVALID_WORK_DIMENSION",
		"CL_INVALID_WORK_GROUP_SIZE",
		"CL_INVALID_WORK_ITEM_SIZE",
		"CL_INVALID_GLOBAL_OFFSET",
		"CL_INVALID_EVENT_WAIT_LIST",
		"CL_INVALID_EVENT",
		"CL_INVALID_OPERATION",
		"CL_INVALID_GL_OBJECT",
		"CL_INVALID_BUFFER_SIZE",
		"CL_INVALID_MIP_LEVEL",
		"CL_INVALID_GLOBAL_WORK_SIZE",
	};
	const int iErrorCount = sizeof(errorString) / sizeof(errorString[0]);
	const int iIndex = -_iError;

	const char* pcSuccessMessage = "SUCCESS";
	if (iIndex >= 0 && iIndex < iErrorCount)
	{
		return errorString[_iError];
	}
	else
	{
		return pcSuccessMessage;
	}
}