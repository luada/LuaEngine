#pragma once

extern "C"
{
	bool IsEnableLuaLog(lua_State* L);
	void EnableLuaLog(lua_State* L, bool bEnable);
}

namespace LuaWrap{
//----------------------------------------------------------------------------------------------------
class CLuaTopStackRecover
{
public:
	CLuaTopStackRecover(lua_State* L):m_L(L)
	{
		m_nTopStack = m_L ? lua_gettop(m_L) : 0;
	}

	~CLuaTopStackRecover()
	{
		if (m_L)
		{
			lua_settop(m_L, m_nTopStack);
		}
	}

	void disableRecover() { m_L = NULL; }

private:
	lua_State* m_L;
	int m_nTopStack;
};

//----------------------------------------------------------------------------------------------------

class CLuaSysLogTmpDisable
{
public:
	CLuaSysLogTmpDisable(lua_State* L):m_L(L), m_bEnable(true)
	{
		if (m_L)
		{
			m_bEnable = IsEnableLuaLog(m_L);
			EnableLuaLog(m_L, false);
		}
	}

	~CLuaSysLogTmpDisable()
	{
		if (m_L)
		{
			EnableLuaLog(m_L, m_bEnable);
		}
	}

private:
	lua_State* m_L;
	bool m_bEnable;
};

//----------------------------------------------------------------------------------------------------
class CLuaUserData
{
public:
	CLuaUserData(lua_State* L, void* value, const char* type): m_L(L)
	{
		tolua_pushusertype(m_L, value, type);
		m_nRef = luaL_ref(m_L, LUA_REGISTRYINDEX);
	}

	~CLuaUserData()
	{
		luaL_unref(m_L, LUA_REGISTRYINDEX, m_nRef);
	}

	operator int() const { return m_nRef; }
	int Ref() const		 { return m_nRef; }
private:
	lua_State*	m_L;
	int			m_nRef;
};
}

