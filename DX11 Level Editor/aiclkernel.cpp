#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

#include "aihivemind.h"
#include "aicontroller.h"
#include "renderentity.h"
#include "openclcontext.h"

#include "aiclkernel.h"

CAICLKernel::CAICLKernel()
{
	m_clInPos = 0;
	m_clInDir = 0;
	m_clInWaypoint = 0;
	m_clAIData = 0;
	m_clOutPos = 0;
	m_clOutDir = 0;
	m_pGlobalGroup = new size_t[1];
	m_pWorkGroup = new size_t[1];
}
CAICLKernel::~CAICLKernel()
{
	clReleaseMemObject(m_clInPos);
	clReleaseMemObject(m_clInDir);
	clReleaseMemObject(m_clInWaypoint);
	clReleaseMemObject(m_clAIData);
	clReleaseMemObject(m_clOutPos);
	clReleaseMemObject(m_clOutDir);
}
void
CAICLKernel::SendDataToGPU(COpenCLContext* _pOpenCL, CAIHiveMind* _pHiveMind, float _fDeltaTime)
{
	cl_int iError = 0;
	m_iArraySize = _pHiveMind->GetAICount();
	D3DXVECTOR4* pPositions = new D3DXVECTOR4[m_iArraySize];
	D3DXVECTOR4* pDirections = new D3DXVECTOR4[m_iArraySize];
	D3DXVECTOR4* pWaypoints = new D3DXVECTOR4[m_iArraySize];
	D3DXVECTOR4* pAIData = new D3DXVECTOR4[m_iArraySize];
	for (int i = 0; i < m_iArraySize; ++i)
	{
		CRenderEntity* pCurrentAI = _pHiveMind->GetAI(i)->GetEntity();
		TAIDescription* pAIType = &_pHiveMind->GetAIDesc(_pHiveMind->GetAI(i)->GetAIType());
		pPositions[i] = D3DXVECTOR4(pCurrentAI->GetPosition().x, pCurrentAI->GetPosition().y, pCurrentAI->GetPosition().z, _fDeltaTime);
		pDirections[i] = D3DXVECTOR4(pCurrentAI->GetForward().x, pCurrentAI->GetForward().y, pCurrentAI->GetForward().z, _fDeltaTime);
		pWaypoints[i] = D3DXVECTOR4(_pHiveMind->GetAI(i)->GetCurrentWaypoint().x, _pHiveMind->GetAI(i)->GetCurrentWaypoint().y, _pHiveMind->GetAI(i)->GetCurrentWaypoint().z, _fDeltaTime);
		pAIData[i] = D3DXVECTOR4(pAIType->fMovementSpeed, pAIType->fRotationSpeed, pCurrentAI->GetRadius(), 1.0f);
	}
	//Input buffers
	m_clInPos = clCreateBuffer(_pOpenCL->GetCLContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(D3DXVECTOR4)* m_iArraySize, pPositions, &iError);
	m_clInDir = clCreateBuffer(_pOpenCL->GetCLContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(D3DXVECTOR4)* m_iArraySize, pDirections, &iError);
	m_clInWaypoint = clCreateBuffer(_pOpenCL->GetCLContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(D3DXVECTOR4)* m_iArraySize, pWaypoints, &iError);
	m_clAIData = clCreateBuffer(_pOpenCL->GetCLContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(D3DXVECTOR4)* m_iArraySize, pAIData, &iError);
	//Output buffers
	m_clOutPos = clCreateBuffer(_pOpenCL->GetCLContext(), CL_MEM_READ_ONLY, sizeof(D3DXVECTOR4)* m_iArraySize, NULL, &iError);
	m_clOutDir = clCreateBuffer(_pOpenCL->GetCLContext(), CL_MEM_READ_ONLY, sizeof(D3DXVECTOR4)* m_iArraySize, NULL, &iError);

	//Bind buffers to kernel
	iError = clSetKernelArg(m_clKernel, 0, sizeof(cl_mem), (void*)&m_clInPos);
	iError = clSetKernelArg(m_clKernel, 1, sizeof(cl_mem), (void*)&m_clInDir);
	iError = clSetKernelArg(m_clKernel, 2, sizeof(cl_mem), (void*)&m_clInWaypoint);
	iError = clSetKernelArg(m_clKernel, 3, sizeof(cl_mem), (void*)&m_clAIData);
	iError = clSetKernelArg(m_clKernel, 4, sizeof(cl_mem), (void*)&m_clOutPos);
	iError = clSetKernelArg(m_clKernel, 5, sizeof(cl_mem), (void*)&m_clOutDir);

	m_pGlobalGroup[0] = m_iArraySize;
	m_pWorkGroup[0] = 1;
	_pOpenCL->SetCLWorkGroupSize(m_pWorkGroup, m_pGlobalGroup, 1);

	delete[] pPositions;
	delete[] pDirections;
	delete[] pWaypoints;
	delete[] pAIData;
}
void
CAICLKernel::RetrieveOpenCLResults(COpenCLContext* _pOpenCL, CAIHiveMind* _pHiveMind)
{
	//Retrieve data from calculations
	D3DXVECTOR4* pOutPositions = new D3DXVECTOR4[m_iArraySize];
	D3DXVECTOR4* pOutDirections = new D3DXVECTOR4[m_iArraySize];
	cl_int iError = 0;
	iError = clEnqueueReadBuffer(_pOpenCL->GetCLCommandQueue(), m_clOutPos, CL_TRUE, 0, sizeof(D3DXVECTOR4)* m_iArraySize, pOutPositions, 0, NULL, &_pOpenCL->GetCLEvent());
	clReleaseEvent(_pOpenCL->GetCLEvent());
	iError = clEnqueueReadBuffer(_pOpenCL->GetCLCommandQueue(), m_clOutDir, CL_TRUE, 0, sizeof(D3DXVECTOR4)* m_iArraySize, pOutDirections, 0, NULL, &_pOpenCL->GetCLEvent());
	clReleaseEvent(_pOpenCL->GetCLEvent());
	
	//Send final output back to AI
	for (int iAI = 0; iAI < _pHiveMind->GetAICount(); ++iAI)
	{
		CRenderEntity* pCurrentEntity = _pHiveMind->GetAI(iAI)->GetEntity();
		pCurrentEntity->SetPosition(D3DXVECTOR3(pOutPositions[iAI].x, pOutPositions[iAI].y, pOutPositions[iAI].z));
		pCurrentEntity->SetForward(D3DXVECTOR3(pOutDirections[iAI].x, pOutDirections[iAI].y, pOutDirections[iAI].z));
	}

	delete[] pOutPositions;
	delete[] pOutDirections;
}