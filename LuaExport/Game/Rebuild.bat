call SetVariables.bat

if exist LuaWrap.cpp del LuaWrap.cpp

if exist ../../%PkgOutPath%/*.pkg (
	cd ../../%PkgOutPath%
	del /s *.pkg
	cd ../../../LuaExport/%PrjName%
)

pause

call LuaWrapExp.bat

pause
