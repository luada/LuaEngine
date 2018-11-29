#pragma once

#include "Singleton.h"
#include "LuaScriptMgr.h"

namespace LuaWrap{

typedef int (*RegToLuaFunction) (lua_State *L);

class CAutoRegToLua
{
public:
	CAutoRegToLua(RegToLuaFunction pfnReg, const char* pszModule);
	~CAutoRegToLua() {}

	void RegToLua(lua_State* L);

private:
	RegToLuaFunction m_pfnReg;
	const char*	m_pszModule;
};

#define MAX_AUTO_REG_COUNT	10

//------------------------------------------------------------------------------------

class CAutoRegToLuaMgr: public CSingletonEx<CAutoRegToLuaMgr>
{
	DECLARE_SINGLETONEX(CAutoRegToLuaMgr)
public:
	void Add(CAutoRegToLua* cpReg);
	void RegToLua(lua_State* L);

	bool ShowRegInfo() const { return m_bShowRegInfo; }
	void ShowRegInfo(bool bShow) { m_bShowRegInfo = bShow; }

	void Clear();

private:
	bool m_bShowRegInfo;
	CAutoRegToLua*	m_regs[MAX_AUTO_REG_COUNT];
	int m_nRegCount;
};

}
