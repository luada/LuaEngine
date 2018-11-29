#include "pch.h"
#include "MemMgr.h"
#include "LuaScriptMgr.h"
#include "AutoRegToLua.h"
#include "ScriptObject.h"
#include "DumpLuaStack.h"
#include "Util.h"

#ifdef _WIN32
	#include <io.h>
	#define snprintf	sprintf_s
#else //linux
	#include <sys/stat.h>
	#include <system.h>
#endif

using namespace LuaWrap;

enum enmLuaScriptMgrCode
{
    LEC_SUCESS = 0,
    LEC_EXCEPTION,
    LEC_UNKNOWN,
    LEC_NO_FUNCTION,
    LEC_INPUT_TYPE,
    LEC_OUT_TYPE,
    LEC_EMPTY_LABEL_NAME,
};

static const char* LuaScriptMgrMsg[] =
{
    "work well done",
    "exception encountered",
    "unknown error",
    "no such function",
    "invalid input param type",
    "invalid output param type",
	"empty label name"
};

enum enmLuaDumpFlag
{
	LDF_OFF,
	LDF_FULL,
	LDF_MIN,
};


#define RegLuaGlobalFunc(func)  lua_register(L, #func, lua_##func)

#define	NULL				0
#define MAX_PATH			260

#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))

#define GameIdFlag	"__GAME_ID_"
#define ResVerFlag	"__RES_VER_"
#define LuaDumpFlag "__LUA_DUMP_"
#define LuaEnableLogFlag  "__LUA_ENABLE_LOG_"

static const int			MSG_BUFF_SIZE		= 256;
static const char* const	UNKNOW_FILE_NAME	= "(unknown source file)";
static const char* const	UNKNOW_FUNCTION		= "(unknown function)";

//------------------------------------------------------------------------------------------------
static void OnDumpLuaStack(void *ud, const char *fmt, ...);
static int CheckDumpLuaRecord(void *ud, const void *record);

static int GetLuaDumpFlag(lua_State *L);
static void SetLuaDumpFlag(lua_State *L, int flag);

typedef void (*lua_ondumpstack) (void *ud, const char *fmt, ...);
typedef int  (*lua_checkrecord)(void *ud, const void *record);

static int lua_dumpstack (lua_State *L, lua_ondumpstack writefunc,
						   lua_checkrecord checkfunc, void *ud);
static int lua_mindumpstack (lua_State *L, lua_ondumpstack writefunc,
							  lua_checkrecord checkfunc, void *ud);
//------------------------------------------------------------------------------------------------
extern "C"
{
	void  FreeMemory(void *ptr, size_t size)
	{
		Free(ptr, size);
	}

	void* ReallocMemory(void *ptr, size_t osize, size_t nsize)
	{
		void* ptrNew = Malloc(nsize);
		memcpy(ptrNew, ptr, min(osize, nsize));
		FreeMemory(ptr, osize);
		return ptrNew;
	}

	int EnterRecordRequire(lua_State *L, const char* name)
	{
		int nResult = 0;
		CLuaTopStackRecover topStackRecover(L);
		lua_getglobal(L, "OnEnterRecordRequire");
		if (!lua_isfunction(L, -1))
		{
			return 1;
		}
		lua_pushstring(L, name);
		nResult = lua_pcall(L, 1, 0, 0);
		if (nResult != 0)
		{
			Log(lua_tostring(L, -1));
		}
		return nResult;
	}

	int LeaveRecordRequire(lua_State *L, const char* name)
	{
		int nResult = 0;
		CLuaTopStackRecover topStackRecover(L);
		lua_getglobal(L, "OnLeaveRecordRequire");
		if (!lua_isfunction(L, -1))
		{
			return 1;
		}
		lua_pushstring(L, name);
		nResult = lua_pcall(L, 1, 0, 0);
		if (nResult != 0)
		{
			Log(lua_tostring(L, -1));
		}
		return nResult;
	}

	bool IsEnableLuaLog(lua_State* L)
	{
		lua_getglobal(L, LuaEnableLogFlag);
		bool bRet =  !!lua_toboolean(L, -1);
		lua_pop(L, 1);
		return bRet;
	}

	void EnableLuaLog(lua_State* L, bool bEnable)
	{
		lua_pushboolean(L, bEnable);
		lua_setglobal(L, LuaEnableLogFlag);
	}

	void LogEx(lua_State* L, const char* pszFmt, ...)
	{
#ifdef _USE_LOG_
		if (IsEnableLuaLog(L))
		{
			char szBuff[1024];
			va_list ap;
			va_start(ap, pszFmt);
			vsnprintf(szBuff, sizeof(szBuff), pszFmt, ap);
			va_end(ap);
			Log(szBuff);
		}
#endif
	}

	void ScriptMsg(lua_State *L, const char* msg)
	{
		if (IsEnableLuaLog(L))
		{
			Log(msg);
		}
	}
}

//------------------------------------------------------------------------------------------------
static void OnDumpLuaStack(void *pUserData, const char *pszFmt, ...)
{
	char buff[1024];
	CDumpLuaStack* pDmp = (CDumpLuaStack*)pUserData;
	va_list ap;
	va_start(ap, pszFmt);
	vsprintf(buff, pszFmt, ap);
	pDmp->OnDumpLuaStack(buff, strlen(buff));
	va_end(ap);
}

static int CheckDumpLuaRecord(void *pUserData, const void *pRecord)
{
	CDumpLuaStack* pDmp = (CDumpLuaStack*)pUserData;
	return pDmp->CheckRecord(pRecord);
}

//------------------------------------------------------------------------------------------------
static int GetLuaDumpFlag(lua_State *L)
{
	lua_getglobal(L, LuaDumpFlag);
	int nRet = (int) lua_tonumber(L, -1);
	lua_pop(L, 1);
	return nRet;
}

static void SetLuaDumpFlag(lua_State *L, int flag)
{
	lua_pushnumber(L, flag);
	lua_setglobal(L, LuaDumpFlag);
}

//------------------------------------------------------------------------------------------------
static int GetGameId(lua_State *L)
{
	lua_getglobal(L, GameIdFlag);
	int nId = (int)luaL_checknumber(L, -1);
	lua_pop(L, 1);
	return nId;
}

