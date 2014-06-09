//
//  File Name   :   CThreadPool.h
//  Description :   Definition of the CThreadPool class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#define PARAMETER_TYPE void*
#define JOB_TYPE std::function<void(PARAMETER_TYPE, int)>

// Library Includes
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

// Local Includes

// Types
struct TQueuedJob
{
	TQueuedJob(){}
	TQueuedJob(JOB_TYPE& _job, PARAMETER_TYPE& _parameters)
	{
		job = _job;
		parameters = _parameters;
	}
	JOB_TYPE job;
	PARAMETER_TYPE parameters;
};
struct TSetupJob
{
	TSetupJob(int Data)
	{
		iData = Data;
	}
	int iData;
};
// Constants

// Prototypes
class CThread;

class CThreadPool
{
//Member functions
public:
	CThreadPool();
	~CThreadPool();

	void Initialise(int _iThreadCount);
	void AddJob(JOB_TYPE _JobType, PARAMETER_TYPE _rParameters);

//Private functions
private:
	void ThreadLoop(int _iThreadIndex);

//Member variables
private:
	std::vector<std::thread> m_vecThreadPool;

	std::queue<TQueuedJob> m_jobList;

	std::mutex m_jobMutex;
	std::condition_variable m_condition;
	bool m_bKillThreads;
};


#endif//__THREADPOOL_H__