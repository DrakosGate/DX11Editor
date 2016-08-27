#include <stdio.h>

#include "defines.h"

#include "openclkernel.h"

OpenCLKernel::OpenCLKernel()
: m_pWorkGroup(0)
, m_pGlobalGroup(0)
{

}
OpenCLKernel::~OpenCLKernel()
{
	SAFEDELETEARRAY(m_pWorkGroup);
	SAFEDELETEARRAY(m_pGlobalGroup);
}
cl_kernel&
OpenCLKernel::GetCLKernel()
{
	return m_clKernel;
}
cl_program&
OpenCLKernel::GetCLProgram()
{
	return m_clProgram;
}