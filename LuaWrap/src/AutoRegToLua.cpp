#include "pch.h"
#include "AutoRegToLua.h"

using namespace LuaWrap;

CAutoRegToLua::CAutoRegToLua(RegToLuaFunction pfnReg, const char* pszModule):
		m_pfnReg(pfnReg), m_pszModule(pszModule)
{
	CAutoRegToLuaMgr::Instance().Add(this);
}

void CAutoRegToLua::RegToLua(lua_State* L)
{
	if (CAutoRegToLuaMgr::Instance().ShowRegInfo())
	{
		Log("CAutoRegToLua:(%s)", m_pszModule);
	}
	m_pfnReg(L);
}

//-----------------------------------------------------------------------------------------------------------------------------------

CAutoRegToLuaMgr::CAutoRegToLuaMgr(): m_nRegCount(0), m_bShowRegInfo(true)
{
}

CAutoRegToLuaMgr::~CAutoRegToLuaMgr()
{
	Clear();
}

void CAutoRegToLuaMgr::Clear()
{
	m_nRegCount = 0;
}

void CAutoRegToLuaMgr::Add(CAutoRegToLua* cpReg)
{
	assert(m_nRegCount < MAX_AUTO_REG_COUNT - 1);
	m_regs[m_nRegCount++] = cpReg;
}

void CAutoRegToLuaMgr::RegToLua(lua_State* L)
{
	for (int i = 0; i < m_nRegCount; ++i)
	{
		m_regs[i]->RegToLua(L);
	}
}


