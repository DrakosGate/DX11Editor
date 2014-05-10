//
//  File Name   :   threadpool.cpp
//  Description :   Definition for ThreadPool class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#include "threadpool.h"

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
	return true;
}
void
CThreadPool::AddJobToPool(JOB_TYPE _job, void* _pParameters)
{
	{
		std::unique_lock<std::mutex> lockList(m_jobLock);
		m_jobList.push(_job);
		m_parameterList.push(_pParameters);
	}
	m_jobNotification.notify_one();
}
//Each thread will process this to find a an active job in the job list
void
CThreadPool::Run(int _iThreadIndex)
{
	//Run forever until threads are killed
	while (m_bKillThreads == false)
	{
		JOB_TYPE activeJob;
		void* pParameters = 0;
		{	//Lock this portion 
			std::unique_lock<std::mutex> lockList(m_jobLock);

			//Loop until a new job has been found
			bool bHasJob = false;
			while (bHasJob == false)
			{
				//Check if the program has exited
				if (m_bKillThreads)
				{
					break;
				}
				//Check if there are jobs in the job list at the moment
				if (m_jobList.empty() == false)
				{
					//New job has been retrieved from the list!
					activeJob = m_jobList.front();
					m_jobList.pop();
					pParameters = m_parameterList.front();
					m_parameterList.pop();
					bHasJob = true;
					break;
				}

				//Wait for a job to be added
				m_jobNotification.wait(lockList);
			}
		}
		//Carry out active job
		if (activeJob)
		{
			activeJob(pParameters);
		}
	}
	printf("Killing thread %i\n", _iThreadIndex);
}