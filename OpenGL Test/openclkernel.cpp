#include <stdio.h>

#include "defines.h"

#include "openclkernel.h"

COpenCLKernel::COpenCLKernel()
: m_pWorkGroup(0)
{

}
COpenCLKernel::~COpenCLKernel()
{
	SAFEDELETEARRAY(m_pWorkGroup);
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