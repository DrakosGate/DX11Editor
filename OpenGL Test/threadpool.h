//
//  File Name   :   threadpool.h
//  Description :   Definition of the ThreadPool class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

#define JOB_TYPE std::function<void(void*)>

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

	std::queue<JOB_TYPE> m_jobList;
	std::queue<void*> m_parameterList;

	std::mutex m_jobLock;
	std::condition_variable m_jobNotification;

	std::atomic<bool> m_bKillThreads;

	int m_iThreadCount;
	int m_iMaxJobCount;
};


#endif//__THREADPOOL_H__