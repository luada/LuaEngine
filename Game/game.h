#pragma once

#include "server.h"

namespace Demo
{
	class CGame: public LuaWrap::CScriptObject
	{
		ScriptWrapImplBegin(Demo::CGame)
		ScriptWrapImplEnd()
	public:
		CGame(CServer& server, LuaWrap::CLuaScriptMgr* pLMgr);
		virtual ~CGame();

		bool Init();
		void OnUpdate();

//@BeginExportToLua
		const char* ReadDbgInput();
		void WriteDbgOutput(const char* info);
//@EndExportToLua

	private:
		const char* DoReadDbgInput();

	private:
		CServer& m_server;
		char m_dbgBuffer[1024];

	};
}
