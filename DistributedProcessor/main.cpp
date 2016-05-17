
#include <Windows.h>
#include "distributedprocessor.h"


int main()
{
	CDistributedProcessor* pProcessor = new CDistributedProcessor();
	pProcessor->Initialise();

	while( pProcessor->IsActive() )
	{
		pProcessor->Run();
	}

	return 0;
}