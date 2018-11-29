#pragma once

#ifdef __WIN__
	#include <WinSock2.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/select.h>
	#include <netdb.h>
	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <signal.h>
	#include <errno.h>
	#include <fcntl.h>

	typedef int 			SOCKET;
	typedef unsigned char	BYTE;
	typedef unsigned short	USHORT;

	#define	INVALID_SOCKET	-1
	#define SOCKET_ERROR	-1
#endif

#include "SimpleMutex.h"
#include "SimpleThread.h"


class CRemoteDebugger
{
public:
	CRemoteDebugger();
	~CRemoteDebugger();

	bool Init();
	void Run();
	void RcvRet();

private:
	void Connect();
	void SndCmd();
	void Ping();

	bool ParseCmd(const char* pData, int nLen);

	void Quit();
	void Help();

	bool SendData(const char* pData, int nLen);
	bool RecvData(char** ppDataOut, int& nLenOut);

	typedef void (CRemoteDebugger::*PrcFuncPtr)(void);

private:
	PrcFuncPtr	m_pfWork;
	CSimpleMutex m_mutexSnd;
	CSimpleThread* m_pThreadRcv;
	SOCKET		m_socketMainSrv;
	char		m_szAddr[256];
	USHORT		m_ushPort;
	char*		m_pSndBuff;
	char*		m_pRcvBuff;
};
