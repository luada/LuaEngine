#pragma once


#include "server.h"

#pragma comment(lib, "ws2_32.lib")   

namespace Demo 
{
	class CNet
	{
	public:
		CNet(CServer& server, int port);
		~CNet();

		void OnUpdate();
		void Clear();

		int Send(int socket, const char* buffer, int size);
		void Close(int socket);

	private:
		int m_socketListen;
		fd_set m_allSockSet;
		timeval m_timeout;
		CServer& m_server;
	};

}