static void SetGameId(lua_State *L, int nGameId)
{
	lua_pushnumber(L, nGameId);
	lua_setglobal(L, GameIdFlag);
}

//------------------------------------------------------------------------------------------------
static int GetResVersion(lua_State *L)
{
	lua_getglobal(L, ResVerFlag);
	int nVer = (int)luaL_checknumber(L, -1);
	lua_pop(L, 1);
	return nVer;
}

static void SetResVersion(lua_State *L, int nVer)
{
	lua_pushnumber(L, nVer);
	lua_setglobal(L, ResVerFlag);
}

//------------------------------------------------------------------------------------------------
static int lua_getGameId(lua_State *L)
{
	lua_getglobal(L, GameIdFlag);
	return 1;
}

static int lua_setGameId(lua_State *L)
{
	lua_setglobal(L, GameIdFlag);
	return 0;
}

static int lua_setEnableLuaLog(lua_State* L)
{
	lua_setglobal(L, LuaEnableLogFlag);
	return 0;
}

static int lua_getEnableLuaLog(lua_State* L)
{
	lua_getglobal(L, LuaEnableLogFlag);
	return 1;
}

static int lua_setLuaDumpFlag(lua_State *L)
{
	lua_setglobal(L, LuaDumpFlag);
	return 0;
}

static int lua_arraySize(lua_State* L)
{
	if (!lua_istable(L, -1))
	{
		return 0;
	}

	int maxIdx = 0;
	lua_pushnil(L);
	while(lua_next(L, -2))
	{
		lua_pop(L, 1);
		if (!lua_isnumber(L, -1))
		{
			continue;
		}
		int idx = lua_tointeger(L, -1);
		maxIdx = MAX(idx, maxIdx);
	}
	lua_pop(L, 1);
	lua_pushinteger(L, maxIdx);
	return 1;
}

static int lua_unpackArray(lua_State *L)
{
	if (!lua_istable(L, -1))
	{
		return 0;
	}

	int maxIdx = 0;
	lua_pushnil(L);
	while(lua_next(L, -2))
	{
		lua_pop(L, 1);
		if (!lua_isnumber(L, -1))
		{
			continue;
		}
		int idx = lua_tointeger(L, -1);
		maxIdx = MAX(idx, maxIdx);
	}

	for (int i = 1; i <= maxIdx; ++i)
	{
		lua_rawgeti(L, -1, i);
		lua_insert(L, -2);
	}
	lua_pop(L, 1);

	return maxIdx;
}

//------------------------------------------------------------------------------------------------

static int lua_isarray(lua_State *L, int idx)
{
	int index = 1;
	if (!lua_istable(L, idx)) 
	{
		return 0;
	}

	lua_pushnil(L);
	if (idx < 0)
	{
		--idx; /*because we have push a nil value to the stack*/
	}

	while (lua_next(L, idx))
	{
		if (!lua_isnumber(L, -2) || lua_tointeger(L, -2) != index++)
		{
			lua_pop(L, 2);
			return 0;
		}
		lua_pop(L, 1);
	}
	return 1;
}

static void lua_dumpvalue(lua_State *L, lua_ondumpstack writefunc, lua_checkrecord checkfunc, void *ud)
{
	const char* value = NULL;
	switch (lua_type(L, -1))
	{
		case LUA_TTABLE:
			{
				const void* tp = lua_topointer(L, -1);
				writefunc(ud, "<table:%p>", tp);
				if (!checkfunc(ud, tp))
				{
					int count = 0;
					writefunc(ud, "{");
					if (lua_isarray(L, -1))
					{
						lua_pushnil(L);
						while (lua_next(L, -2))
						{
							if (count++ != 0)
							{
								writefunc(ud, ", ");
							}
							lua_dumpvalue(L, writefunc, checkfunc, ud);
							lua_pop(L, 1);
						}
					}
					else
					{
						const char* key;
						lua_pushnil(L);
						while (lua_next(L, -2))
						{
							if (count++ != 0)
							{
								writefunc(ud, ", ");
							}
							lua_pushvalue(L, -2);
							key = lua_tostring(L, -1);
							lua_pop(L, 1);
							writefunc(ud, "%s=", key ? key : "<?>");
							lua_dumpvalue(L, writefunc, checkfunc, ud);
							lua_pop(L, 1);
						}
					}
					writefunc(ud, "}");
				}
			}
			break;
		case LUA_TFUNCTION:
			{
				int index = 1;
				const void* tp = lua_topointer(L, -1);
				writefunc(ud, "<function:%p>", tp);
				if (!checkfunc(ud, tp))
				{
					int count = 0;
					writefunc(ud, "(");
					for (;;)
					{
						const char* name = lua_getupvalue(L, -1, index++);
						if (!name)
						{
							break;
						}

						if (count++ != 0)
						{
							writefunc(ud, ", ");
						}

						writefunc(ud, "%s=", name);
						lua_dumpvalue(L, writefunc, checkfunc, ud);
						lua_pop(L, 1);
					}
					writefunc(ud, ")");
				}
			}
			break;
		case LUA_TUSERDATA:
			{
				writefunc(ud, "<user data:%p>", lua_topointer(L, -1));
			}
			break;
		case LUA_TLIGHTUSERDATA:
			{
				writefunc(ud, "<light user data:%p>", lua_topointer(L, -1));
			}
			break;
		case LUA_TTHREAD:
			{
				writefunc(ud, "<thread:%p>", lua_topointer(L, -1));
			}
			break;
		case LUA_TBOOLEAN:
			{
				value = lua_toboolean(L, -1) == 0 ? "false" : "true";
				writefunc(ud, "%s", value);
			}
			break;
		case LUA_TNIL:
			{
				writefunc(ud, "nil", value);
			}
			break;
		default:
			{
				value = lua_tostring(L, -1);
				writefunc(ud, "%s", value ? value : "<?>");
			}
			break;
	}
}

