//
//  File Name   :   threadpool.cpp
//  Description :   Code for ThreadPool
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes
#include "grass.h"
#include "aihivemind.h"

// This Include
#include "threadpool.h"

// Static Variables

// Static Function Prototypes

// Implementation
void PrintSomething(PARAMETER_TYPE _pData, int _iThreadID)
{
	TSetupJob* pJob = reinterpret_cast<TSetupJob*>(_pData);
	printf("Thread %i: %i\n", _iThreadID, pJob->iData);
}
void GrassProcessingThread(PARAMETER_TYPE _pData, int _iThreadID)
{
	TGrassThread* pJob = reinterpret_cast<TGrassThread*>(_pData);
	//Process a section of grass
	pJob->pGrass->ProcessGrassSection(	pJob->iGrassSection,
										pJob->fDeltaTime);
}
void AIProcessingThread(PARAMETER_TYPE _pData, int _iThreadID)
{
	TAIThreadData* pParam = reinterpret_cast<TAIThreadData*>(_pData);
	pParam->pThis->ProcessIndividualAIMovement(pParam->iAIIndex, pParam->fDeltaTime);
	pParam->pThis->ProcessIndividualAIController(pParam->iAIIndex, pParam->fDeltaTime);
}
/**
*
* ThreadPool class Constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
ThreadPool::ThreadPool()
: m_bKillThreads(false)
{

}
/**
*
* ThreadPool class Destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
ThreadPool::~ThreadPool()
{
	{//Lock mutex
		std::unique_lock<std::mutex> lockMutex(m_jobMutex);
		m_bKillThreads = true;
	}//Unlock mutex
	m_condition.notify_all();

	//Join all threads back to the main thread
	for (unsigned int iThread = 0; iThread < m_vecThreadPool.size(); ++iThread)
	{
		m_vecThreadPool[iThread].join();
	}
}
/**
*
* ThreadPool class Initialises the thread pool with a number of threads
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _iThreadCount Initial thread count
*
*/
void
ThreadPool::Initialise(int _iThreadCount)
{
	//Add initial threads to the thread pool
	for (int iThread = 0; iThread < _iThreadCount; ++iThread)
	{
		m_vecThreadPool.push_back(std::thread(&ThreadPool::ThreadLoop, this, iThread));
	}
}
/**
*
* ThreadPool class Main thread loop function
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _iThreadIndex Unique ID for this thread
*
*/
void
ThreadPool::ThreadLoop(int _iThreadIndex)
{
	printf("Thread %i Loop started\n", _iThreadIndex);
	TQueuedJob currentJob;
	while (true)
	{
		{//Lock mutex
			std::unique_lock<std::mutex> threadLock(m_jobMutex);

			//Check if the job list is empty
			while (m_bKillThreads == false && m_jobList.empty())
			{
				m_condition.wait(threadLock);
			}
			//Check if the thread pool is still alive
			if (m_bKillThreads)
			{
				break;
			}

			//Get tasks from the queue
			currentJob = m_jobList.front();
			m_jobList.pop();
		}//Unlock mutex
		currentJob.job(currentJob.parameters, _iThreadIndex);
	}
}
/**
*
* ThreadPool class Adds a job to the list of thread jobs
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _JobType Job Function pointer
* @param _rParameters Parameters assigned to this job
*
*/
void
ThreadPool::AddJobToPool(JOB_TYPE _JobType, PARAMETER_TYPE _rParameters)
{
	{//Lock mutex
		std::unique_lock<std::mutex> threadLock(m_jobMutex);

		m_jobList.push(TQueuedJob(_JobType, _rParameters));
	}//Unlock mutex
	m_condition.notify_one();
}
void
ThreadPool::JoinWithMainThread()
{
	for (unsigned int iThread = 0; iThread < m_vecThreadPool.size(); ++iThread)
	{
		m_vecThreadPool[iThread].join();
	}
}