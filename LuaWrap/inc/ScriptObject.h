#pragma once

#include "Type.h"
#include "Macro.h"

namespace LuaWrap{

class CLuaScriptMgr;

class CScriptObject
{
	ScriptWrapImplBegin(LuaWrap::CScriptObject)
	ScriptWrapImplEnd()
public:
    CScriptObject(CLuaScriptMgr* pLMgr);
    virtual ~CScriptObject();

//@BeginExportToLua
	bool IsScriptObjValid() const { return m_nScriptObjID != LUA_NOREF; }
	int  GetScriptObjID() const {  return m_nScriptObjID; }
	void SetScriptObjID(int nObjID);
	bool CreateScriptObj(const String& sScriptPath);
	void SetScriptClass(const String& scriptClass) { m_sScriptClass = scriptClass; }
	const String& GetScriptClass() const { return m_sScriptClass; }
//@EndExportToLua

	CLuaScriptMgr& GetLuaScriptMgr();

	bool OnLoaded();
	bool OnRelease();

protected:
	CLuaScriptMgr* m_pLMgr;
	int m_nScriptObjID;
	String m_sScriptClass;
};

}
