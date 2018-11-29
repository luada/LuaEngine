#include "PkgExporter.h"
#include "LuaWrapGenerator.h"



extern "C"
{
	void scriptMsg(const char* msg)
	{
		LogMsg << msg;
	}

	struct lua_State;

	void scriptOnError(lua_State* L, int errcode)
	{
		LogMsg << "scriptOnError";
	}
};

INT _tmain(INT argc, _TCHAR* argv[])
{
	LogMsg << "*********************************************************************\n";
	LogMsg << "*****************  Welcome to use \'AutoWrapLua\'!   ******************\n";
	LogMsg << "*****************         Written by Luada         ******************\n";
	LogMsg << "*********************************************************************\n";

	INT* inputFilenameIndexs = new INT[argc];
	INT inputFilenameCount = 0;
	g_exportCfgFilename = DefaultExportConfig;
	g_genPkgfileCount = 0;
	g_workHomeDir = NULL;

	CHAR** luaWrapArgv = new CHAR* [argc];
	INT luaWrapArgc = 0;
	TCHAR* wrapLuaFilename = NULL;

	luaWrapArgv[luaWrapArgc++] = TCHARToCHAR(argv[0]);

	enum CtrlState
	{
		NULLState,
		InputSrcState,
		PreCompiledHeaderState,
		ToluaAddPathState,
		OutputCfgState,
		LuaWrapCtrlState,
		WorkHomeDirState,
		WorkPrjDirState,
		PkgPathState,
	};

	CtrlState ctrlState = NULLState;

	for (INT i = 1; i < argc; ++i)
	{
		if (IsEqual(argv[i], InputSrcCtrlFlag))
		{
			ctrlState = InputSrcState;
		}
		else if (IsEqual(argv[i], OutputCfgCtrlFlag))
		{
			ctrlState = OutputCfgState;
		}
		else if (IsEqual(argv[i], LuaWrapCtrlFlag))
		{
			ctrlState = LuaWrapCtrlState;
		}
		else if (IsEqual(argv[i], WorkHomeDirFlag))
		{
			ctrlState = WorkHomeDirState;
		}
		else if (IsEqual(argv[i], WorkPrjDirFlag))
		{
			ctrlState = WorkPrjDirState;
		}
		else if (IsEqual(argv[i], PreCompiledHeaderFlag))
		{
			ctrlState = PreCompiledHeaderState;
		}
		else if (IsEqual(argv[i], PkgPathFlag))
		{
			ctrlState = PkgPathState;
		}
		else if (IsEqual(argv[i], ToLuaAddPathFlag))
		{
			ctrlState = ToluaAddPathState;
		}
		else
		{
			switch(ctrlState)
			{
			case InputSrcState:
				{
					inputFilenameIndexs[inputFilenameCount++] = i;
					break;
				}
			case OutputCfgState:
				{
					g_exportCfgFilename = argv[i];
					break;
				}
			case LuaWrapCtrlState:
				{
					luaWrapArgv[luaWrapArgc++] = TCHARToCHAR(argv[i]);
					if (IsEqual(argv[i], ToluaOutputFlag) && i + 1 < argc)
					{
						wrapLuaFilename = argv[i+1];
					}
					break;
				}
			case WorkHomeDirState:
				{
					g_workHomeDir = argv[i];
					break;
				}
			case WorkPrjDirState:
				{
					g_workPrjDir = argv[i];
					break;
				}
			case PreCompiledHeaderState:
				{
					g_pch = argv[i];
					break;
				}
			case ToluaAddPathState:
				{
					g_toluaAddPath = argv[i];
					break;
				}
			case PkgPathState:
				{
					g_PkgPath = argv[i];
					break;
				}
			default:
				break;
			}
		}
	}

	getchar();

	SetFileData(g_exportCfgFilename, ExportConfigDim, FALSE);

	LogMsg << "\n\nGenerating pkg files...\n";
	for (INT i = 0; i < inputFilenameCount; ++i)
	{
		LPCTSTR filename = argv[inputFilenameIndexs[i]];
		if (IsFileExists(filename))
		{
			ParseFile(filename);
		}
		else
		{
			ParseDir(filename);
		}
	}

	CHAR* info = TCHARToCHAR(g_exportCfgFilename);
	LogMsg << "\nGenerating configure...\n" << info << "\n";
	SafeDelArray(info);

	if (luaWrapArgc > 1)
	{
		LPCTSTR pkgMainFilename = argv[argc - 1];
		if (g_genPkgfileCount > 0 || !IsFileExists(wrapLuaFilename) || IsManualPkgNew(pkgMainFilename, wrapLuaFilename))
		{
			GenLuaWrap(luaWrapArgc, luaWrapArgv);
		}
		else
		{
			LogMsg << "\n\nNo LuaWrap functions changes!";
		}
	}

	while(--luaWrapArgc >= 0)
	{
		SafeDelArray(luaWrapArgv[luaWrapArgc]);
	}

	SafeDelArray(luaWrapArgv);
	SafeDelArray(inputFilenameIndexs);

	LogMsg << "\nAutoWrapLua Done!\n";

	return 0;
}