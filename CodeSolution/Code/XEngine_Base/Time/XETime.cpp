/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <Windows.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "XETime.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETime::XETime(bool sampleFPS)
	: m_IsSamplingFPS(sampleFPS)
{
}

XETime::~XETime()
{
}

void XETime::UpdateAllTimers()
{
	UpdateConstantUpdateTimer();
	UpdateUpdateTimer();
	UpdatePostUpdateTimer();
	UpdateRenderTimer();
	UpdateFrameTimer();
}

void XETime::UpdateConstantUpdateTimer()
{
	m_ConstantUpdateTimer.Update();
}

void XETime::PostUpdateConstantUpdateTimer()
{
	m_ConstantUpdateTimer.PostUpdate();
}

void XETime::UpdateUpdateTimer()
{
	m_UpdateTimer.Update();
}

void XETime::PostUpdateUpdateTimer()
{
	m_UpdateTimer.PostUpdate();
}

void XETime::UpdatePostUpdateTimer()
{
	m_PostUpdateTimer.Update();
}

void XETime::PostUpdatePostUpdateTimer()
{
	m_PostUpdateTimer.PostUpdate();
}

void XETime::UpdateRenderTimer()
{
	m_RenderTimer.Update();
}

void XETime::PostUpdateRenderTimer()
{
	m_RenderTimer.PostUpdate();
}

void XETime::UpdateFrameTimer()
{
	m_FrameTimer.Update();

	if(m_IsSamplingFPS)
	{
		SampleFPS();
	}
}

void XETime::SampleFPS()
{
	m_FrameCount++;

	double diff = m_FrameTimer.GetTimerParams().m_TotalElapsedTime - m_LastFPSTime;

	if( diff >= 1.0 )
	{
		m_FPS =  m_FrameCount/(float)diff;
		m_FrameCount = 0;
		m_LastFPSTime = m_FrameTimer.GetTimerParams().m_TotalElapsedTime;

		m_MilliPerFrame = 1000.0f / m_FPS;
	}
}
