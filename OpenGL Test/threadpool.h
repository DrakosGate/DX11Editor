//
//  File Name   :   CThreadPool.h
//  Description :   Definition of the CThreadPool class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __CThreadPool_H__
#define __CThreadPool_H__

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

#define JOB_TYPE std::function<void(void*)>

struct TTaskDescription
{
	TTaskDescription(JOB_TYPE& _rJob, void* _pParameters)
	{
		job = _rJob;
		pParameter = _pParameters;
	}
	JOB_TYPE job;
	void* pParameter;
};

class CThreadPool
{
//Member functions
public:
	CThreadPool();
	~CThreadPool();

	bool Initialise(int _iThreadCount, int _iMaxJobCount);
	void AddJobToPool(JOB_TYPE _job, void* _pParameters);	
	
private:
	void Run(int _iThreadIndex);

//Member variables
private:
	std::vector<std::thread*> m_threads;

	std::queue<TTaskDescription*> m_jobList;

	std::mutex m_jobLock;
	std::condition_variable m_jobNotification;

	std::atomic<bool> m_bKillThreads;

	std::mutex m_threadLock[2];

	int m_iThreadCount;
	int m_iMaxJobCount;
};


#endif//__CThreadPool_H__