call SetVariables.bat

cd ../..

%AutoWrapLuaApp% -SRC %SrcPath%  -HOME %PrjHome% -PRJ %PrjName% -PKG_PATH  %PkgOutPath% -PCH pch.h -CFG %ExpCfgFilename% -TOLUA -n %PrjName% -o %LuaWrapFilename% %MainPkgFilename%