static int lua_dumpstack (lua_State *L, lua_ondumpstack writefunc, lua_checkrecord checkfunc, void *ud)
{
	int index;
	int level = 0; 
	lua_Debug ar;
	if (lua_isstring(L, -1))
	{
		writefunc(ud, "\n**************************************************\n%s",
			lua_tostring(L, -1));
	}

	while (lua_getstack(L, level++, &ar)) 
	{
		lua_getinfo(L, "flnSu", &ar); 
		writefunc(ud, "\n--------------------------------------------------\n");
		writefunc(ud, "stack info:\n");
		writefunc(ud, " name=%s\n namewhat=%s\n what=%s\n source=%s\n"
			" currentline=%d\n nups=%d\n linedefined=%d\n lastlinedefined=%d\n"
			" short_src=%s\n\n", 
			ar.name, ar.namewhat, ar.what, ar.source, ar.currentline, 
			ar.nups, ar.linedefined, ar.lastlinedefined, ar.short_src);
		
		writefunc(ud, "local:");
		index = 1;
		for (;;)
		{
			const char* name = lua_getlocal(L, &ar, index++);
			if (!name)
			{	
				break;
			}
			writefunc(ud, "\n %s=", name);
			lua_dumpvalue(L, writefunc, checkfunc, ud);
			lua_pop(L, 1); /*lua_getlocal*/
		}
		lua_pop(L, 1); /*lua_getinfo*/
	}
	return 0;
}

static int lua_mindumpstack (lua_State *L, lua_ondumpstack writefunc, lua_checkrecord checkfunc, void *ud)
{
	int level = 0; 
	lua_Debug ar;
	if (lua_isstring(L, -1))
	{
	   writefunc(ud, lua_tostring(L, -1));
	}

	while (lua_getstack(L, level++, &ar)) 
	{
	   lua_getinfo(L, "flnS", &ar);
	   if (ar.currentline > 0)
	   {
		   writefunc(ud, "\n%s:%s(%d)", ar.source, ar.name, ar.currentline);
	   }
	   lua_pop(L, 1); /*lua_getinfo*/
	}
	return 0;
}

static int lua_onDump(lua_State* L)
{
#ifdef WIN32
	const char* pszError = lua_tostring(L, -1);
	ScriptMsg(L, pszError);
#endif
	CLuaTopStackRecover topStackRecover(L);
	int flag = GetLuaDumpFlag(L);
	if (flag == LDF_OFF)
	{
		return 0;
	}
	SetLuaDumpFlag(L, LDF_OFF);

	CDumpLuaStack dmp;
	if (flag == LDF_FULL)
	{
		lua_dumpstack(L, OnDumpLuaStack, CheckDumpLuaRecord, &dmp);

	}
	else //default LDF_MIN
	{
		lua_mindumpstack(L, OnDumpLuaStack, CheckDumpLuaRecord, &dmp);
	}

	lua_getglobal(L, "onLuaDump");
	if (lua_isfunction(L, -1))
	{
		bool bRet = false;

		dmp.PushChar(0);
		const char* pszErrInfo = dmp.GetInfo();
		ScriptMsg(L, pszErrInfo);
		lua_pushstring(L, pszErrInfo);
		lua_pushboolean(L, 0);
		lua_pcall(L, 2, 1, 0);
		bRet = !!lua_toboolean(L, -1);

		if (bRet && flag == LDF_MIN && GetLuaDumpFlag(L) == LDF_FULL)
		{
			lua_pop(L, 1);

			dmp.Clear();
			lua_dumpstack(L, OnDumpLuaStack, CheckDumpLuaRecord, &dmp);

			lua_getglobal(L, "onLuaDump");
			if (lua_isfunction(L, -1))
			{
				dmp.PushChar(0);
				lua_pushstring(L, dmp.GetInfo());
				lua_pushboolean(L, 1);
				lua_pcall(L, 2, 1, 0);
				bRet = !!lua_toboolean(L, -1);
			}
		}
	}
	SetLuaDumpFlag(L, LDF_OFF);
	return 0;
}

//------------------------------------------------------------------------------------------------
static int lua_getResVersion(lua_State *L)
{
	lua_getglobal(L, ResVerFlag);
	return 1;
}

static int lua_setResVersion(lua_State *L)
{
	lua_setglobal(L, ResVerFlag);
	return 0;
}
//------------------------------------------------------------------------------------------------
static int lua_newRegistryRef(lua_State *L)
{
	int r = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_pushnumber(L, r);
	return 1;
}

static int lua_releaseRegistryRef(lua_State* L)
{
	int r = (int)luaL_checknumber(L, -1);
	luaL_unref(L, LUA_REGISTRYINDEX, r);
	return 0;
}

static int lua_getRegistryObj(lua_State* L)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(L,1,0,&tolua_err) ||
		!tolua_isnoobj(L,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int iSelf = ((int)  tolua_tonumber(L,1,0));
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, iSelf);
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(L,"#ferror in function 'getRegistryObj'.",&tolua_err);
	return 0;
#endif
}

static int lua_objectDynamicCast(lua_State* L)
{
	int nTop = lua_gettop(L);
	if (lua_isuserdata(L, 1) && (lua_isstring(L, 2) || nTop < 2) && nTop < 3)
	{
		CScriptObject* pObj = (CScriptObject*)  tolua_tousertype(L, 1, NULL);
		const char* pszClassName = NULL;
		if (lua_isstring(L, 2))
		{
			pszClassName = tolua_tostring(L, 2, NULL);
		}
		void * This = pObj->GetThis(pszClassName);
		if (This != NULL)
		{
			pszClassName = (pszClassName == NULL) ? pObj->ClassName() : pszClassName;
			tolua_pushusertype(L, This, pszClassName);
			return 1;
		}
	}
	luaL_error(L, "invalid 'arguments' in function 'objectDynamicCast(CScriptObject, pszClassName = nil)'");
	return 0;
}

