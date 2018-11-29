/*
** Lua Wrap: Game
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
TOLUA_API int  tolua_Game_open (lua_State* tolua_S);

#include "const.h"
#include "dataReader.h"
#include "server.h"
#include "dataWriter.h"
#include "game.h"
#include "type.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Demo__CDataWriter (lua_State* tolua_S)
{
 Demo::CDataWriter* self = (Demo::CDataWriter*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(CDataWriter, self);
	return 0;
}

static int tolua_collect_Demo__CDataReader (lua_State* tolua_S)
{
 Demo::CDataReader* self = (Demo::CDataReader*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(CDataReader, self);
	return 0;
}
#endif

using namespace LuaWrap;


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Demo::CDataWriter");
 tolua_usertype(tolua_S,"Demo::CDataReader");
 tolua_usertype(tolua_S,"Demo::CGame");
 tolua_usertype(tolua_S,"LuaWrap::CScriptObject");
}

/* method: new of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_new00
static int tolua_Game_Demo_CDataReader_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const void* pData = ((const void*)  tolua_touserdata(tolua_S,2,0));
  int nLen = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   Demo::CDataReader* tolua_ret = (Demo::CDataReader*)  Mtolua_new(Demo::CDataReader, (Demo::CDataReader)(pData,nLen));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demo::CDataReader");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_new00_local
static int tolua_Game_Demo_CDataReader_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const void* pData = ((const void*)  tolua_touserdata(tolua_S,2,0));
  int nLen = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   Demo::CDataReader* tolua_ret = (Demo::CDataReader*)  Mtolua_new(Demo::CDataReader, (Demo::CDataReader)(pData,nLen));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demo::CDataReader");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_delete00
static int tolua_Game_Demo_CDataReader_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(CDataReader, self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AttachData of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_AttachData00
static int tolua_Game_Demo_CDataReader_AttachData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  const void* pData = ((const void*)  tolua_touserdata(tolua_S,2,0));
  int nLen = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AttachData'", NULL);
#endif
  {
   self->AttachData(pData,nLen);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AttachData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Data of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_Data00
static int tolua_Game_Demo_CDataReader_Data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demo::CDataReader* self = (const Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Data'", NULL);
#endif
  {
   const void* tolua_ret = (const void*)  self->Data();
   tolua_pushuserdata(tolua_S,(void*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Data'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CurData of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_CurData00
static int tolua_Game_Demo_CDataReader_CurData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demo::CDataReader* self = (const Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CurData'", NULL);
#endif
  {
   const void* tolua_ret = (const void*)  self->CurData();
   tolua_pushuserdata(tolua_S,(void*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CurData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DataLen of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_DataLen00
static int tolua_Game_Demo_CDataReader_DataLen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demo::CDataReader* self = (const Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DataLen'", NULL);
#endif
  {
   int tolua_ret = (int)  self->DataLen();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DataLen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LeftLen of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_LeftLen00
static int tolua_Game_Demo_CDataReader_LeftLen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demo::CDataReader* self = (const Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LeftLen'", NULL);
#endif
  {
   int tolua_ret = (int)  self->LeftLen();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LeftLen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_Clear00
static int tolua_Game_Demo_CDataReader_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'", NULL);
#endif
  {
   self->Clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCur of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_GetCur00
static int tolua_Game_Demo_CDataReader_GetCur00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demo::CDataReader* self = (const Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCur'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetCur();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCur'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCur of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_SetCur00
static int tolua_Game_Demo_CDataReader_SetCur00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  int nCur = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCur'", NULL);
#endif
  {
   self->SetCur(nCur);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCur'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Skip of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_Skip00
static int tolua_Game_Demo_CDataReader_Skip00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  int nLen = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Skip'", NULL);
#endif
  {
   self->Skip(nLen);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Skip'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadUInt of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadUInt00
static int tolua_Game_Demo_CDataReader_ReadUInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  UINT uDefault = ((UINT)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadUInt'", NULL);
#endif
  {
   UINT tolua_ret = (UINT)  self->ReadUInt(uDefault);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadUInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadInt of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadInt00
static int tolua_Game_Demo_CDataReader_ReadInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  int nDefault = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadInt'", NULL);
#endif
  {
   int tolua_ret = (int)  self->ReadInt(nDefault);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadUShort of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadUShort00
static int tolua_Game_Demo_CDataReader_ReadUShort00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  USHORT usDefault = ((USHORT)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadUShort'", NULL);
#endif
  {
   USHORT tolua_ret = (USHORT)  self->ReadUShort(usDefault);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadUShort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadShort of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadShort00
static int tolua_Game_Demo_CDataReader_ReadShort00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  short sDefault = ((short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadShort'", NULL);
#endif
  {
   short tolua_ret = (short)  self->ReadShort(sDefault);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadShort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadFloat of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadFloat00
static int tolua_Game_Demo_CDataReader_ReadFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  float fDefault = ((float)  tolua_tonumber(tolua_S,2,0.f));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadFloat'", NULL);
#endif
  {
   float tolua_ret = (float)  self->ReadFloat(fDefault);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadDouble of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadDouble00
static int tolua_Game_Demo_CDataReader_ReadDouble00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  double dDefault = ((double)  tolua_tonumber(tolua_S,2,0.0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadDouble'", NULL);
#endif
  {
   double tolua_ret = (double)  self->ReadDouble(dDefault);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadDouble'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadBool of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadBool00
static int tolua_Game_Demo_CDataReader_ReadBool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  bool bDefault = ((bool)  tolua_toboolean(tolua_S,2,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadBool'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ReadBool(bDefault);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadBool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadChar of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadChar00
static int tolua_Game_Demo_CDataReader_ReadChar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  char cDefault = ((char)  tolua_tonumber(tolua_S,2,'\0'));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadChar'", NULL);
#endif
  {
   char tolua_ret = (char)  self->ReadChar(cDefault);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadChar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadByte of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadByte00
static int tolua_Game_Demo_CDataReader_ReadByte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  BYTE btDefault = ((BYTE)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadByte'", NULL);
#endif
  {
   BYTE tolua_ret = (BYTE)  self->ReadByte(btDefault);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadByte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadINT8 of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadINT800
static int tolua_Game_Demo_CDataReader_ReadINT800(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  unsigned char i8Default = (( unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadINT8'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->ReadINT8(i8Default);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadINT8'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadUINT8 of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadUINT800
static int tolua_Game_Demo_CDataReader_ReadUINT800(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  UINT8 ui8Default = ((UINT8)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadUINT8'", NULL);
#endif
  {
   UINT8 tolua_ret = (UINT8)  self->ReadUINT8(ui8Default);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadUINT8'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadINT16 of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadINT1600
static int tolua_Game_Demo_CDataReader_ReadINT1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  INT16 i16Default = ((INT16)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadINT16'", NULL);
#endif
  {
   INT16 tolua_ret = (INT16)  self->ReadINT16(i16Default);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadINT16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadUINT16 of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadUINT1600
static int tolua_Game_Demo_CDataReader_ReadUINT1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  UINT16 ui16Default = ((UINT16)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadUINT16'", NULL);
#endif
  {
   UINT16 tolua_ret = (UINT16)  self->ReadUINT16(ui16Default);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadUINT16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadINT32 of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadINT3200
static int tolua_Game_Demo_CDataReader_ReadINT3200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  INT32 i32Default = ((INT32)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadINT32'", NULL);
#endif
  {
   INT32 tolua_ret = (INT32)  self->ReadINT32(i32Default);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadINT32'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadUINT32 of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadUINT3200
static int tolua_Game_Demo_CDataReader_ReadUINT3200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  UINT32 ui32Default = ((UINT32)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadUINT32'", NULL);
#endif
  {
   UINT32 tolua_ret = (UINT32)  self->ReadUINT32(ui32Default);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadUINT32'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadPointer of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadPointer00
static int tolua_Game_Demo_CDataReader_ReadPointer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  void* pDefault = ((void*)  tolua_touserdata(tolua_S,2,NULL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadPointer'", NULL);
#endif
  {
   void* tolua_ret = (void*)  self->ReadPointer(pDefault);
   tolua_pushuserdata(tolua_S,(void*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadPointer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadStrBuffer of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadStrBuffer00
static int tolua_Game_Demo_CDataReader_ReadStrBuffer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  int nBuffLen = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* pszDefault = ((const char*)  tolua_tostring(tolua_S,3,""));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadStrBuffer'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->ReadStrBuffer(nBuffLen,pszDefault);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadStrBuffer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadString of class  Demo::CDataReader */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataReader_ReadString00
static int tolua_Game_Demo_CDataReader_ReadString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataReader",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataReader* self = (Demo::CDataReader*)  tolua_tousertype(tolua_S,1,0);
  const char* pszDefault = ((const char*)  tolua_tostring(tolua_S,2,""));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadString'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->ReadString(pszDefault);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_new00
