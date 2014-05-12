//
//  File Name   :   CThreadPool.cpp
//  Description :   Definition for CThreadPool class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#include "threadpool.h"


void PrintSomething(void* _pSomething)
{
	int* pSomething = reinterpret_cast<int*>(_pSomething);
	printf("Processing %i\n", *pSomething);
}

CThreadPool::CThreadPool()
{

}
CThreadPool::~CThreadPool()
{
	//Notify all active threads that the program is shutting down
	m_bKillThreads = true;
	m_jobNotification.notify_all(); 

	//Set all threads to join - main thread will wait for all threads to finish before closing
	for (int iThread = 0; iThread < m_iThreadCount; ++iThread)
	{
		m_threads[iThread]->join();
		delete m_threads[iThread];
		m_threads[iThread] = 0;
	}
}
bool
CThreadPool::Initialise(int _iThreadCount, int _iMaxJobCount)
{
	printf("\n==== INITIALISING THREAD POOL WITH %i THREADS ====\n\n", _iThreadCount);
	m_iThreadCount = _iThreadCount;
	m_iMaxJobCount = _iMaxJobCount;

	m_bKillThreads = false;
	//Add threads to the vector of threads
	for (int iThread = 0; iThread < _iThreadCount; ++iThread)
	{
		//Assign the threads with the Run function - loop forever until killed
		m_threads.push_back(new std::thread(&CThreadPool::Run, this, iThread));
	}

	for (int iJob = 0; iJob < 100; ++iJob)
	{
		AddJobToPool(PrintSomething, &iJob);
	}

	return true;
}
void
CThreadPool::AddJobToPool(JOB_TYPE _job, void* _pParameters)
{
	std::unique_lock<std::mutex> lockList(m_jobLock);
	m_jobList.push(new TTaskDescription(_job, _pParameters));
	m_jobNotification.notify_one();
}
//Each thread will process this to find a an active job in the job list
void
CThreadPool::Run(int _iThreadIndex)
{
	TTaskDescription* pJob = 0;
	while (true)
	{
		std::unique_lock<std::mutex> lockList(m_jobLock);
		while (true)
		{
			if (m_bKillThreads)
			{
				return;
			}
			if (m_jobList.empty() == false)
			{
				pJob = m_jobList.front();
				m_jobList.pop();
				break;
			}
			m_jobNotification.wait(lockList);
		}
		if (pJob)
		{
			pJob->job(pJob->pParameter);
			delete pJob;
			pJob = 0;
		}
	}
	//Run forever until threads are killed
	//while (m_bKillThreads == false)
	//{
	//	TTaskDescription* pActiveJob = 0;
	//	{	//Lock this portion 
	//		std::unique_lock<std::mutex> lockList(m_jobLock);
	//
	//		//Loop until a new job has been found
	//		bool bHasJob = false;
	//		while (bHasJob == false)
	//		{
	//			//Check if the program has exited
	//			if (m_bKillThreads)
	//			{
	//				break;
	//			}
	//			//Check if there are jobs in the job list at the moment
	//			if (m_jobList.empty() == false)
	//			{
	//				//New job has been retrieved from the list!
	//				pActiveJob = m_jobList.front();
	//				m_jobList.pop();
	//				bHasJob = true;
	//				break;
	//			}
	//
	//			//Wait for a job to be added
	//			m_jobNotification.wait(lockList);
	//		}
	//	}
	//	//Carry out active job
	//	if (pActiveJob)
	//	{
	//		std::unique_lock<std::mutex> lockList(m_jobLock);
	//		pActiveJob->job(pActiveJob->pParameter);
	//		delete pActiveJob;
	//		pActiveJob = 0;
	//
	//		m_jobNotification.notify_one();
	//	}
	//}
	//printf("Killing thread %i\n", _iThreadIndex);
}