static int lua_loadRes(lua_State* L)
{
	int nTop = lua_gettop(L);
	if (nTop == 1 && lua_isstring(L, 1))
	{
		const char* pszFilename = lua_tostring(L, 1);
		int nGameId = GetGameId(L);
		int nVer = GetResVersion(L);
		int nSize = 0;
		const char* pcFileData = LoadRes(pszFilename, nGameId, nVer, nSize);
		char* pcData = (char*) Malloc(nSize + 1);
		memcpy(pcData, pcFileData, nSize);
		pcData[nSize] = 0;
		lua_pushstring(L, pcData);
		Free(pcData, nSize + 1);
		UnloadRes(pcFileData, pszFilename, nGameId, nVer, nSize);
		return 1;
	}
	luaL_error(L, "invalid 'arguments' in function 'loadRes(pszFilename)'");
	return 0;
}

static int lua_sleep(lua_State* L)
{
	int nTop = lua_gettop(L);
	if (nTop == 1 && lua_isnumber(L, 1))
	{
		int ms = (int)lua_tonumber(L, 1);
#ifdef WIN32
		Sleep((DWORD)ms);
#else
		usleep(ms * 1000);
#endif
		return 1;
	}
	luaL_error(L, "invalid 'arguments' in function 'sleep(ms)'");
	return 0;
}

static int lua_createDir(lua_State* L)
{
	int nTop = lua_gettop(L);
	if (nTop == 1 && lua_isstring(L, 1))
	{
		const char* path = lua_tostring(L, 1);
		bool bRet = false;

		if (access(path, 0) != -1)
		{
			bRet = true;
		}
		else
		{
			String fullPath(path);
			int index = 0;
			do
			{
				index = fullPath.find('/', index + 1);
				int len = index == String::npos ? fullPath.length() : index;
				String curPath = fullPath.substr(0, index);
#ifdef _WIN32
				CreateDirectory(curPath.c_str(), NULL);
#else
				mkdir(curPath.c_str(), 0777);
#endif
			} while (index != String::npos);
			bRet = access(path, 0) != -1;
		}
		lua_pushboolean(L, bRet);
		return 1;
	}
	luaL_error(L, "invalid 'path' in function 'createDir(path)'");
	lua_pushboolean(L, false);
	return 1;
}

static int lua_disturbArray(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	int n = luaL_getn(L, 1);
	for (int i = 0; i < n; ++i)
	{
		int a = rand() % n + 1;
		int b = rand() % n + 1;
		lua_rawgeti(L, 1, a);
		lua_rawgeti(L, 1, b);
		lua_rawseti(L, 1, a);
		lua_rawseti(L, 1, b);
	}
	return 0;
}

static int lua_getTracebackCount(lua_State *L) 
{
	lua_Debug ar;
	int index = 1;
	while (lua_getstack(L, index, &ar))
	{
		index++;
	}
	lua_pushnumber( L, index - 1 );
	return 1;
}

static void RegLuaGlobalFuncs(lua_State* L)
{
	RegLuaGlobalFunc(newRegistryRef);
	RegLuaGlobalFunc(releaseRegistryRef);
	RegLuaGlobalFunc(objectDynamicCast);
	RegLuaGlobalFunc(loadRes);
	RegLuaGlobalFunc(sleep);
	RegLuaGlobalFunc(createDir);
	RegLuaGlobalFunc(unpackArray);
	RegLuaGlobalFunc(arraySize);
	RegLuaGlobalFunc(getRegistryObj);
	RegLuaGlobalFunc(getResVersion);
	RegLuaGlobalFunc(setResVersion);
	RegLuaGlobalFunc(getGameId);
	RegLuaGlobalFunc(setGameId);
	RegLuaGlobalFunc(setLuaDumpFlag);
	RegLuaGlobalFunc(disturbArray);
	RegLuaGlobalFunc(getTracebackCount);
	RegLuaGlobalFunc(getEnableLuaLog);
	RegLuaGlobalFunc(setEnableLuaLog);
}

//------------------------------------------------------------------------------------------------
static int loadfile(lua_State* L, const char* pszFilename)
{
	int nStatus = -1;
	int nSize;
	int nGameId = GetGameId(L);
	int nResVer = GetResVersion(L);
	const char* pData = LoadRes(pszFilename, nGameId, nResVer, nSize);
	if(pData != NULL)
	{
		nStatus = luaL_loadbuffer(L, pData, nSize, pszFilename);
	}
	else
	{
		char szErrMsg[MSG_BUFF_SIZE];
		snprintf(szErrMsg, sizeof(szErrMsg), "Can not open file: %s!", pszFilename);
		lua_pushstring(L, szErrMsg);
	}
	UnloadRes(pData, pszFilename, nGameId, nResVer, nSize);
	return nStatus;
}

static int loadlua(lua_State* L)
{
	const char* pszName = luaL_gsub(L, luaL_checkstring(L, 1), ".", "/");
	char szFilename[MSG_BUFF_SIZE];
	snprintf(szFilename, sizeof(szFilename), "%s.luc", pszName);
	int nStatus = loadfile(L, szFilename);
	if (nStatus == -1)
	{
		snprintf(szFilename, sizeof(szFilename), "%s.lua", pszName);
		nStatus = loadfile(L, szFilename);
	}
	if (nStatus != 0)
	{
		luaL_error(L, "error loading module %s from file %s:\n\t%s",
			lua_tostring(L, 1), pszName, lua_tostring(L, -1));
	}
	return 1;
}

static int setLoader(lua_State*L, lua_CFunction fn)
{
	lua_getglobal(L, LUA_LOADLIBNAME);
	if (lua_istable(L, -1))
	{
		lua_getfield(L, -1, "loaders");
		if (lua_istable(L, -1))
		{
			lua_pushcfunction(L, fn);
			lua_rawseti(L, -2, 2);
			return 0;
		}
	}
	return -1;
}

static int luc_writer(lua_State* L, const void* pIn, size_t size, void* pOut)
{
	if (size > 0)
	{
		Vector(char)& out = *(Vector(char)*)pOut;
		int nLen = out.size();
		out.resize(nLen + size);
		memcpy(&out[nLen], pIn, size);
	}
	return 0;
}


//------------------------------------------------------------------------------------------------
CLuaScriptMgr::CLuaScriptMgr() :
			m_L(NULL),
			m_pszFilename(UNKNOW_FILE_NAME),
			m_pszFunctionName(UNKNOW_FUNCTION),
			m_nLineNum(0)
{
}

