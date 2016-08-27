//
//  File Name   :   ThreadPool.h
//  Description :   Definition of the ThreadPool class
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
class Grass;
class AIHiveMind;
struct TGrassThread
{
	TGrassThread(){}
	TGrassThread(Grass* _pGrass, int _iGrassSection, float _fDeltaTime)
	{
		pGrass = _pGrass;
		iGrassSection = _iGrassSection;
		fDeltaTime = _fDeltaTime;
	}
	Grass* pGrass;
	int iGrassSection;
	float fDeltaTime;
};
// Constants

// Prototypes

//Thread Function Declarations
void GrassProcessingThread(PARAMETER_TYPE _pData, int _iThreadID);
void PrintSomething(PARAMETER_TYPE _pData, int _iThreadID);
void AIProcessingThread(PARAMETER_TYPE _pData, int _iThreadID);

class CThread;

class ThreadPool
{
//Member functions
public:
	ThreadPool();
	~ThreadPool();

	void Initialise(int _iThreadCount);
	void AddJobToPool(JOB_TYPE _JobType, PARAMETER_TYPE _rParameters);
	void JoinWithMainThread();

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