//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   clock.cpp
//  Description :   Clock Interface
//  Author      :   Chris Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <ctime>

// Local Includes

// This Include
#include "clock.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CCloth class Constructor
*
* @author Christopher Howlett
*
*/
CClock::CClock()
:	m_fStartTime(0.0f)
,	m_fEndTime(0.0f)
,	m_fDeltaTime(0.0f)
,	m_fLastTime(0.0f)
,	m_fCurrentTime(0.0f)
,	m_fTimeElapsed(0.0f)
,	m_bUsingPerformanceCounter(false)
,	m_fFramesCounted(0.0f)
,	m_fTotalAverageTimeElapsed(0.0f)
,	m_fFramesPerSecondLimit(0.0f)
,	m_bIsLimittingFPS(false)
,	m_iFramesPerSecond(0)
,	m_iFrameCount(0)
{

}

/**
*
* CCloth class Destructor
*
* @author Christopher Howlett
*
*/
CClock::~CClock()
{

}

/**
*
* CCloth class Initialisation
*
* @author Christopher Howlett
*
*/
bool
CClock::Initialise()
{
	//Check if QueryPerformanceCounter can be used
	if(QueryPerformanceFrequency(&m_iTimerFrequency))
	{
		m_bUsingPerformanceCounter = true;
	}
	else
	{
		m_bUsingPerformanceCounter = false;
	}
	Process();
	return (m_bUsingPerformanceCounter);
}
/**
*
* CCloth class Process
*
* @author Christopher Howlett
*
*/
void
CClock::Process()
{
	m_fLastTime = m_fCurrentTime;
	
	//If QueryPerformanceCounter is supported...
	if(m_bUsingPerformanceCounter)
	{
		QueryPerformanceCounter(&m_iNumCounts);
		m_fCurrentTime = static_cast<float>(m_iNumCounts.QuadPart / static_cast<float>(m_iTimerFrequency.QuadPart));
	}

	if (m_fLastTime == 0.0f)
	{
		m_fLastTime = m_fCurrentTime;
	}

	m_fDeltaTime = m_fCurrentTime - m_fLastTime;
	//Count time elapsed - calculate FPS
	++m_iFrameCount;
	m_fTimeElapsed += m_fDeltaTime;
	if(m_fTimeElapsed > 1.0f)
	{
		m_iFramesPerSecond = m_iFrameCount;
		m_fTimeElapsed = 0;
		m_iFrameCount = 0;
	}
	//Limit frames per second
	if(m_bIsLimittingFPS)
	{
		if(m_fDeltaTime > m_fFramesPerSecondLimit)
		{
			m_fDeltaTime = m_fFramesPerSecondLimit;
		}
	}
	else
	{
		//Do nothing
	}
}
/**
*
* CCloth class GetDeltaTick
*
* @author Christopher Howlett
* @return Returns game time elapsed since last frame
*
*/
float
CClock::GetDeltaTick()
{
	return(m_fDeltaTime);
}
/**
*
* Gets Clock limits max frames per second
*
* @author Christopher Howlett
*
*/
void 
CClock::LimitFramesPerSecond(float _fMaxFramesPerSecond)
{
	m_bIsLimittingFPS = true;
	m_fFramesPerSecondLimit = 1.0f / _fMaxFramesPerSecond;
}
/**
*
* Clock Start Timer - starts counting for performance measurements
*
* @author Christopher Howlett
*
*/
void
CClock::StartTimer()
{
	QueryPerformanceCounter(&m_iNumCounts);
	m_fStartTime = static_cast<float>(m_iNumCounts.QuadPart / static_cast<float>(m_iTimerFrequency.QuadPart));;
}
/**
*
* Clock End Timer - stops counting for performance measurements
*
* @author Christopher Howlett
*
*/
void 
CClock::EndTimer()
{
	QueryPerformanceCounter(&m_iNumCounts);
	m_fEndTime = static_cast<float>(m_iNumCounts.QuadPart / static_cast<float>(m_iTimerFrequency.QuadPart));;
}
/**
*
* Gets Clock counter time elapsed
*
* @author Christopher Howlett
*
*/
float
CClock::GetTimeElapsed()
{
	++m_fFramesCounted;
	m_fTotalAverageTimeElapsed += m_fEndTime - m_fStartTime;
	return (m_fTotalAverageTimeElapsed / m_fFramesCounted);
}
/**
*
* Gets Clock counter time elapsed
*
* @author Christopher Howlett
*
*/
int
CClock::GetFPS() const
{
	return m_iFramesPerSecond;
}