CLuaScriptMgr::~CLuaScriptMgr()
{
	CloseLua();
}

bool CLuaScriptMgr::OpenLua(int nGameId, int nResVer, bool bShowRegInfo)
{
	CloseLua();

	m_L = lua_open();
	if(m_L == NULL)
	{
		return false;
	}

	lua_atpanic(m_L, NULL);
	luaL_openlibs(m_L);
	RegLuaGlobalFuncs(m_L);
	CAutoRegToLuaMgr::Instance().ShowRegInfo(bShowRegInfo);
	CAutoRegToLuaMgr::Instance().RegToLua(m_L);

	setLoader(m_L, loadlua);
	lua_checkstack(m_L, 512);

	SetGameId(m_L, nGameId);
	SetResVersion(m_L, nResVer);
	SetLuaDumpFlag(m_L, LDF_OFF);
	EnableSysLog(true);

	TryExcLuaFileEx(ScriptEntry(nGameId, nResVer));
	return true;
}

bool CLuaScriptMgr::CloseLua()
{
	if(m_L == NULL)
	{
		return false;
	}

	lua_close(m_L);
	m_L = NULL;
	return true;
}

bool CLuaScriptMgr::IsOpen()
{
	return m_L != NULL;
}

int CLuaScriptMgr::GetGameId() const
{
	return ::GetGameId(m_L);
}

int CLuaScriptMgr::GetResVersion() const
{
	return ::GetResVersion(m_L);
}

bool CLuaScriptMgr::EnableSysLog() const 
{
	return IsEnableLuaLog(m_L);
}

void CLuaScriptMgr::EnableSysLog(bool bEnable)
{
	EnableLuaLog(m_L, bEnable);
}

CLuaScriptMgr& CLuaScriptMgr::Trace(const char* pszFilename, int nLineNum, const char* pszFunctionName)
{
	m_pszFilename = pszFilename;
	m_pszFunctionName = pszFunctionName;
	m_nLineNum = nLineNum;

	return *this;
}


const char* CLuaScriptMgr::Compile(const char* pszFilename, int& nSize)
{
	char* pData = NULL;
	nSize = 0;

	int nLen;
	int nGameId = ::GetGameId(m_L);
	int nResVer = ::GetResVersion(m_L);
	const char* pDat = LoadRes(pszFilename, nGameId, nResVer, nLen);
	if(pDat != NULL)
	{
		if(luaL_loadbuffer(m_L, pDat, nLen ,pszFilename) == 0)
		{
			Vector(char) codeDat;
			lua_dump(m_L, luc_writer, &codeDat);

			nSize = codeDat.size();
			pData = (char*) Malloc(nSize);
			memcpy(pData, &codeDat[0], nSize);
		}
	}
	UnloadRes(pDat, pszFilename, nGameId, nResVer, nLen);
	return pData;
}


const char* CLuaScriptMgr::GetLastError() const
{
	return lua_tostring(m_L, -1);
}

bool CLuaScriptMgr::ExcLuaFile(const char* pszFilename)
{
	if(pszFilename == NULL || *pszFilename == 0)
	{
		return false;
	}

    CLuaTopStackRecover topStackRecover(m_L);

	lua_pushcfunction(m_L, lua_onDump);
	int nErrFunc = lua_gettop(m_L);

    int nResult = 0;
	const char* pszErrMsg = NULL;
    
	char szFilename[MSG_BUFF_SIZE];
	snprintf(szFilename, sizeof(szFilename), "%s.luc", pszFilename);
	nResult = loadfile(m_L, szFilename);

	if (nResult == -1)
	{
		snprintf(szFilename, sizeof(szFilename),"%s.lua", pszFilename);
		nResult = loadfile(m_L, szFilename);
	}
    if(nResult == 0)
    {
        nResult = lua_pcall(m_L, 0, 0, nErrFunc);
    }

    if(nResult != 0)
	{
        pszErrMsg = lua_tostring(m_L, -1);
    }
   

	if (pszErrMsg != NULL)
	{
		TraceMsg();
		LogEx(m_L, "load and execute file %s failed!, Reason: %s", pszFilename, pszErrMsg);
	}

    return (nResult == 0);
}

bool CLuaScriptMgr::ExcLuaBuffer(const char* pLuaData, int nDataLen)
{
	if(!pLuaData)	return false;
	if(nDataLen <= 0) nDataLen = strlen(pLuaData);
	if(nDataLen <= 0) return false;

    CLuaTopStackRecover topStackRecover(m_L);

	lua_pushcfunction(m_L, lua_onDump);
	int nErrFunc = lua_gettop(m_L);

    int nResult = 0;
	const char* pszErrMsg = NULL;
   
    nResult = luaL_loadbuffer(m_L, pLuaData, nDataLen, "ExcLuaBuffer");
    if(nResult == 0)
    {
	    nResult = lua_pcall(m_L, 0, 0, nErrFunc);
    }
    if(nResult != 0)
	{
        pszErrMsg = lua_tostring(m_L, -1);
    }
   
	if (pszErrMsg != NULL)
	{
		TraceMsg();
		LogEx(m_L, "execute buffer block failed! Reason: %s!", pszErrMsg);
	}

    return (nResult == 0);
}

//Notice: type pszFmt: "ifbsodrpn>ifbsodrpn"
bool CLuaScriptMgr::ExcLuaFunction(int iSelf, const String& sFunctionName, const char* pszFmt, ...)
{
	CLuaTopStackRecover topStackRecover(m_L);

	lua_pushcfunction(m_L, lua_onDump);
	int nErrFunc = lua_gettop(m_L);

	int nResult = -1;
	const char* pszErrMsg = NULL;

	if (PushLuaFunction(sFunctionName))
	{
		va_list vl;
		va_start(vl, pszFmt);
		
		int nInNum;		// number of arguments
		int nOutNum; 	// number of arguments and results
		// push arguments
		nInNum = 0;
		if (iSelf != LUA_NOREF)
		{
			lua_rawgeti(m_L, LUA_REGISTRYINDEX, iSelf);
			++nInNum;
		}

		int nCurInNum = ParserInputArgs(pszFmt, vl);
		if (nCurInNum >= 0)
		{
			nInNum += nCurInNum;
			pszFmt += nCurInNum;
			if (*pszFmt == '>')
			{
				++pszFmt;
			}
			nOutNum = strlen(pszFmt); 	// number of expected results
			nResult = lua_pcall(m_L, nInNum, nOutNum, nErrFunc);	// do the call
		}
		if (nResult == 0)
		{
			nResult = (ParserOutputArgs(pszFmt, vl) >= 0) ? 0 : -1;
		}
		if(nResult != 0)
		{
			pszErrMsg = lua_tostring(m_L, -1);
		}
		
		va_end(vl);
	}

	if (pszErrMsg != NULL)
	{
		TraceMsg();
		LogEx(m_L, "call %s(...) failed! Reason: %s!", sFunctionName.c_str(), pszErrMsg);
	}

	return (nResult == 0);
}

