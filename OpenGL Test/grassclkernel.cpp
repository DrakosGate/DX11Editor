#include "grass.h"

#include "grassclkernel.h"

CGrassCLKernel::CGrassCLKernel()
{

}
CGrassCLKernel::~CGrassCLKernel()
{
	
}
void
CGrassCLKernel::SendDataToGPU(CGrass* _pGrass, float _fDeltaTime)
{
	//cl_int iError = 0;
	//m_iArraySize = _pHiveMind->GetAICount();
	//D3DXVECTOR4* pPositions = new D3DXVECTOR4[m_iArraySize];
	//D3DXVECTOR4* pDirections = new D3DXVECTOR4[m_iArraySize];
	//D3DXVECTOR4* pWaypoints = new D3DXVECTOR4[m_iArraySize];
	//D3DXVECTOR4* pAIData = new D3DXVECTOR4[m_iArraySize];
	//for (int i = 0; i < m_iArraySize; ++i)
	//{
	//	CRenderEntity* pCurrentAI = _pHiveMind->GetAI(i)->GetEntity();
	//	TAIDescription* pAIType = &_pHiveMind->GetAIDesc(_pHiveMind->GetAI(i)->GetAIType());
	//	pPositions[i] = D3DXVECTOR4(pCurrentAI->GetPosition().x, pCurrentAI->GetPosition().y, pCurrentAI->GetPosition().z, _fDeltaTime);
	//	pDirections[i] = D3DXVECTOR4(pCurrentAI->GetForward().x, pCurrentAI->GetForward().y, pCurrentAI->GetForward().z, _fDeltaTime);
	//	pWaypoints[i] = D3DXVECTOR4(_pHiveMind->GetAI(i)->GetCurrentWaypoint().x, _pHiveMind->GetAI(i)->GetCurrentWaypoint().y, _pHiveMind->GetAI(i)->GetCurrentWaypoint().z, _fDeltaTime);
	//	pAIData[i] = D3DXVECTOR4(pAIType->fMovementSpeed, pAIType->fRotationSpeed, pCurrentAI->GetRadius(), 1.0f);
	//}
	////Input buffers
	//m_clInPos = clCreateBuffer(m_clContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(D3DXVECTOR4)* m_iArraySize, pPositions, &iError);
	//m_clInDir = clCreateBuffer(m_clContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(D3DXVECTOR4)* m_iArraySize, pDirections, &iError);
	//m_clInWaypoint = clCreateBuffer(m_clContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(D3DXVECTOR4)* m_iArraySize, pWaypoints, &iError);
	//m_clAIData = clCreateBuffer(m_clContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(D3DXVECTOR4)* m_iArraySize, pAIData, &iError);
	////Output buffers
	//m_clOutPos = clCreateBuffer(m_clContext, CL_MEM_READ_ONLY, sizeof(D3DXVECTOR4)* m_iArraySize, NULL, &iError);
	//m_clOutDir = clCreateBuffer(m_clContext, CL_MEM_READ_ONLY, sizeof(D3DXVECTOR4)* m_iArraySize, NULL, &iError);
	//
	////Bind buffers to kernel
	//iError = clSetKernelArg(m_clKernel, 0, sizeof(cl_mem), (void*)&m_clInPos);
	//iError = clSetKernelArg(m_clKernel, 1, sizeof(cl_mem), (void*)&m_clInDir);
	//iError = clSetKernelArg(m_clKernel, 2, sizeof(cl_mem), (void*)&m_clInWaypoint);
	//iError = clSetKernelArg(m_clKernel, 3, sizeof(cl_mem), (void*)&m_clAIData);
	//iError = clSetKernelArg(m_clKernel, 4, sizeof(cl_mem), (void*)&m_clOutPos);
	//iError = clSetKernelArg(m_clKernel, 5, sizeof(cl_mem), (void*)&m_clOutDir);
	//
	//workGroupSize[0] = m_iArraySize;
	//
	//delete[] pPositions;
	//delete[] pDirections;
	//delete[] pWaypoints;
	//delete[] pAIData;
}
void
CGrassCLKernel::RetrieveOpenCLResults(CGrass* _pGrass)
{
	//Retrieve data from calculations
	//D3DXVECTOR4* pOutPositions = new D3DXVECTOR4[m_iArraySize];
	//D3DXVECTOR4* pOutDirections = new D3DXVECTOR4[m_iArraySize];
	//cl_int iError = 0;
	//iError = clEnqueueReadBuffer(m_clCommandQueue, m_clOutPos, CL_TRUE, 0, sizeof(D3DXVECTOR4)* m_iArraySize, pOutPositions, 0, NULL, &m_clEvent);
	//clReleaseEvent(m_clEvent);
	//iError = clEnqueueReadBuffer(m_clCommandQueue, m_clOutDir, CL_TRUE, 0, sizeof(D3DXVECTOR4)* m_iArraySize, pOutDirections, 0, NULL, &m_clEvent);
	//clReleaseEvent(m_clEvent);
	//
	////Send final output back to AI
	//for (int iAI = 0; iAI < _pHiveMind->GetAICount(); ++iAI)
	//{
	//	CRenderEntity* pCurrentEntity = _pHiveMind->GetAI(iAI)->GetEntity();
	//	pCurrentEntity->SetPosition(D3DXVECTOR3(pOutPositions[iAI].x, pOutPositions[iAI].y, pOutPositions[iAI].z));
	//	pCurrentEntity->SetForward(D3DXVECTOR3(pOutDirections[iAI].x, pOutDirections[iAI].y, pOutDirections[iAI].z));
	//}
	//
	//delete[] pOutPositions;
	//delete[] pOutDirections;
}