static int tolua_Game_Demo_CDataWriter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int nSize = ((int)  tolua_tonumber(tolua_S,2,Demo::MAX_BUFF_SIZE));
  {
   Demo::CDataWriter* tolua_ret = (Demo::CDataWriter*)  Mtolua_new(Demo::CDataWriter, (Demo::CDataWriter)(nSize));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demo::CDataWriter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_new00_local
static int tolua_Game_Demo_CDataWriter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int nSize = ((int)  tolua_tonumber(tolua_S,2,Demo::MAX_BUFF_SIZE));
  {
   Demo::CDataWriter* tolua_ret = (Demo::CDataWriter*)  Mtolua_new(Demo::CDataWriter, (Demo::CDataWriter)(nSize));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demo::CDataWriter");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_new01
static int tolua_Game_Demo_CDataWriter_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  void* pBuff = ((void*)  tolua_touserdata(tolua_S,2,0));
  int nSize = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   Demo::CDataWriter* tolua_ret = (Demo::CDataWriter*)  Mtolua_new(Demo::CDataWriter, (Demo::CDataWriter)(pBuff,nSize));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demo::CDataWriter");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Game_Demo_CDataWriter_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_new01_local
static int tolua_Game_Demo_CDataWriter_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  void* pBuff = ((void*)  tolua_touserdata(tolua_S,2,0));
  int nSize = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   Demo::CDataWriter* tolua_ret = (Demo::CDataWriter*)  Mtolua_new(Demo::CDataWriter, (Demo::CDataWriter)(pBuff,nSize));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demo::CDataWriter");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_Game_Demo_CDataWriter_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_delete00
static int tolua_Game_Demo_CDataWriter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(CDataWriter, self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Data of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_Data00
static int tolua_Game_Demo_CDataWriter_Data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demo::CDataWriter* self = (const Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Data'", NULL);
#endif
  {
   void* tolua_ret = (void*)  self->Data();
   tolua_pushuserdata(tolua_S,(void*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Data'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DataLen of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_DataLen00
static int tolua_Game_Demo_CDataWriter_DataLen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DataLen'", NULL);
#endif
  {
   int tolua_ret = (int)  self->DataLen();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DataLen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: BufferLen of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_BufferLen00
static int tolua_Game_Demo_CDataWriter_BufferLen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'BufferLen'", NULL);
#endif
  {
   int tolua_ret = (int)  self->BufferLen();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'BufferLen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDataLen of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_SetDataLen00
static int tolua_Game_Demo_CDataWriter_SetDataLen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  int nLen = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDataLen'", NULL);
#endif
  {
   self->SetDataLen(nLen);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDataLen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Resize of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_Resize00
static int tolua_Game_Demo_CDataWriter_Resize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Resize'", NULL);
#endif
  {
   self->Resize();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Resize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_Clear00
static int tolua_Game_Demo_CDataWriter_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'", NULL);
#endif
  {
   self->Clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddBlob of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_AddBlob00
static int tolua_Game_Demo_CDataWriter_AddBlob00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  const void* pcData = ((const void*)  tolua_touserdata(tolua_S,2,0));
  int nDataLen = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddBlob'", NULL);
#endif
  {
   self->AddBlob(pcData,nDataLen);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddBlob'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteUInt of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteUInt00
static int tolua_Game_Demo_CDataWriter_WriteUInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  UINT uValue = ((UINT)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteUInt'", NULL);
#endif
  {
   self->WriteUInt(uValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteUInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteInt of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteInt00
static int tolua_Game_Demo_CDataWriter_WriteInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  int nValue = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteInt'", NULL);
#endif
  {
   self->WriteInt(nValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteUShort of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteUShort00
static int tolua_Game_Demo_CDataWriter_WriteUShort00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  USHORT usValue = ((USHORT)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteUShort'", NULL);
#endif
  {
   self->WriteUShort(usValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteUShort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteShort of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteShort00
static int tolua_Game_Demo_CDataWriter_WriteShort00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  short sValue = ((short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteShort'", NULL);
#endif
  {
   self->WriteShort(sValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteShort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteFloat of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteFloat00
static int tolua_Game_Demo_CDataWriter_WriteFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteFloat'", NULL);
#endif
  {
   self->WriteFloat(fValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteDouble of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteDouble00
static int tolua_Game_Demo_CDataWriter_WriteDouble00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  double dValue = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteDouble'", NULL);
#endif
  {
   self->WriteDouble(dValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteDouble'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteBool of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteBool00
static int tolua_Game_Demo_CDataWriter_WriteBool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  bool bValue = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteBool'", NULL);
#endif
  {
   self->WriteBool(bValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteBool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteChar of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteChar00
static int tolua_Game_Demo_CDataWriter_WriteChar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  char cValue = ((char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteChar'", NULL);
#endif
  {
   self->WriteChar(cValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteChar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteByte of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteByte00
static int tolua_Game_Demo_CDataWriter_WriteByte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  BYTE btValue = ((BYTE)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteByte'", NULL);
#endif
  {
   self->WriteByte(btValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteByte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteINT8 of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteINT800
static int tolua_Game_Demo_CDataWriter_WriteINT800(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  unsigned char i8Value = (( unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteINT8'", NULL);
#endif
  {
   self->WriteINT8(i8Value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteINT8'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteUINT8 of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteUINT800
static int tolua_Game_Demo_CDataWriter_WriteUINT800(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  UINT8 ui8Value = ((UINT8)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteUINT8'", NULL);
#endif
  {
   self->WriteUINT8(ui8Value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteUINT8'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteINT16 of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteINT1600
static int tolua_Game_Demo_CDataWriter_WriteINT1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  INT16 i16Value = ((INT16)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteINT16'", NULL);
#endif
  {
   self->WriteINT16(i16Value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteINT16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteUINT16 of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteUINT1600
static int tolua_Game_Demo_CDataWriter_WriteUINT1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  UINT16 ui16Value = ((UINT16)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteUINT16'", NULL);
#endif
  {
   self->WriteUINT16(ui16Value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteUINT16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteINT32 of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteINT3200
static int tolua_Game_Demo_CDataWriter_WriteINT3200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  INT32 i32Value = ((INT32)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteINT32'", NULL);
#endif
  {
   self->WriteINT32(i32Value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteINT32'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteUINT32 of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteUINT3200
static int tolua_Game_Demo_CDataWriter_WriteUINT3200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  UINT32 ui32Value = ((UINT32)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteUINT32'", NULL);
#endif
  {
   self->WriteUINT32(ui32Value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteUINT32'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WritePointer of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WritePointer00
static int tolua_Game_Demo_CDataWriter_WritePointer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  void* pObj = ((void*)  tolua_touserdata(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WritePointer'", NULL);
#endif
  {
   self->WritePointer(pObj);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WritePointer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteStrBuffer of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteStrBuffer00
static int tolua_Game_Demo_CDataWriter_WriteStrBuffer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  const char* pszValue = ((const char*)  tolua_tostring(tolua_S,2,0));
  int nBuffLen = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteStrBuffer'", NULL);
#endif
  {
   self->WriteStrBuffer(pszValue,nBuffLen);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteStrBuffer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteString of class  Demo::CDataWriter */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CDataWriter_WriteString00
static int tolua_Game_Demo_CDataWriter_WriteString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CDataWriter",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CDataWriter* self = (Demo::CDataWriter*)  tolua_tousertype(tolua_S,1,0);
  const char* pszValue = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteString'", NULL);
#endif
  {
   self->WriteString(pszValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadDbgInput of class  Demo::CGame */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CGame_ReadDbgInput00
static int tolua_Game_Demo_CGame_ReadDbgInput00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CGame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CGame* self = (Demo::CGame*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadDbgInput'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->ReadDbgInput();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadDbgInput'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteDbgOutput of class  Demo::CGame */
#ifndef TOLUA_DISABLE_tolua_Game_Demo_CGame_WriteDbgOutput00
static int tolua_Game_Demo_CGame_WriteDbgOutput00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demo::CGame",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demo::CGame* self = (Demo::CGame*)  tolua_tousertype(tolua_S,1,0);
  const char* info = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteDbgOutput'", NULL);
#endif
  {
   self->WriteDbgOutput(info);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteDbgOutput'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Game_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"Demo",0);
  tolua_beginmodule(tolua_S,"Demo");
   tolua_constant(tolua_S,"MAX_BUFF_SIZE",Demo::MAX_BUFF_SIZE);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Demo",0);
  tolua_beginmodule(tolua_S,"Demo");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"CDataReader","Demo::CDataReader","",tolua_collect_Demo__CDataReader);
   #else
   tolua_cclass(tolua_S,"CDataReader","Demo::CDataReader","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"CDataReader");
    tolua_function(tolua_S,"new",tolua_Game_Demo_CDataReader_new00);
    tolua_function(tolua_S,"new_local",tolua_Game_Demo_CDataReader_new00_local);
    tolua_function(tolua_S,".call",tolua_Game_Demo_CDataReader_new00_local);
    tolua_function(tolua_S,"delete",tolua_Game_Demo_CDataReader_delete00);
    tolua_function(tolua_S,"AttachData",tolua_Game_Demo_CDataReader_AttachData00);
    tolua_function(tolua_S,"Data",tolua_Game_Demo_CDataReader_Data00);
    tolua_function(tolua_S,"CurData",tolua_Game_Demo_CDataReader_CurData00);
    tolua_function(tolua_S,"DataLen",tolua_Game_Demo_CDataReader_DataLen00);
    tolua_function(tolua_S,"LeftLen",tolua_Game_Demo_CDataReader_LeftLen00);
    tolua_function(tolua_S,"Clear",tolua_Game_Demo_CDataReader_Clear00);
    tolua_function(tolua_S,"GetCur",tolua_Game_Demo_CDataReader_GetCur00);
    tolua_function(tolua_S,"SetCur",tolua_Game_Demo_CDataReader_SetCur00);
    tolua_function(tolua_S,"Skip",tolua_Game_Demo_CDataReader_Skip00);
    tolua_function(tolua_S,"ReadUInt",tolua_Game_Demo_CDataReader_ReadUInt00);
    tolua_function(tolua_S,"ReadInt",tolua_Game_Demo_CDataReader_ReadInt00);
    tolua_function(tolua_S,"ReadUShort",tolua_Game_Demo_CDataReader_ReadUShort00);
    tolua_function(tolua_S,"ReadShort",tolua_Game_Demo_CDataReader_ReadShort00);
    tolua_function(tolua_S,"ReadFloat",tolua_Game_Demo_CDataReader_ReadFloat00);
    tolua_function(tolua_S,"ReadDouble",tolua_Game_Demo_CDataReader_ReadDouble00);
    tolua_function(tolua_S,"ReadBool",tolua_Game_Demo_CDataReader_ReadBool00);
    tolua_function(tolua_S,"ReadChar",tolua_Game_Demo_CDataReader_ReadChar00);
    tolua_function(tolua_S,"ReadByte",tolua_Game_Demo_CDataReader_ReadByte00);
    tolua_function(tolua_S,"ReadINT8",tolua_Game_Demo_CDataReader_ReadINT800);
    tolua_function(tolua_S,"ReadUINT8",tolua_Game_Demo_CDataReader_ReadUINT800);
    tolua_function(tolua_S,"ReadINT16",tolua_Game_Demo_CDataReader_ReadINT1600);
    tolua_function(tolua_S,"ReadUINT16",tolua_Game_Demo_CDataReader_ReadUINT1600);
    tolua_function(tolua_S,"ReadINT32",tolua_Game_Demo_CDataReader_ReadINT3200);
    tolua_function(tolua_S,"ReadUINT32",tolua_Game_Demo_CDataReader_ReadUINT3200);
    tolua_function(tolua_S,"ReadPointer",tolua_Game_Demo_CDataReader_ReadPointer00);
    tolua_function(tolua_S,"ReadStrBuffer",tolua_Game_Demo_CDataReader_ReadStrBuffer00);
    tolua_function(tolua_S,"ReadString",tolua_Game_Demo_CDataReader_ReadString00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Demo",0);
  tolua_beginmodule(tolua_S,"Demo");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"CDataWriter","Demo::CDataWriter","",tolua_collect_Demo__CDataWriter);
   #else
   tolua_cclass(tolua_S,"CDataWriter","Demo::CDataWriter","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"CDataWriter");
    tolua_function(tolua_S,"new",tolua_Game_Demo_CDataWriter_new00);
    tolua_function(tolua_S,"new_local",tolua_Game_Demo_CDataWriter_new00_local);
    tolua_function(tolua_S,".call",tolua_Game_Demo_CDataWriter_new00_local);
    tolua_function(tolua_S,"new",tolua_Game_Demo_CDataWriter_new01);
    tolua_function(tolua_S,"new_local",tolua_Game_Demo_CDataWriter_new01_local);
    tolua_function(tolua_S,".call",tolua_Game_Demo_CDataWriter_new01_local);
    tolua_function(tolua_S,"delete",tolua_Game_Demo_CDataWriter_delete00);
    tolua_function(tolua_S,"Data",tolua_Game_Demo_CDataWriter_Data00);
    tolua_function(tolua_S,"DataLen",tolua_Game_Demo_CDataWriter_DataLen00);
    tolua_function(tolua_S,"BufferLen",tolua_Game_Demo_CDataWriter_BufferLen00);
    tolua_function(tolua_S,"SetDataLen",tolua_Game_Demo_CDataWriter_SetDataLen00);
    tolua_function(tolua_S,"Resize",tolua_Game_Demo_CDataWriter_Resize00);
    tolua_function(tolua_S,"Clear",tolua_Game_Demo_CDataWriter_Clear00);
    tolua_function(tolua_S,"AddBlob",tolua_Game_Demo_CDataWriter_AddBlob00);
    tolua_function(tolua_S,"WriteUInt",tolua_Game_Demo_CDataWriter_WriteUInt00);
    tolua_function(tolua_S,"WriteInt",tolua_Game_Demo_CDataWriter_WriteInt00);
    tolua_function(tolua_S,"WriteUShort",tolua_Game_Demo_CDataWriter_WriteUShort00);
    tolua_function(tolua_S,"WriteShort",tolua_Game_Demo_CDataWriter_WriteShort00);
    tolua_function(tolua_S,"WriteFloat",tolua_Game_Demo_CDataWriter_WriteFloat00);
    tolua_function(tolua_S,"WriteDouble",tolua_Game_Demo_CDataWriter_WriteDouble00);
    tolua_function(tolua_S,"WriteBool",tolua_Game_Demo_CDataWriter_WriteBool00);
    tolua_function(tolua_S,"WriteChar",tolua_Game_Demo_CDataWriter_WriteChar00);
    tolua_function(tolua_S,"WriteByte",tolua_Game_Demo_CDataWriter_WriteByte00);
    tolua_function(tolua_S,"WriteINT8",tolua_Game_Demo_CDataWriter_WriteINT800);
    tolua_function(tolua_S,"WriteUINT8",tolua_Game_Demo_CDataWriter_WriteUINT800);
    tolua_function(tolua_S,"WriteINT16",tolua_Game_Demo_CDataWriter_WriteINT1600);
    tolua_function(tolua_S,"WriteUINT16",tolua_Game_Demo_CDataWriter_WriteUINT1600);
    tolua_function(tolua_S,"WriteINT32",tolua_Game_Demo_CDataWriter_WriteINT3200);
    tolua_function(tolua_S,"WriteUINT32",tolua_Game_Demo_CDataWriter_WriteUINT3200);
    tolua_function(tolua_S,"WritePointer",tolua_Game_Demo_CDataWriter_WritePointer00);
    tolua_function(tolua_S,"WriteStrBuffer",tolua_Game_Demo_CDataWriter_WriteStrBuffer00);
    tolua_function(tolua_S,"WriteString",tolua_Game_Demo_CDataWriter_WriteString00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Demo",0);
  tolua_beginmodule(tolua_S,"Demo");
   tolua_cclass(tolua_S,"CGame","Demo::CGame","LuaWrap::CScriptObject",NULL);
   tolua_beginmodule(tolua_S,"CGame");
    tolua_function(tolua_S,"ReadDbgInput",tolua_Game_Demo_CGame_ReadDbgInput00);
    tolua_function(tolua_S,"WriteDbgOutput",tolua_Game_Demo_CGame_WriteDbgOutput00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


static CAutoRegToLua s_autoRegToLua(tolua_Game_open, "Game");

FORCE_LINK_DECL(Game);