//Notice: type pszFmt: to Set Property use flag of "iufbsodrpn", while to Get Property use the flag of ">iufbsodrpn"
//		  propNameList: "propName1, propName2,..."
//
//Somethine defferent: ExecLuaProperty: exclude param order is from left to right; while lua exclude param order is from right to left
//
//Example:
//	to set property
//	(1)		ExcLuaProperty(iSelf, "posX, posY, posZ", "fff", 0.3, 0.2, 0.1)
//			exc step: self.posX = 0.3; self.posY = 0.2; self.posZ = 0.1
//	(2)		ExcLuaProperty(iSelf, "posX, posY, posZ", "ff", 0.3, 0.2)
//			exc step: self.posX = 0.3; self.posY = 0.2; self.posZ = nil
//	(3)		ExcLuaProperty(iSelf, "posX, posY", "fff", 0.3, 0.2, 0.1)
//			exc step: self.posX = 0.3; self.posY = 0.2
//
// to get property
//	(1)		ExecLuaProperty(iSelf, "posX, posY, posZ", ">fff", &posX, &posY, &posZ)
//			exec step: posX = self;.posX; posY = self.posY; posZ = self.posZ
bool CLuaScriptMgr::ExcLuaProperty(int iSelf, const String& sPropName, const char* pszFmt, ...)
{
	CLuaTopStackRecover topStackRecover(m_L);

	if (pszFmt == NULL || sPropName.empty() || (iSelf < 0 && iSelf != LUA_GLOBALSINDEX))
	{
		TraceMsg();
		LogEx(m_L, "SysError ExcLuaProperty: input param error!");
		return false;
	}

	enum
	{
		SetPropValue,
		GetPropValue
	} accessPropFlag = SetPropValue;

	va_list vl;
	va_start(vl, pszFmt);

	char curFmt[2] = {*pszFmt, 0};
	if (*pszFmt == '>')
	{
		++pszFmt;
		curFmt[0] = *pszFmt;
		accessPropFlag = GetPropValue;
	}
	curFmt[0] = curFmt[0] ? curFmt[0] : 'n';

	bool bFind = false;
	bool bWaitForComma = false;

	for(String::size_type i = 0, startI = 0, endI = 0, count = sPropName.length(); i <= count; ++i)
	{
		//find each property name

		bool bParseOK = true;
		char cElem = i < count ? sPropName[i] : ',';

		if (isalnum(cElem) || cElem == '_')
		{
			bParseOK = !bWaitForComma;
			endI = i;
			if (!bFind)
			{
				//start match property name
				bFind = true;
				if (isdigit(cElem))
				{
					bParseOK = false;
				}
				startI = i;
			}
		}
		else if (cElem == ' ' || cElem == '\t' || cElem == '\r' || cElem == '\n')
		{
			bWaitForComma = bFind;
		}
		else if (cElem == ',')
		{
			bWaitForComma = false;
			if (!bFind)
			{
				bParseOK = false;
			}
			else
			{
				//end match property name, and exclude it(to Set or to Get).
				bFind = false;
				if (endI + 1 < count && sPropName[endI + 1] == '.')
				{
					bParseOK = false;
				}

				if (bParseOK)
				{
					String sElemLb;
					CLuaTopStackRecover topStackRecover(m_L);

					String::size_type j = sPropName.find_first_of('.', startI);
					if (String::npos == j || j > endI) //global property
					{
						sElemLb = sPropName.substr(startI, endI + 1 - startI);
						if (accessPropFlag == SetPropValue)
						{
							if (iSelf == LUA_GLOBALSINDEX)
							{
								if (ParserInputArgs(curFmt, vl) != 1)
								{
									TraceMsg();
									String sArg = sPropName.substr(0, i + 1);
									LogEx(m_L, "SysError ExcLuaProperty: ParserInputArgs '%s' in '%s'!", sArg.c_str(), sPropName.c_str());
									va_end(vl);
									return false;
								}
								lua_setfield(m_L, LUA_GLOBALSINDEX, sElemLb.c_str());
							}
							else
							{
								lua_rawgeti(m_L, LUA_REGISTRYINDEX, iSelf);
								if (!lua_istable(m_L, -1))
								{
									TraceMsg();
									LogEx(m_L, "SysError ExcLuaProperty: iSelf('%d') is not a table!", iSelf);
									va_end(vl);
									return false;
								}
								if (ParserInputArgs(curFmt, vl) != 1)
								{
									TraceMsg();
									String sArg = sPropName.substr(0, i + 1);
									LogEx(m_L, "SysError ExcLuaProperty: ParserInputArgs '%s' in '%s'!", sArg.c_str(), sPropName.c_str());
									va_end(vl);
									return false;
								}
								lua_setfield(m_L, -1, sElemLb.c_str());
							}
						}
						else //default GetPropValue
						{
							if (*pszFmt)
							{
								if (iSelf == LUA_GLOBALSINDEX)
								{
									lua_getfield(m_L, LUA_GLOBALSINDEX, sElemLb.c_str());
								}
								else
								{
									lua_rawgeti(m_L, LUA_REGISTRYINDEX, iSelf);
									if (!lua_istable(m_L, -1))
									{
										TraceMsg();
										LogEx(m_L, "SysError ExcLuaProperty: iSelf('%d') is not a table!", iSelf);
										va_end(vl);
										return false;
									}
									lua_getfield(m_L, -1, sElemLb.c_str());
								}
								if (ParserOutputArgs(curFmt, vl) != 1)
								{
									TraceMsg();
									String sArg = sPropName.substr(0, i + 1);
									LogEx(m_L, "SysError ExcLuaProperty: ParserOutputArgs '%s' in '%s'!", sArg.c_str(), sPropName.c_str());
									va_end(vl);
									return false;
								}
							}
						}
					}
					else //table property
					{
						//(1) head lb
						sElemLb = sPropName.substr(startI, j - startI);
						String sCurParam = sPropName.substr(startI, endI + 1 - startI);
						if (iSelf == LUA_GLOBALSINDEX)
						{
							lua_getfield(m_L, LUA_GLOBALSINDEX, sElemLb.c_str());
						}
						else
						{
							lua_rawgeti(m_L, LUA_REGISTRYINDEX, iSelf);
							if (!lua_istable(m_L, -1))
							{
								TraceMsg();
								LogEx(m_L, "SysError ExcLuaProperty: iSelf('%d') is not a table!", iSelf);
								va_end(vl);
								return false;
							}
							lua_pushstring(m_L, sElemLb.c_str());
							lua_gettable(m_L, -2);
							lua_remove(m_L, -2);
						}
						if (!lua_istable(m_L, -1))
						{
							TraceMsg();
							LogEx(m_L, "SysError ExcLuaProperty: '%s' is not a table in '%s'!", sElemLb.c_str(), sCurParam.c_str());
							va_end(vl);
							return false;
						}

						//(2) middle lb
						startI = j + 1;
						while (String::npos != (j = sPropName.find_first_of('.', startI)) && j < endI)
						{
							sElemLb = sPropName.substr(startI, j - startI);
							lua_pushstring(m_L, sElemLb.c_str());
							lua_gettable(m_L, -2);
							lua_remove(m_L, -2);
							if (!lua_istable(m_L, -1))
							{
								TraceMsg();
								LogEx(m_L, "SysError ExcLuaProperty: '%s' is not a table in '%s'!", sElemLb.c_str(), sCurParam.c_str());
								va_end(vl);
								return false;
							}
							startI = j + 1;
						}

						//(3) tail lb
						sElemLb = sPropName.substr(startI, endI + 1 - startI);
						if (accessPropFlag == SetPropValue)
						{
							if (ParserInputArgs(curFmt, vl) != 1)
							{
								TraceMsg();
								String sArg = sPropName.substr(0, i + 1);
								LogEx(m_L, "SysError ExcLuaProperty: ParserInputArgs '%s' in '%s'!", sArg.c_str(), sPropName.c_str());
								va_end(vl);
								return false;
							}
							lua_setfield(m_L, -2, sElemLb.c_str());
						}
						else // default GetPropValue
						{
							if(*pszFmt)
							{
								lua_getfield(m_L, -1, sElemLb.c_str());
								if (ParserOutputArgs(curFmt, vl) != 1)
								{
									TraceMsg();
									String sArg = sPropName.substr(0, i + 1);
									LogEx(m_L, "SysError ExcLuaProperty: ParserOutputArgs '%s' in '%s'!", sArg.c_str(), sPropName.c_str());
									va_end(vl);
									return false;
								}
							}
						}
					}

					if(*pszFmt)
					{
						++pszFmt;
						curFmt[0] = *pszFmt ? *pszFmt : 'n';
					}
				}
			}
		}
		else if (cElem != '.' || !bFind || sPropName[i - 1] == '.' || bWaitForComma)
		{
			bParseOK = false;
		}

		if (!bParseOK)
		{
			TraceMsg();
			String sArg = sPropName.substr(0, i + 1);
			LogEx(m_L, "SysError ExcLuaProperty: Parsing '%s' in '%s'!", sArg.c_str(), sPropName.c_str());
			va_end(vl);
			return false;
		}

	}
	va_end(vl);
	return true;
}

