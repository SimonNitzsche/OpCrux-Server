/// \file
///
/// This file is part of RakNet Copyright 2003 Kevin Jenkins.
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.jenkinssoftware.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.
/// GPL license users are subject to the GNU General Public
/// License as published by the Free
/// Software Foundation; either version 2 of the License, or (at your
/// option) any later version.

#include "GetTime.h"
#ifdef _CONSOLE_1
#include "Console1Includes.h" // Developers of a certain platform will know what to do here.
#elif defined(_WIN32)
#include <windows.h>
DWORD mProcMask;
DWORD mSysMask;
HANDLE mThread;
static LARGE_INTEGER yo;
#elif defined(_CONSOLE_2)
#include "Console2Includes.h"
uint64_t ticksPerSecond;
uint64_t initialTime;
#else
#include <sys/time.h>
#include <unistd.h>
static timeval tp;
RakNetTimeNS initialTime;
#endif

static bool initialized = false;
int queryCount = 0;

RakNetTime RakNet::GetTime(void)
{
	return (RakNetTime)(GetTimeNS() / 1000);
}
RakNetTimeNS RakNet::GetTimeNS(void)
{
#if defined(_CONSOLE_2)
	uint64_t curTime;
	if (initialized == false)
	{
		ticksPerSecond = _CONSOLE_2_GetTicksPerSecond();
		// Use the function to get elapsed ticks, this is a macro.
		_CONSOLE_2_GetElapsedTicks(curTime);
		uint64_t quotient, remainder;
		quotient = (curTime / ticksPerSecond);
		remainder = (curTime % ticksPerSecond);
		initialTime = (RakNetTimeNS)quotient * (RakNetTimeNS)1000000 + (remainder * (RakNetTimeNS)1000000 / ticksPerSecond);
		initialized = true;
	}
#elif (defined(__GNUC__)  || defined(__GCCXML__))
	if (initialized == false)
	{
		gettimeofday(&tp, 0);
		initialized = true;
		// I do this because otherwise RakNetTime in milliseconds won't work as it will underflow when dividing by 1000 to do the conversion
		initialTime = (tp.tv_sec) * (RakNetTimeNS)1000000 + (tp.tv_usec);
	}
#else
	// Win32
	if (initialized == false)
	{
		initialized = true;
		// Save the current process
		HANDLE mProc = GetCurrentProcess();

		// Get the current Affinity
#if _MSC_VER >= 1400 && defined (_M_X64)
		GetProcessAffinityMask(mProc, (PDWORD_PTR)&mProcMask, (PDWORD_PTR)&mSysMask);
#else
		GetProcessAffinityMask(mProc, &mProcMask, &mSysMask);
#endif

		mThread = GetCurrentThread();

		QueryPerformanceFrequency(&yo);
	}
	// 01/12/08 - According to the docs "The frequency cannot change while the system is running." so this shouldn't be necessary
	/*
	if (++queryCount==200)
	{
		// Set affinity to the first core
		SetThreadAffinityMask(mThread, 1);

		QueryPerformanceFrequency( &yo );

		// Reset affinity
		SetThreadAffinityMask(mThread, mProcMask);

		queryCount=0;
	}
	*/
#endif

#if defined(_CONSOLE_2)
	// Use the function to get elapsed ticks, this is a macro.
	_CONSOLE_2_GetElapsedTicks(curTime);
	uint64_t quotient, remainder;
	quotient = (curTime / ticksPerSecond);
	remainder = (curTime % ticksPerSecond);
	curTime = (RakNetTimeNS)quotient * (RakNetTimeNS)1000000 + (remainder * (RakNetTimeNS)1000000 / ticksPerSecond);
	// Subtract from initialTime so the millisecond conversion does not underflow
	return curTime - initialTime;
#elif defined(_WIN32)
	RakNetTimeNS curTime;
	static RakNetTimeNS lastQueryVal = (RakNetTimeNS)0;
	static unsigned long lastTickCountVal = GetTickCount();

	LARGE_INTEGER PerfVal;

	// Set affinity to the first core
	SetThreadAffinityMask(mThread, 1);

	// Query the timer
	QueryPerformanceCounter(&PerfVal);

	// Reset affinity
	SetThreadAffinityMask(mThread, mProcMask);

	__int64 quotient, remainder;
	quotient = ((PerfVal.QuadPart) / yo.QuadPart);
	remainder = ((PerfVal.QuadPart) % yo.QuadPart);
	curTime = (RakNetTimeNS)quotient * (RakNetTimeNS)1000000 + (remainder * (RakNetTimeNS)1000000 / yo.QuadPart);

	if (lastQueryVal == 0)
	{
		// First call
		lastQueryVal = curTime;
		return curTime;
	}

	// To workaround http://support.microsoft.com/kb/274323 where the timer can sometimes jump forward by hours or days
	unsigned long curTickCount = GetTickCount();
	unsigned elapsedTickCount = curTickCount - lastTickCountVal;
	RakNetTimeNS elapsedQueryVal = curTime - lastQueryVal;
	if (elapsedQueryVal / 1000 > elapsedTickCount + 100)
	{
		curTime = lastQueryVal + elapsedTickCount * 1000;
	}

	lastTickCountVal = curTickCount;
	lastQueryVal = curTime;
	return curTime;
#else
	// GCC
	RakNetTimeNS curTime;
	gettimeofday(&tp, 0);

	curTime = (tp.tv_sec) * (RakNetTimeNS)1000000 + (tp.tv_usec);
	// Subtract from initialTime so the millisecond conversion does not underflow
	return curTime - initialTime;
#endif
}