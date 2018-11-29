#pragma once

#include "Singleton.h"
#include "ResIO.h"
#include "Type.h"
#include "Logger.h"
#include "LuaWrap.h"
#include "Util.h"


namespace LuaWrap{

#if LUA_EXC_TRACE_DISABLE
	#define		ExcLuaFileEx		GetLuaScriptMgr().ExcLuaFile
	#define		ExcLuaBufferEx		GetLuaScriptMgr().ExcLuaBuffer
	#define		ExcLuaFunctionEx	GetLuaScriptMgr().ExcLuaFunction
	#define		ExcLuaPropertyEx	GetLuaScriptMgr().ExcLuaProperty

	#define		TryExcLuaFileEx		CLuaSysLogTmpDisable tmpDisableSysLog(GetLuaScriptMgr().GetLuaState()); ExcLuaFileEx
	#define		TryExcLuaBufferEx	CLuaSysLogTmpDisable tmpDisableSysLog(GetLuaScriptMgr().GetLuaState());	ExcLuaBufferEx
	#define		TryExcLuaFunctionEx	CLuaSysLogTmpDisable tmpDisableSysLog(GetLuaScriptMgr().GetLuaState()); ExcLuaFunctionEx
	#define		TryExcLuaPropertyEx	CLuaSysLogTmpDisable tmpDisableSysLog(GetLuaScriptMgr().GetLuaState()); ExcLuaPropertyEx
#else
	#define		ExcLuaFileEx		GetLuaScriptMgr().Trace(__FILE__, __LINE__, __FUNCTION__).ExcLuaFile
	#define		ExcLuaBufferEx		GetLuaScriptMgr().Trace(__FILE__, __LINE__, __FUNCTION__).ExcLuaBuffer
	#define		ExcLuaFunctionEx	GetLuaScriptMgr().Trace(__FILE__, __LINE__, __FUNCTION__).ExcLuaFunction
	#define		ExcLuaPropertyEx	GetLuaScriptMgr().Trace(__FILE__, __LINE__, __FUNCTION__).ExcLuaProperty
	
	#define		TryExcLuaFileEx		CLuaSysLogTmpDisable tmpDisableSysLog(GetLuaScriptMgr().GetLuaState()); ExcLuaFileEx
	#define		TryExcLuaBufferEx	CLuaSysLogTmpDisable tmpDisableSysLog(GetLuaScriptMgr().GetLuaState());	ExcLuaBufferEx
	#define		TryExcLuaFunctionEx	CLuaSysLogTmpDisable tmpDisableSysLog(GetLuaScriptMgr().GetLuaState()); ExcLuaFunctionEx
	#define		TryExcLuaPropertyEx	CLuaSysLogTmpDisable tmpDisableSysLog(GetLuaScriptMgr().GetLuaState()); ExcLuaPropertyEx
#endif

#define CallObjScriptFn(fn)		ExcLuaFunctionEx(GetScriptObjID(), (m_sScriptClass + "."##fn))
#define CallObjScriptFnEx(fn, fmt, ...)	ExcLuaFunctionEx(GetScriptObjID(), (m_sScriptClass + "."##fn), fmt, __VA_ARGS__)

class CLuaScriptMgr
{
public:
	CLuaScriptMgr();
	~CLuaScriptMgr();

	bool 			OpenLua(int nGameId, int nResVer, bool bShowRegInfo = true);
	bool			CloseLua();
	bool			IsOpen();
	
	int				GetGameId() const;
	int				GetResVersion() const;

	void			EnableSysLog(bool bEnable);
	bool			EnableSysLog() const;

	lua_State*		GetLuaState(void) { return m_L; }
	CLuaScriptMgr&	Trace(const char* pszFilename, int nLineNum, const char* pszFunctionName);

	const char*		Compile(const char* pszFilename, int& nSize);
	const char*		GetLastError() const;

	bool			ExcLuaFile(const char* pszFilename);
	bool			ExcLuaBuffer(const char* pszLuaData, int nDataLen = 0);
	bool			ExcLuaFunction(int iSelf, const String& sFunctionName, const char* pszFmt = "", ...);
	bool			ExcLuaProperty(int iSelf, const String& sPropNames, const char* pszFmt = "", ...);

	CLuaScriptMgr&	GetLuaScriptMgr() { return *this; }

private:
	bool			PushLuaFunction(const String& sLbName);
	int				ParserInputArgs(const char* pszFmt, va_list& vl);
	int				ParserOutputArgs(const char* pszFmt, va_list& vl);
	void			TraceMsg();

private:
	lua_State*		m_L;
	const char*		m_pszFilename;
	const char*		m_pszFunctionName;
	int				m_nLineNum;
};

}
