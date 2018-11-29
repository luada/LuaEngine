#include "pch.h"
#include "RemoteDebugger.h"
#include <iostream>


using namespace std;

#define DEFAULT_PLAYER_UIN  283582095
#define DEFAULT_GAME_ID		220

#define RCV_PKG_HEADER_SIZE 2

#define MAX_SND_BUFF_SIZE	1024
#define MAX_RCV_BUFF_SIZE	1024

#ifdef __WIN__
#else
	int GetLastError()
	{
		return errno;
	}

	void Sleep(unsigned int ms)
	{
		usleep(ms * 1000);
	}
	#define closesocket close
	#define ZeroMemory(dst, size)	memset(dst, 0, size)
#endif


static void RunThreadRcv(void* arg)
{
	CRemoteDebugger* This = (CRemoteDebugger*) arg;
	This->RcvRet();
}

CRemoteDebugger::CRemoteDebugger()
{
	m_pfWork = &CRemoteDebugger::Connect;
	m_socketMainSrv = INVALID_SOCKET;
	m_pSndBuff = new char[MAX_SND_BUFF_SIZE];
	m_pRcvBuff = new char[MAX_RCV_BUFF_SIZE];
	m_pThreadRcv = new CSimpleThread(RunThreadRcv, this);
	m_ushPort = 0;
}

CRemoteDebugger::~CRemoteDebugger()
{
	Quit();
#ifdef __WIN__
	WSACleanup();
#endif
	delete[] m_pSndBuff;
	delete[] m_pRcvBuff;
	delete   m_pThreadRcv;
}

bool CRemoteDebugger::Init()
{
#ifdef __WIN__
	WSADATA  ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) !=  0)
	{
		cout << "\n>初始化Windows Socket失败::" << GetLastError() << endl;
		return false;
	}
#endif

	m_socketMainSrv = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socketMainSrv == INVALID_SOCKET)
	{
		cout << "\n>创建Socket失败!::" << GetLastError() << endl;
		return false;
	}
	return true;
}

void CRemoteDebugger::Run()
{
	while (m_pfWork)
	{
		(this->*m_pfWork)();
	}
}

void CRemoteDebugger::Ping()
{
	SendData(NULL, 0);
}

void CRemoteDebugger::Quit()
{
	if (m_socketMainSrv != INVALID_SOCKET)
	{
		::closesocket(m_socketMainSrv);
	}
	m_pfWork = NULL;
}

void CRemoteDebugger::Connect()
{
	struct sockaddr_in srvAddr;
	ZeroMemory(&srvAddr, sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;

	if (m_ushPort == 0)
	{
		cout << ">请输入MainServer的IP地址、端口\n>";
		cin >> m_szAddr >> m_ushPort;

	}

	
	srvAddr.sin_addr.s_addr = inet_addr(m_szAddr);
	srvAddr.sin_port = htons(m_ushPort);

	int ret = ::connect(m_socketMainSrv, (struct sockaddr*)&srvAddr, sizeof(srvAddr));
	if (ret == SOCKET_ERROR)
	{
		m_ushPort = 0;
		cout << "\n>连接失败::" << GetLastError() << endl;
	}
	else
	{
		cout << "\n>连接成功!" << endl;
		m_pfWork = &CRemoteDebugger::SndCmd;
	}
}

void CRemoteDebugger::SndCmd()
{
	char* szCmd = m_pSndBuff + 2;
	cout << "\n>";
	cin.getline(szCmd, MAX_SND_BUFF_SIZE - 2);
	int nCmdLen = (int)strlen(szCmd);
	if (nCmdLen > 0)
	{
		if (ParseCmd(szCmd, nCmdLen))
		{
			*(short*)&m_pSndBuff[0] = (short)htons(nCmdLen);
			if (!SendData(m_pSndBuff, nCmdLen + 2))
			{
				cout << "\n>连接断开!\n>";
				::closesocket(m_socketMainSrv);
				m_socketMainSrv = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (m_socketMainSrv == INVALID_SOCKET)
				{
					cout << "\n>创建Socket失败!::" << GetLastError() << endl;
					m_pfWork = NULL;
				}
				else
				{
					m_pfWork = &CRemoteDebugger::Connect;
				}
			}
		}
		else
		{
			Help();
		}
	}
}

void CRemoteDebugger::RcvRet()
{
	char* pRcv;
	int   nLen;
	while (true)
	{
		//Ping();
		if(RecvData(&pRcv, nLen) && nLen > 0)
		{
			pRcv[nLen] = 0;
			cout << "\n" << pRcv << "\n>";
		}
		else
		{
			Sleep(500);
		}
	}
}

bool CRemoteDebugger::ParseCmd(const char* pData, int nLen)
{
	return true;
}

void CRemoteDebugger::Help()
{
}

bool CRemoteDebugger::SendData(const char* pData, int nLen)
{
	CMutexHolder holder(m_mutexSnd);
	int sendLen = 0;
	while (sendLen < nLen)
	{
		int nCurSndLen = ::send(m_socketMainSrv, pData + sendLen, nLen - sendLen, 0);
		if (nCurSndLen < 0)
		{
			Connect();
			return false;
		}
		sendLen += nCurSndLen;
	}
	return true;
}

bool CRemoteDebugger::RecvData(char** ppDataOut, int& nLenOut)
{
	*ppDataOut = NULL;
	nLenOut = 0;

	//(1) package header
	int rcvIndex = 0;
	int rcvLen = RCV_PKG_HEADER_SIZE;
	while(rcvLen > 0)
	{
		int curRcvLen = ::recv(m_socketMainSrv, m_pRcvBuff + rcvIndex, rcvLen, 0);
		if (curRcvLen < 0)
		{
			return false;
		}
		rcvIndex += curRcvLen;
		rcvLen -= curRcvLen;
	}

	//(2) message
	nLenOut = ntohs(*(short*) &m_pRcvBuff[RCV_PKG_HEADER_SIZE - 2]);
	rcvLen = nLenOut;
	*ppDataOut = m_pRcvBuff + RCV_PKG_HEADER_SIZE;
	while(rcvLen > 0)
	{
		int curRcvLen = ::recv(m_socketMainSrv, m_pRcvBuff + rcvIndex, rcvLen, 0);
		if (curRcvLen < 0)
		{
			*ppDataOut = NULL;
			nLenOut = 0;
			return false;
		}
		rcvIndex += curRcvLen;
		rcvLen -= curRcvLen;
	}
	return true;
}
