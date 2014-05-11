#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

#include "openclkernel.h"

COpenCLKernel::COpenCLKernel()
{

}
COpenCLKernel::~COpenCLKernel()
{
	printf("==== Cleaning Up OpenCL ====\n");
	if (m_clKernel)
	{
		clReleaseKernel(m_clKernel);
		clReleaseProgram(m_clProgram);
		clReleaseCommandQueue(m_clCommandQueue);

		clReleaseMemObject(m_clArrayA);
		clReleaseMemObject(m_clArrayB);
		clReleaseMemObject(m_clArrayOut);

		clReleaseContext(m_clContext);
		delete[] m_pDevices;
	}
}
void
COpenCLKernel::InitialiseOpenCL()
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
	m_clArrayA = 0;
	m_clArrayB = 0;
	m_clArrayOut = 0;
}
void
COpenCLKernel::LoadProgram(char* _pcCLProgram)
{
	cl_int iError = 0;
	printf("-\tLoading OpenCL Program: %s\n", _pcCLProgram);
	size_t programLength;

	int iStringLength = 0;
	char* pCLSource = GetCLFileContents(_pcCLProgram, iStringLength);
	programLength = static_cast<size_t>(iStringLength);

	m_clProgram = clCreateProgramWithSource(m_clContext, 1, (const char**)&pCLSource, &programLength, &iError);
	printf("-\tCreating Program with source: %s\n", GetErrorString(iError));

	BuildExecutable();
	delete[] pCLSource;
	pCLSource = 0;
}
void
COpenCLKernel::SendDataToGPU()
{
	cl_int iError = 0;
	m_clKernel = clCreateKernel(m_clProgram, "ArrayAdd", &iError);
	printf("-\tCreating OpenCL Kernel %s: %s", "ArrayAdd", GetErrorString(iError));

	m_iArraySize = 10;
	float* pArrayA = new float[m_iArraySize];
	float* pArrayB = new float[m_iArraySize];
	for (int i = 0; i < m_iArraySize; ++i)
	{
		pArrayA[i] = i * 1.0f;
		pArrayB[i] = i * 2.0f;
	}
	m_clArrayA = clCreateBuffer(m_clContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)* m_iArraySize, pArrayA, &iError);
	m_clArrayB = clCreateBuffer(m_clContext, CL_MEM_READ_ONLY, sizeof(float)* m_iArraySize, NULL, &iError);
	m_clArrayOut = clCreateBuffer(m_clContext, CL_MEM_WRITE_ONLY, sizeof(float)* m_iArraySize, NULL, &iError);

	//Fill array B buffer
	clEnqueueWriteBuffer(m_clCommandQueue, m_clArrayB, CL_TRUE, 0, sizeof(float)* m_iArraySize, pArrayB, 0, NULL, &m_clEvent);
	clReleaseEvent(m_clEvent);

	iError = clSetKernelArg(m_clKernel, 0, sizeof(cl_mem), (void*)&m_clArrayA);
	iError = clSetKernelArg(m_clKernel, 0, sizeof(cl_mem), (void*)&m_clArrayB);
	iError = clSetKernelArg(m_clKernel, 0, sizeof(cl_mem), (void*)&m_clArrayOut);
	//Wait for this to finish before continuing
	clFinish(m_clCommandQueue);

	//Set work group size to the size of this calculation
	workGroupSize[0] = m_iArraySize;

	delete[] pArrayA;
	delete[] pArrayB;
}
void
COpenCLKernel::Run()
{
	cl_int iError = 0;
	iError = clEnqueueNDRangeKernel(m_clCommandQueue, m_clKernel, 1, NULL, workGroupSize, NULL, 0, NULL, &m_clEvent);
	clReleaseEvent(m_clEvent);
	//Wait for this to finish
	clFinish(m_clCommandQueue);

	//Retrieve data from calculations
	float* pResult = new float[m_iArraySize];
	iError = clEnqueueReadBuffer(m_clCommandQueue, m_clArrayOut, CL_TRUE, 0, sizeof(float)* m_iArraySize, &pResult, 0, NULL, &m_clEvent);
	clReleaseEvent(m_clEvent);
	printf("Result:\n");
	for (int i = 0; i < m_iArraySize; ++i)
	{
		printf("%g\n", pResult[i]);
	}
}
void 
COpenCLKernel::BuildExecutable()
{
	cl_int iError = 0;
	iError = clBuildProgram(m_clProgram, 0, NULL, NULL, NULL, NULL);
	printf("-\tBuilding program executable: %s\n", GetErrorString(iError));

	cl_build_status buildStats;
	iError = clGetProgramBuildInfo(m_clProgram, m_pDevices[m_iDeviceUsed], CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &buildStats, NULL);

	char* pBuildLog;
	size_t valueSize;
	iError = clGetProgramBuildInfo(m_clProgram, m_pDevices[m_iDeviceUsed], CL_PROGRAM_BUILD_LOG, 0, NULL, &valueSize);
	pBuildLog = new char[valueSize + 1];
	iError = clGetProgramBuildInfo(m_clProgram, m_pDevices[m_iDeviceUsed], CL_PROGRAM_BUILD_LOG, valueSize, pBuildLog, NULL);
	pBuildLog[valueSize] = '\0';
	printf("BUILD LOG: %s", pBuildLog);
}
cl_int 
COpenCLKernel::GetPlatformID(cl_platform_id* _pPlatformID)
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
COpenCLKernel::GetCLFileContents(const char* _pcFilename, int& _iLength)
{
	std::ifstream fileIn;
	fileIn.open(_pcFilename);

	char* pcBuffer = 0;
	std::string sFileString;

	if (fileIn.is_open())
	{
		fileIn.seekg(0, fileIn.end);
		_iLength = static_cast<int>(fileIn.tellg());
		fileIn.seekg(0, fileIn.beg);

		pcBuffer = new char[_iLength + 1];
		fileIn.read(pcBuffer, _iLength);
		pcBuffer[_iLength] = '\0';

		fileIn.close();
	}
	else
	{
		printf("Could not open OpenCL file %s", _pcFilename);
	}
	return pcBuffer;
}
const char* 
COpenCLKernel::GetErrorString(cl_int _iError)
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