int CLuaScriptMgr::ParserInputArgs(const char *pszFmt, va_list& vl)
{
	int nInNum = 0;
	while (*pszFmt)
	{
		switch (*pszFmt++)
		{
		case 'i': /* int argument */
			{
				lua_pushnumber(m_L, va_arg(vl, int));
				break;
			}
		case 'u': /* uint argument */
			{
				lua_pushnumber(m_L, va_arg(vl, unsigned int));
				break;
			}
		case 'f': /* float argument */
			{
				lua_pushnumber(m_L, va_arg(vl, double));
				break;
			}
		case 'b': /* bool argument */
			{
			    /*
			    	gcc version 4.6.2(SUSE Linux)
			    	waring: 'bool' is promoted to 'int' when passed through '...'
			    	[enabled by default],so you should pass 'int' not 'bool' to 'va_arg'
			    */
				lua_pushboolean(m_L, va_arg(vl, int));
				break;
			}
		case 's': /* string argument */
			{
				lua_pushstring(m_L, va_arg(vl, char*));
				break;
			}
		case 'o': /* CScriptObject argument */
			{
				CScriptObject* pObj = (CScriptObject*) va_arg(vl, void*);
				tolua_pushusertype(m_L, pObj->GetThis(), pObj->ClassName());
				break;
			}
		case 'd': /* double argument */
			{
				lua_pushnumber(m_L, va_arg(vl, double));
				break;
			}
		case 'r': /* LUA_REGISTRYINDEX value */
			{
				lua_rawgeti(m_L, LUA_REGISTRYINDEX, va_arg(vl, int));
				break;
			}
		case 'p': /* light user data argument */
			{
				lua_pushlightuserdata(m_L, va_arg(vl, void*));
				break;
			}
		case 'n': /* nil argument */
			{
				lua_pushnil(m_L);
				break;
			}
		case 't': //tolua_pushusertype: use tow input arg
			{
				void* pObj = (void*) va_arg(vl, void*);
				const char* pszName = va_arg(vl, const char*);
				tolua_pushusertype(m_L, pObj, pszName);
				break;
			}
		case '>': /* separate from input and output */
			{
				return nInNum;
			}
		default:
			{
				LogEx(m_L, "invalid option (%c)", *(pszFmt - 1));
				return -1;
			}
		}
		++nInNum;
	}
	return nInNum;
}

