#include "net.h"
#include "client.h"

namespace Demo
{
	CNet::CNet(CServer& server, int port) 
		:m_server(server)
	{

		WSADATA wsaData;
		WORD version = MAKEWORD(2, 2);
		if (WSAStartup(version, &wsaData) != 0)
		{
			LOG_ERROR("WSAStartup() error.");
			return;
		}


		m_socketListen = socket(AF_INET, SOCK_STREAM, 0);

		if (m_socketListen == INVALID_SOCKET)
		{
			WSACleanup();
			LOG_ERROR("socket() error.");
			return;
		}


		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(port);


		if (bind(m_socketListen, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			closesocket(m_socketListen);
			WSACleanup();
			LOG_ERROR("bind() error.");
			return;
		}

		if (listen(m_socketListen, SOMAXCONN) == SOCKET_ERROR)
		{
			closesocket(m_socketListen);
			WSACleanup();
			LOG_ERROR("listen() error.");
			return;
		}

		u_long ul=1;
		ioctlsocket(m_socketListen,FIONBIO,&ul);    //用非阻塞的连接

		FD_ZERO(&m_allSockSet);
		FD_SET(m_socketListen, &m_allSockSet);

		m_timeout.tv_sec = 0;
		m_timeout.tv_usec = 1000;

		LOG_DEBUG("正在监听...端口：%u\n", ntohs(addr.sin_port));
	}

	CNet::~CNet()
	{
		if (m_socketListen != INVALID_SOCKET)
		{
			if (closesocket(m_socketListen) == 0)
			{
				m_socketListen = INVALID_SOCKET;
				WSACleanup();
				return;
			}
			LOG_ERROR("close socket error.");
		}
	}

	void CNet::Close(int socket)
	{
		closesocket(socket);
	}


	void CNet::OnUpdate()
	{

		do 
		{
			fd_set fdRead = m_allSockSet;
			int selectRet = select(0, &fdRead, NULL, NULL, &m_timeout);

			if (selectRet == SOCKET_ERROR)
			{///SOCKET_ERROR == -1
				LOG_ERROR("listen() error.");
				break; 
			}


			if (selectRet == 0)
			{
				//LOG_ERROR("listen() time out.");
				break; 
			}


			for (ClientMap::iterator it = m_server.GetClients().begin(); it != m_server.GetClients().end();)
			{
				int client = (it->second == NULL ? -1 : it->second->GetSocket());
				++it;

				if (!FD_ISSET(client, &fdRead))
				{
					closesocket(client);

					FD_CLR(client, &m_allSockSet);
					LOG_DEBUG("目前客户端的数量为：%u\n", m_allSockSet.fd_count - 1);

					m_server.OnClientClose(client);
				}
				else
				{
					sockaddr_in addr;
					int addr_len = sizeof(addr);
					getpeername(client, (struct sockaddr *)&addr, &addr_len);

					char ip_addr[16] = { 0 };
					///inet_ntop将ip地址从2进制->点10进制
					inet_ntop(AF_INET, &addr, ip_addr, sizeof(ip_addr));

					char recvBuffer[1024 * 100];
					int rcvRet = recv(client, recvBuffer, sizeof(recvBuffer), 0);
					if (rcvRet == SOCKET_ERROR)
					{
						DWORD err = WSAGetLastError();
						if (err == WSAECONNRESET)
						{
							LOG_DEBUG("客户端[%s:%u]被强行关闭.\n", ip_addr, ntohs(addr.sin_port));
						}
						else
						{
							LOG_ERROR("recv() error.");
						}
						closesocket(client);

						FD_CLR(client, &m_allSockSet);
						LOG_DEBUG("目前客户端的数量为：%u\n", m_allSockSet.fd_count - 1);

						m_server.OnClientClose(client);
						break;
					}
					else if (rcvRet == 0) 
					{
						closesocket(client);

						FD_CLR(client, &m_allSockSet);
						LOG_DEBUG("客户端[%s:%u]已经退出，目前客户端的数量为：%u.\n", ip_addr, 
							ntohs(addr.sin_port), m_allSockSet.fd_count - 1);

						m_server.OnClientClose(client);
						break;
					}

					m_server.OnClientRead(client, recvBuffer, rcvRet);
				}

			}

			if (FD_ISSET(m_socketListen, &fdRead))
			{
				sockaddr_in addr;
				int addr_len = sizeof(addr);
				int client = accept(m_socketListen, (sockaddr*)&addr, &addr_len);
				if (client == INVALID_SOCKET){
					LOG_ERROR("accept() error.");
					break;
				}
				FD_SET(client, &m_allSockSet);   
				char szAddr[16] = { 0 };
				inet_ntop(AF_INET, &addr, szAddr, sizeof(szAddr));
				LOG_DEBUG("有新的连接[%s:%u],目前客户端的数量为：%u\n", szAddr, 
					ntohs(addr.sin_port), m_allSockSet.fd_count - 1);

				m_server.OnClientConnect(client);
			}
			
		} while (0);

	}

	int CNet::Send(int socket, const char* buffer, int size)
	{
		return send(socket, buffer, size, 0);
	}

	void CNet::Clear()
	{

		for (u_int i = 1; i < m_allSockSet.fd_count; ++i) 
		{
			int socket_set = m_allSockSet.fd_array[i];
			if (socket_set != INVALID_SOCKET)
			{
				closesocket(socket_set);
			}
			FD_CLR(socket_set, &m_allSockSet);
		}
		WSACleanup();
	}
}

