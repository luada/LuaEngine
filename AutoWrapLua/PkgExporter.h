#ifndef PKG_EXPORTER_H
#define PKG_EXPORTER_H

#include <afx.h> 
#include <io.h> 
#include <iostream>
#include "Macro.h"

#define InputSrcCtrlFlag		_T("-SRC")
#define PreCompiledHeaderFlag	_T("-PCH")
#define ToLuaAddPathFlag		_T("-TLADD")
#define OutputCfgCtrlFlag		_T("-CFG")
#define LuaWrapCtrlFlag			_T("-TOLUA")
#define ToluaOutputFlag			_T("-o")
#define WorkHomeDirFlag			_T("-HOME")
#define WorkPrjDirFlag			_T("-PRJ")
#define PkgPathFlag				_T("-PKG_PATH")

#define ExportConfigDim			_T("//This file is created by AutoWrapLua.exe\r\n\r\n")
#define DefaultExportConfig		_T("exportConfig.pkg")
#define ToluaWrapFuncBegin		_T("static void tolua_reg_types")

#define LogMsg					std::cout


VOID ParseDir(LPCTSTR pstrDirname, INT basePathLen = -1);
VOID ParseFile(LPCTSTR pstrFilename);
BOOL IsEqual(LPCTSTR psrc1, LPCTSTR pstr2);
BOOL IsFileExists(LPCTSTR pstr);
BOOL SetFileData(LPCTSTR pstrFilename, LPCTSTR pData, BOOL isAppen);
CHAR* TCHARToCHAR(LPCTSTR pData);
BOOL FixLuaWrapSrc(LPCSTR filename);
BOOL IsManualPkgNew(LPCTSTR pstrMain, LPCTSTR pstrLuaWrapFilename);

extern LPCTSTR	g_exportCfgFilename;
extern LPCTSTR  g_workHomeDir;
extern LPCSTR	g_workPrjDir;
extern LPCTSTR	g_pch;
extern LPCTSTR  g_toluaAddPath;
extern LPCTSTR	g_PkgPath;
extern INT		g_genPkgfileCount;

#endif //PKG_EXPORTER_H