int CLuaScriptMgr::ParserOutputArgs(const char* pszFmt, va_list& vl)
{
	bool bParserArgOK = true;
	int nOutNum = strlen(pszFmt);
	int nResult = nOutNum;
	nOutNum = -nOutNum; //inverse,from the top stack of lua
	/* retrieve results */
	/* stack index of first result */
	while (*pszFmt)
	{
		switch (*pszFmt++)
		{
		case 'i': /* int result */
			{
			    /*
                    add doubl parenthesis to avoid the warning of: suggest
                    parentheses around assignment used as truth value
                */
				if ((bParserArgOK = !!lua_isnumber(m_L, nOutNum)))
				{
					*va_arg(vl, int*) = (int)lua_tonumber(m_L, nOutNum);
				}
				break;
			}
		case 'u':
			{
				if ((bParserArgOK = !!lua_isnumber(m_L, nOutNum)))
				{
					*va_arg(vl, unsigned int*) = (unsigned int)lua_tonumber(m_L, nOutNum);
				}
				break;
			}
		case 'f': /* float result */
			{
				if ((bParserArgOK = !!lua_isnumber(m_L, nOutNum)))
				{
					*va_arg(vl, float*) = (float)lua_tonumber(m_L, nOutNum);
				}
				break;
			}
		case 'b': /* bool result */
			{
				if ((bParserArgOK = !!lua_isboolean(m_L, nOutNum)))
				{
					*va_arg(vl, bool*) = !!lua_toboolean(m_L, nOutNum);
				}
				break;
			}
		case 's': /* string result */
			{
				if ((bParserArgOK = !!lua_isstring(m_L, nOutNum)))
				{
					*va_arg(vl, const char**) = lua_tostring(m_L, nOutNum);
				}
				break;
			}
		case 'o': /* CScriptObject result */
			{
				tolua_Error tolua_err;
				if ((bParserArgOK = !!tolua_isusertype(m_L, nOutNum, "CScriptObject", 0, &tolua_err)))
				{
					*va_arg(vl, void**) = tolua_tousertype(m_L, nOutNum, NULL);
				}
				break;
			}
		case 'd': /* double result */
			{
				if ((bParserArgOK = !!lua_isnumber(m_L, nOutNum)))
				{
					*va_arg(vl, double*) = lua_tonumber(m_L, nOutNum);
				}
				break;
			}
		case 'r': /* LUA_REGISTRYINDEX value */
			{
				lua_pushvalue(m_L, nOutNum);
				*va_arg(vl, int*) = luaL_ref(m_L, LUA_REGISTRYINDEX);
				break;
			}
		case 'p': /* light user data result */
			{
				if ((bParserArgOK = !!lua_islightuserdata(m_L, nOutNum)))
				{
					*va_arg(vl, const void**) = lua_topointer(m_L, nOutNum);
				}
				break;
			}
		case 'n': /* nil result */
			{
				if ((bParserArgOK = !!lua_isnil(m_L, nOutNum)))
				{
					*va_arg(vl, const void**) = 0;
				}
				break;
			}
		default:
			{
				LogEx(m_L, "invalid option (%c)", *(pszFmt - 1));
				return -1;
			}
		}

		if (!bParserArgOK)
		{
			int tp = lua_type(m_L, nOutNum);
			const char* type = lua_typename(m_L, tp);
			LogEx(m_L, "wrong result type (%c), lua_type(%s)", *(pszFmt - 1), type);
			return -1;
		}

		++nOutNum;
	}
	return nResult;
}

//format for sLbName: global.*(table).lbName
//Notice:				*(table), none or more tables
bool CLuaScriptMgr::PushLuaFunction(const String& sLbName)
{
	CLuaTopStackRecover topStackRecover(m_L);

	if (sLbName.empty())
	{
		LogEx(m_L, "%s!", LuaScriptMgrMsg[LEC_EMPTY_LABEL_NAME]);
		return false;
	}

	// do we have any dots in the handler name? if so we grab the function as a table field
	String::size_type splitIndex = sLbName.find_first_of('.');
	if (String::npos == splitIndex) //global function
	{
		lua_getglobal(m_L, sLbName.c_str());
	}
	else //member function
	{
		String::size_type start = 0;
		String sElemLb = sLbName.substr(start, splitIndex - start);
		lua_getglobal(m_L, sElemLb.c_str());
		if (!lua_istable(m_L, -1))
		{
			LogEx(m_L, "Unable to get table '%s' in '%s'!", sElemLb.c_str(), sLbName.c_str());
			return false;
		}

		start = splitIndex + 1;

		while(String::npos != (splitIndex = sLbName.find_first_of('.', start)))
		{
			sElemLb = sLbName.substr(start, splitIndex - start);
			lua_pushstring(m_L, sElemLb.c_str());
			lua_gettable(m_L, -2);
			lua_remove(m_L, -2);

			if (!lua_istable(m_L, -1))
			{
				LogEx(m_L, "Unable to get table '%s' in '%s'!", sElemLb.c_str(), sLbName.c_str());
				return false;
			}

			start = splitIndex + 1;
		}

		splitIndex = sLbName.length();
		sElemLb = sLbName.substr(start, splitIndex - start);
		lua_pushstring(m_L, sElemLb.c_str());
		lua_gettable(m_L, -2);
		lua_remove(m_L, -2);
	}

	if (!lua_isfunction(m_L, -1))
	{
		LogEx(m_L, "%s!", LuaScriptMgrMsg[LEC_NO_FUNCTION]);
		return false;
	}

	topStackRecover.disableRecover();

	return true;
}

void CLuaScriptMgr::TraceMsg()
{
	if (m_pszFunctionName != UNKNOW_FUNCTION /*&& m_pszFunctionName != NULL
		&& m_pszFilename != UNKNOW_FILE_NAME && m_pszFilename != NULL*/)
	{
		LogEx(m_L, "\n%s(%u): %s", m_pszFilename, m_nLineNum, m_pszFunctionName);
	}
}
