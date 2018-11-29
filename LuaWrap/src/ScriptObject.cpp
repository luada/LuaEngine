#include "pch.h"
#include "LuaScriptMgr.h"
#include "ScriptObject.h"

using namespace LuaWrap;

CScriptObject::CScriptObject(CLuaScriptMgr* pLMgr): m_pLMgr(pLMgr), m_nScriptObjID(LUA_NOREF)
{
	assert(pLMgr != NULL);
}

CScriptObject::~CScriptObject()
{
	OnRelease();
}

void CScriptObject::SetScriptObjID(int nObjID)
{
	if (IsScriptObjValid())
	{
		lua_unref(m_pLMgr->GetLuaState(), m_nScriptObjID);
	}
	m_nScriptObjID = nObjID;
}

bool CScriptObject::CreateScriptObj(const String& sScriptPath)
{
	String sScriptFn;
	if (IsScriptObjValid())
	{
		//oldScriptObj:__onEngineObjLost()
		sScriptFn = m_sScriptClass + ".__onEngineObjLost";
		if(!ExcLuaFunctionEx(GetScriptObjID(), sScriptFn))
		{
		    SetScriptObjID(LUA_NOREF);
		}
	}

	String::size_type nSplitIndex = sScriptPath.rfind('/');
	if (nSplitIndex != String::npos)
	{
		const String sModuleName = "require \"" + sScriptPath.substr(0, nSplitIndex) + "\"";
		ExcLuaBufferEx(sModuleName.c_str(), sModuleName.length());
		m_sScriptClass = sScriptPath.substr(nSplitIndex + 1);
	}
	else
	{
		m_sScriptClass = sScriptPath;
	}

	//newScriptObj = NewScriptName.New()
	int nNewScriptObjID = LUA_NOREF;
	sScriptFn = m_sScriptClass + ".New";
	if(!ExcLuaFunctionEx(LUA_NOREF, sScriptFn, ">r", &nNewScriptObjID))
	{
		SetScriptObjID(LUA_NOREF);
		return false;
	}
	SetScriptObjID(nNewScriptObjID);

	//newScriptObj:__onEngineObjAttach(ref)
	sScriptFn = m_sScriptClass + ".__onEngineObjAttach";
	bool nResult = ExcLuaFunctionEx(GetScriptObjID(), sScriptFn, "o", this);

	return nResult;
}

bool CScriptObject::OnLoaded()
{
	bool nResult = false;
	if (IsScriptObjValid())
	{
		String sScriptFn = m_sScriptClass + ".__onLoaded";
		nResult = ExcLuaFunctionEx(GetScriptObjID(), sScriptFn);
	}
	return nResult;
}

bool CScriptObject::OnRelease()
{
	bool nResult = false;
	if (m_nScriptObjID != LUA_NOREF)
	{
		String sScriptFn = "release";
		nResult = ExcLuaFunctionEx(LUA_NOREF, sScriptFn, "r", m_nScriptObjID);
		lua_unref(m_pLMgr->GetLuaState(), m_nScriptObjID);
		m_nScriptObjID = LUA_NOREF;
	}
	return nResult;
}

CLuaScriptMgr& CScriptObject::GetLuaScriptMgr()
{
    return *m_pLMgr;
}


