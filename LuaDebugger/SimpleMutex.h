#pragma once

#ifdef __WIN__
#include <windows.h>
#include <process.h>

#define CSType			CRITICAL_SECTION
#define InitCS(cs) 		InitializeCriticalSection(&cs)
#define DeleteCS(cs)	DeleteCriticalSection(&cs)
#define EnterCS(cs)		EnterCriticalSection(&cs)
#define TryEnterCS(cs)	TryEnterCriticalSection(&cs)
#define LeaveCS(cs)		LeaveCriticalSection(&cs)

#else
#include <pthread.h>

#define CSType			pthread_mutex_t
#define InitCS(cs)		pthread_mutex_init(&cs, NULL)
#define	DeleteCS(cs)	pthread_mutex_destroy(&cs)
#define EnterCS(cs)		pthread_mutex_trylock(&cs)
#define TryEnterCS(cs)	pthread_mutex_trylock(&cs)
#define LeaveCS(cs)		pthread_mutex_unlock(&cs)

#endif
#include <assert.h>


class CSimpleMutex
{
public:
	CSimpleMutex()
	{
		InitCS(m_impl);
		m_bGone = false;
	}

	~CSimpleMutex()
	{
		DeleteCS(m_impl);
	}

	void Grab()
	{
		EnterCS(m_impl);
		assert(!m_bGone);
		m_bGone = true;
	}

	bool GrabTry()
	{
		if (TryEnterCS(m_impl))
		{
			assert(!m_bGone);
			m_bGone = true;
			return true;
		}
		return false;
	}

	void Give()
	{
		assert(m_bGone);
		m_bGone = false;
		LeaveCS(m_impl);
	}
private:
	CSType		m_impl;
	bool		m_bGone;
};


class CMutexHolder
{
public:
	CMutexHolder(CSimpleMutex& mutex): m_mutex(mutex)
	{
		m_mutex.Grab();
	}

	~CMutexHolder()
	{
		m_mutex.Give();
	}

private:
	CSimpleMutex& m_mutex;
};
