echo off

set AutoWrapLuaApp=LuaExport\AutoWrapLua.exe
set PrjName=Game
set PrjHome=Game
set SrcPath=%PrjHome%
set PkgOutPath=BuildOut\Pkg\%PrjName%
set ExpCfgFilename=%PkgOutPath%\exportConfig.pkg
set LuaWrapFilename=LuaExport\%PrjName%\LuaWrap.cpp
set MainPkgFilename=LuaExport\%PrjName%\Main.pkg
