#pragma once 

#include <map>
#include <vector>
#include <stdio.h>
#include <tchar.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <string.h>
#include "const.h"
#include "type.h"
#include "define.h"

#include "LuaWrap.h"
#include "MemMgr.h"
#include "LuaScriptMgr.h"
#include "ScriptObject.h"


namespace Demo 
{
	class CClient;
	class CNet;
	class CGame;

	typedef std::map<int, CClient*> ClientMap;

	class CServer 
	{
	public:
		CServer();
		~CServer();

		bool Init();
		void Run();

		
		int SendClientMsg(int socket, const char* buffer, int size);
		int OnClientRead(int socket, const char* buffer, int size);
		int OnClientConnect(int socket);
		int OnClientClose(int socket);
		
		ClientMap& GetClients();
		CNet* GetNet();
		int GetDBGPackage(char* bufferOut, int size);
		int SendDBGPackage(const char* buffer, int size);

	private:
		ClientMap m_clients;
		LuaWrap::CLuaScriptMgr m_luaMgr;
		CNet* m_net;;
		CGame* m_game;
	};
}
