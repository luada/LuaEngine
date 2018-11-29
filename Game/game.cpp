#include "game.h"
#include "net.h"

namespace Demo
{
	CGame::CGame(CServer& server, LuaWrap::CLuaScriptMgr* pLMgr)
		: LuaWrap::CScriptObject(pLMgr)
		, m_server(server)
	{

	}

	CGame::~CGame() 
	{

	}

	bool CGame::Init()
	{
		if (!CreateScriptObj("game/Game")) 
		{
			return false;
		}

		bool ret = false;
		CallObjScriptFnEx("init", ">b", &ret);
		return ret;
	}

	void CGame::OnUpdate()
	{
		CallObjScriptFn("onUpdate");

		const char* dgbCmd = DoReadDbgInput();
		if (dgbCmd != NULL && dgbCmd[0])
		{
			CallObjScriptFnEx("onDbgCmd", "s", dgbCmd);
		}
	}

	const char* CGame::DoReadDbgInput()
	{
		m_dbgBuffer[0] = '\0';
		m_server.GetDBGPackage(m_dbgBuffer, sizeof(m_dbgBuffer));
		return m_dbgBuffer;
	}

	const char* CGame::ReadDbgInput()
	{
		m_server.GetNet()->OnUpdate();
		return DoReadDbgInput();
	}

	void CGame::WriteDbgOutput(const char* info)
	{
		m_server.SendDBGPackage(info, strlen(info));
	}
}
