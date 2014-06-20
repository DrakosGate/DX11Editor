#include <stdio.h>

#include "defines.h"

#include "openclkernel.h"

COpenCLKernel::COpenCLKernel()
: m_pWorkGroup(0)
, m_pGlobalGroup(0)
{

}
COpenCLKernel::~COpenCLKernel()
{
	SAFEDELETEARRAY(m_pWorkGroup);
	SAFEDELETEARRAY(m_pGlobalGroup);
}
cl_kernel&
COpenCLKernel::GetCLKernel()
{
	return m_clKernel;
}
cl_program&
COpenCLKernel::GetCLProgram()
{
	return m_clProgram;
}