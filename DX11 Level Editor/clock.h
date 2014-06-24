//
//  File Name   :   clock.h
//  Description :   Clock Interface
//  Author      :   Chris Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __CLOCK_H__
#define __CLOCK_H__

// Library Includes
#include <Windows.h>

// Local Includes

// Types

// Constants

// Prototypes


class CClock
{
    // Member Functions
public:
	CClock();
	~CClock();
	bool Initialise();
	void Process();
	float GetDeltaTick();
	void LimitFramesPerSecond(float _fMaxFramesPerSecond);

	void StartTimer();
	void EndTimer();
	float GetTimeElapsed();
	int GetFPS() const;

protected:

private:
	CClock(const CClock& _kr);
	CClock& operator= (const CClock& _rHS);

    // Member Variables
public:

protected:
	bool m_bUsingPerformanceCounter;
	bool m_bIsCounting;
	double m_fStartTime;
	double m_fEndTime;
	double m_fDeltaTime;
	double m_fLastTime;
	double m_fCurrentTime;

	double m_fTimerFrequency;

	//Count FPS
	double m_fTimeElapsed;
	int m_iFramesPerSecond;
	int m_iFrameCount;	
	
	double m_fTotalAverageTimeElapsed;
	double m_fFramesCounted;
	
	double m_fFramesPerSecondLimit;
	bool m_bIsLimittingFPS;

private:
	LARGE_INTEGER m_iNumCounts;
	LARGE_INTEGER m_iTimerFrequency;

};

#endif // __CLOCK_H__
