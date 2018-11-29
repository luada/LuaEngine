echo off

set AutoWrapLuaApp=LuaExport\AutoWrapLua.exe
set PrjName=LuaWrap
set PrjHome=LuaWrap\inc
set SrcPath=%PrjHome%
set PkgOutPath=BuildOut\Pkg\%PrjName%
set ExpCfgFilename=%PkgOutPath%\exportConfig.pkg
set LuaWrapFilename=LuaExport\%PrjName%\LuaWrap.cpp
set MainPkgFilename=LuaExport\%PrjName%\Main.pkg
