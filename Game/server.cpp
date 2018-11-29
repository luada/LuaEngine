#include "server.h"
#include "client.h"
#include "game.h"
#include "net.h"

namespace Demo
{
	CServer::CServer() 
		: m_net(NULL)
	{
	}

	CServer::~CServer()
	{

	}

	bool CServer::Init()
	{
		if (m_net != NULL) 
		{
			return false;
		}
		m_net = new CNet(*this, SvrPort);
		m_game = new CGame(*this, &m_luaMgr);

		m_luaMgr.OpenLua(GameID, ResVer);
		return m_game->Init();
	}

	void CServer::Run()
	{
		while (true) 
		{
			m_net->OnUpdate();
			m_game->OnUpdate();
			Sleep(3);
		}
	}

	int CServer::SendClientMsg(int socket, const char* buffer, int size)
	{
		return m_net->Send(socket, buffer, size);
	}

	int CServer::OnClientConnect(int socket)
	{
		CClient* client= m_clients[socket];
		if (client != NULL) 
		{
			m_net->Close(socket);
		}
		else
		{
			client = new CClient();
		}

		client->Init(socket);
		m_clients[socket] = client;

		return 0;

	}

	int CServer::OnClientClose(int socket)
	{
		ClientMap::iterator it = m_clients.find(socket);
		if (it == m_clients.end())
		{
			return -1;
		}

		delete it->second;
		m_clients.erase(it);
		return 0;
	}

	ClientMap& CServer::GetClients() 
	{
		return m_clients;
	}

	CNet* CServer::GetNet()
	{
		return m_net;
	}

	int CServer::OnClientRead(int socket, const char* buffer, int size)
	{
		ClientMap::iterator it = m_clients.find(socket);
		CClient* client = it == m_clients.end() ? NULL : it->second;
		if (client == NULL)
		{
			return -1;
		}

		return client->OnRead(buffer, size);
	}

	int CServer::GetDBGPackage(char* bufferOut, int size)
	{
		int len = -1;
		for (ClientMap::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
		{
			len = it->second->GetPackage(bufferOut, size);
			if (len > 0)
			{
				break;
			}
		}
		return len;
	}

	int CServer::SendDBGPackage(const char* buffer, int size)
	{
		short len = htons((short)size);
		for (ClientMap::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
		{
			int socket = it->second->GetSocket();
			m_net->Send(socket, (char*)&len ,sizeof(len));
			m_net->Send(socket, buffer, size);
		}
		return 0;
	}
}
