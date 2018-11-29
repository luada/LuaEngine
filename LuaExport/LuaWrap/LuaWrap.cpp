/*
** Lua Wrap: LuaWrap
** Generated automatically by AutoWrapLua.exe
*/

#include "pch.h"
#include "AutoRegToLua.h"
#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_LuaWrap_open (lua_State* tolua_S);

#include "ScriptObject.h"
#include "Type.h"
#include "Macro.h"

using namespace LuaWrap;

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"LuaWrap::CScriptObject");
}

/* method: IsScriptObjValid of class  LuaWrap::CScriptObject */
#ifndef TOLUA_DISABLE_tolua_LuaWrap_LuaWrap_CScriptObject_IsScriptObjValid00
static int tolua_LuaWrap_LuaWrap_CScriptObject_IsScriptObjValid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const LuaWrap::CScriptObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const LuaWrap::CScriptObject* self = (const LuaWrap::CScriptObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsScriptObjValid'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsScriptObjValid();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsScriptObjValid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetScriptObjID of class  LuaWrap::CScriptObject */
#ifndef TOLUA_DISABLE_tolua_LuaWrap_LuaWrap_CScriptObject_GetScriptObjID00
static int tolua_LuaWrap_LuaWrap_CScriptObject_GetScriptObjID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const LuaWrap::CScriptObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const LuaWrap::CScriptObject* self = (const LuaWrap::CScriptObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetScriptObjID'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetScriptObjID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScriptObjID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetScriptObjID of class  LuaWrap::CScriptObject */
#ifndef TOLUA_DISABLE_tolua_LuaWrap_LuaWrap_CScriptObject_SetScriptObjID00
static int tolua_LuaWrap_LuaWrap_CScriptObject_SetScriptObjID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LuaWrap::CScriptObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LuaWrap::CScriptObject* self = (LuaWrap::CScriptObject*)  tolua_tousertype(tolua_S,1,0);
  int nObjID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetScriptObjID'", NULL);
#endif
  {
   self->SetScriptObjID(nObjID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetScriptObjID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CreateScriptObj of class  LuaWrap::CScriptObject */
#ifndef TOLUA_DISABLE_tolua_LuaWrap_LuaWrap_CScriptObject_CreateScriptObj00
static int tolua_LuaWrap_LuaWrap_CScriptObject_CreateScriptObj00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LuaWrap::CScriptObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LuaWrap::CScriptObject* self = (LuaWrap::CScriptObject*)  tolua_tousertype(tolua_S,1,0);
  const String sScriptPath = ((const String)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CreateScriptObj'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->CreateScriptObj(sScriptPath);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)sScriptPath);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateScriptObj'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetScriptClass of class  LuaWrap::CScriptObject */
#ifndef TOLUA_DISABLE_tolua_LuaWrap_LuaWrap_CScriptObject_SetScriptClass00
static int tolua_LuaWrap_LuaWrap_CScriptObject_SetScriptClass00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LuaWrap::CScriptObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LuaWrap::CScriptObject* self = (LuaWrap::CScriptObject*)  tolua_tousertype(tolua_S,1,0);
  const String scriptClass = ((const String)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetScriptClass'", NULL);
#endif
  {
   self->SetScriptClass(scriptClass);
   tolua_pushcppstring(tolua_S,(const char*)scriptClass);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetScriptClass'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetScriptClass of class  LuaWrap::CScriptObject */
#ifndef TOLUA_DISABLE_tolua_LuaWrap_LuaWrap_CScriptObject_GetScriptClass00
static int tolua_LuaWrap_LuaWrap_CScriptObject_GetScriptClass00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const LuaWrap::CScriptObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const LuaWrap::CScriptObject* self = (const LuaWrap::CScriptObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetScriptClass'", NULL);
#endif
  {
    const String tolua_ret = (  const String)  self->GetScriptClass();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScriptClass'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_LuaWrap_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"LuaWrap",0);
  tolua_beginmodule(tolua_S,"LuaWrap");
   tolua_cclass(tolua_S,"CScriptObject","LuaWrap::CScriptObject","",NULL);
   tolua_beginmodule(tolua_S,"CScriptObject");
    tolua_function(tolua_S,"IsScriptObjValid",tolua_LuaWrap_LuaWrap_CScriptObject_IsScriptObjValid00);
    tolua_function(tolua_S,"GetScriptObjID",tolua_LuaWrap_LuaWrap_CScriptObject_GetScriptObjID00);
    tolua_function(tolua_S,"SetScriptObjID",tolua_LuaWrap_LuaWrap_CScriptObject_SetScriptObjID00);
    tolua_function(tolua_S,"CreateScriptObj",tolua_LuaWrap_LuaWrap_CScriptObject_CreateScriptObj00);
    tolua_function(tolua_S,"SetScriptClass",tolua_LuaWrap_LuaWrap_CScriptObject_SetScriptClass00);
    tolua_function(tolua_S,"GetScriptClass",tolua_LuaWrap_LuaWrap_CScriptObject_GetScriptClass00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


static CAutoRegToLua s_autoRegToLua(tolua_LuaWrap_open, "LuaWrap");

FORCE_LINK_DECL(LuaWrap);
