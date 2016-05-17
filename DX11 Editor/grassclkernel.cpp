#include "grass.h"
#include "openclcontext.h"

#include "grassclkernel.h"

CGrassCLKernel::CGrassCLKernel()
: m_pPositions(0)
, m_pDirections(0)
, m_pObjectData(0)
, m_pOutDirections(0)
, m_iNumObstacles(0)
{
	m_clInPos = 0;
	m_clInDir = 0;
	m_clInObjects = 0;
	m_clOutDir = 0;
}
CGrassCLKernel::~CGrassCLKernel()
{
	SAFEDELETEARRAY(m_pPositions);
	SAFEDELETEARRAY(m_pDirections);
	SAFEDELETEARRAY(m_pObjectData);
	SAFEDELETEARRAY(m_pOutDirections); 

	clReleaseMemObject(m_clInPos);
	clReleaseMemObject(m_clInDir);
	clReleaseMemObject(m_clInObjects);
	clReleaseMemObject(m_clOutDir);
}
void 
CGrassCLKernel::CreateBuffers(COpenCLContext* _pOpenCL, CGrass* _pGrass, std::vector<RenderEntity*>* _pCollisionObjects)
{
	cl_int iError = 0;
	m_iArraySize = _pGrass->GetVertexCount();

	m_pPositions = new Math::Vector4[m_iArraySize];
	m_pDirections = new Math::Vector4[m_iArraySize];
	m_pOutDirections = new Math::Vector4[m_iArraySize];

	//Create OpenCL buffers that WONT change
	m_clInPos = clCreateBuffer(_pOpenCL->GetCLContext(), CL_MEM_READ_ONLY, sizeof(Math::Vector4)* m_iArraySize, NULL, &iError);
	m_clInDir = clCreateBuffer(_pOpenCL->GetCLContext(), CL_MEM_READ_ONLY, sizeof(Math::Vector4)* m_iArraySize, NULL, &iError);

	m_pWorkGroup = new size_t[3];
	m_pGlobalGroup = new size_t[3];
}
void
CGrassCLKernel::SendDataToGPU(COpenCLContext* _pOpenCL, CGrass* _pGrass, std::vector<RenderEntity*>* _pCollisionObjects, float _fDeltaTime)
{
	/*	__global float4* _pGrassPositions, 
		__global float4* _pGrassDirections,
		__global float4* _pObjectPositions,

		__global float4* _pOutDirections*/
	cl_int iError = 0;

	m_iNumObstacles = _pCollisionObjects->size();
	m_pObjectData = new Math::Vector4[m_iNumObstacles];
	//Vertex data contains vector + delta time
	for (int iVertex = 0; iVertex < m_iArraySize; ++iVertex)
	{
		m_pPositions[iVertex] = Math::Vector4(_pGrass->GetVertexData(iVertex)->pos.x, _pGrass->GetVertexData(iVertex)->pos.y, _pGrass->GetVertexData(iVertex)->pos.z, _fDeltaTime);
		m_pDirections[iVertex] = Math::Vector4(_pGrass->GetVertexData(iVertex)->normal.x, _pGrass->GetVertexData(iVertex)->normal.y, _pGrass->GetVertexData(iVertex)->normal.z, _fDeltaTime);
	}
	//Object data contains position + radius
	for (int iObject = 0; iObject < m_iNumObstacles; ++iObject)
	{
		m_pObjectData[iObject] = Math::Vector4((*_pCollisionObjects)[iObject]->GetPosition().x, (*_pCollisionObjects)[iObject]->GetPosition().y, (*_pCollisionObjects)[iObject]->GetPosition().z, (*_pCollisionObjects)[iObject]->GetRadius());
	}

	//Input buffers
	clEnqueueWriteBuffer(_pOpenCL->GetCLCommandQueue(), m_clInPos, CL_TRUE, 0, sizeof(Math::Vector4)* m_iArraySize, m_pPositions, 0, NULL, NULL);
	clEnqueueWriteBuffer(_pOpenCL->GetCLCommandQueue(), m_clInDir, CL_TRUE, 0, sizeof(Math::Vector4)* m_iArraySize, m_pDirections, 0, NULL, NULL);
	//clEnqueueWriteBuffer(_pOpenCL->GetCLCommandQueue(), m_clInObjects, CL_TRUE, 0, sizeof(Math::Vector4)* m_iNumObstacles, m_pObjectData, 0, NULL, NULL);

	//m_clInPos = clCreateBuffer(_pOpenCL->GetCLContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Math::Vector4)* m_iArraySize, m_pPositions, &iError);
	//m_clInDir = clCreateBuffer(_pOpenCL->GetCLContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Math::Vector4)* m_iArraySize, m_pDirections, &iError);
	if (m_clInObjects)
	{
		clReleaseMemObject(m_clInObjects);
	}
	m_clInObjects = clCreateBuffer(_pOpenCL->GetCLContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Math::Vector4)* m_iNumObstacles, m_pObjectData, &iError);
	
	//Output buffers
	m_clOutDir = clCreateBuffer(_pOpenCL->GetCLContext(), CL_MEM_WRITE_ONLY, sizeof(Math::Vector4)* m_iArraySize, NULL, &iError);
	
	//Bind buffers to kernel
	iError = clSetKernelArg(m_clKernel, 0, sizeof(cl_mem), (void*)&m_clInPos);
	iError = clSetKernelArg(m_clKernel, 1, sizeof(cl_mem), (void*)&m_clInDir);
	iError = clSetKernelArg(m_clKernel, 2, sizeof(cl_mem), (void*)&m_clInObjects);
	iError = clSetKernelArg(m_clKernel, 3, sizeof(cl_mem), (void*)&m_clOutDir);
	
	m_pGlobalGroup[0] = _pGrass->GetDimensionSize() * m_iNumObstacles;
	m_pGlobalGroup[1] = _pGrass->GetDimensionSize();// m_iArraySize * m_iNumObstacles;

	m_pWorkGroup[0] = _pGrass->GetDimensionSize();
	m_pWorkGroup[1] = 1;

	_pOpenCL->SetCLWorkGroupSize(m_pWorkGroup, m_pGlobalGroup, 2);
	SAFEDELETEARRAY(m_pObjectData);
}
void
CGrassCLKernel::RetrieveOpenCLResults(COpenCLContext* _pOpenCL, CGrass* _pGrass)
{
	//Retrieve data from calculations
	cl_int iError = 0;
	iError = clEnqueueReadBuffer(_pOpenCL->GetCLCommandQueue(), m_clOutDir, CL_TRUE, 0, sizeof(Math::Vector4)* m_iArraySize, m_pOutDirections, 0, NULL, NULL);
	
	//Send final output back to Grass model
	for (int iBlade = 0; iBlade < _pGrass->GetVertexCount(); ++iBlade)
	{
		_pGrass->GetVertexData(iBlade)->normal = Math::Vector3(m_pOutDirections[iBlade].x, m_pOutDirections[iBlade].y, m_pOutDirections[iBlade].z);
	}
}