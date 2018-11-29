#pragma once

#ifdef __WIN__
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#define	HANDLE	pthread_t
#endif

/// thread local declaration - ie "static THREADLOCAL(type) blah"
#if !defined(SINGLE_THREADED)
	#define THREADLOCAL(type) __declspec(thread) type
#else
	#define THREADLOCAL(type) type
#endif

typedef void (*SimpleThreadFunc)(void*);

struct CThreadTrampolineInfo
{
	CThreadTrampolineInfo(SimpleThreadFunc func, void* arg): m_func(func), m_arg(arg) {}

	SimpleThreadFunc m_func;
	void* m_arg;
};

class CSimpleThread
{
public:
	CSimpleThread(SimpleThreadFunc threadfunc, void * arg)
	{
		CThreadTrampolineInfo * info = new CThreadTrampolineInfo(threadfunc, arg);
	#ifdef __WIN__
		unsigned threadId;
		m_impl = HANDLE(_beginthreadex(0, 0, Trampoline, info, 0, &threadId));
	#else
		pthread_create(&m_impl, NULL, Trampoline, info);
	#endif
	}

	~CSimpleThread()
	{
	#ifdef __WIN__
		WaitForSingleObject(m_impl, INFINITE);
		CloseHandle(m_impl);
	#else
		pthread_join(m_impl, NULL);
	#endif
	}

	HANDLE Handle() const	{ return m_impl; }	/*exposed for more complex ops*/

private:
	HANDLE m_impl;

	/*
	 * this trampoline function is present so that we can hide the fact that windows
	 * and linux expect different function signatures for thread functions
	 */
#ifdef __WIN__
	static unsigned int _stdcall Trampoline(void* arg)
#else
	static void* Trampoline(void * arg)
#endif
	{
		CThreadTrampolineInfo * info = static_cast<CThreadTrampolineInfo*>(arg);
		info->m_func(info->m_arg);
		delete info;
		return 0;
	}
};
