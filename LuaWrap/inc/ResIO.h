#pragma once

#include "MemMgr.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>


#ifdef WIN32
	#include <WinSock2.h>
#else	//Linux
	#include <stdint.h>
	#include <dirent.h>
#endif

#define MD5_DIGEST_LENGTH		16

//#define USE_SO_RESMGR

#ifdef USE_SO_RESMGR


namespace Demo
{
	extern const char* LoadRes(const char* pszFilename, int nGameId, int nVer, int& nSize);
	extern void UnloadRes(const char* pData, const char* pszFilename, int nGameId, int nVer, int nSize);
}
#endif //USE_SO_RESMGR

namespace LuaWrap{

inline const char* LoadRes(const char* pszFilename, int nGameId, int nVer, int& nSize)
{
#ifdef USE_SO_RESMGR
	return Demo::LoadRes(pszFilename, nGameId, nVer, nSize);
#else
	nSize = 0;
	char* pData = NULL;

	if (pszFilename != NULL)
	{
		char path[256];
		sprintf(path, "script/%s", pszFilename);
		FILE* f = fopen(path, "rb");

		if (f != NULL)
		{
			fseek(f, 0, SEEK_END);
			nSize = ftell(f);
			if(nSize > 0)
			{
				fseek(f, 0, SEEK_SET);
				pData = (char*) Malloc(nSize);
				fread(pData, nSize, 1, f);
			}
			fclose(f);
		}
	}
	return pData;
#endif // USE_SO_RESMGR
}

inline void UnloadRes(const char* pData, const char* pszFilename, int nGameId, int nVer, int nSize)
{
#ifdef USE_SO_RESMGR
	return Demo::UnloadRes(pData, pszFilename, nGameId, nVer, nSize);
#else
	Free(const_cast<char*>(pData), nSize);
#endif // USE_SO_RESMGR
}

inline bool SaveRes(const char* pData, const char* pszFilename, int nSize)
{
	if (pData == NULL || pszFilename == NULL)
	{
		return false;
	}

	FILE* f = fopen(pszFilename, "ab+");
	if (f == NULL)
	{
		return false;
	}
	fwrite(pData, nSize, 1, f);
	fclose(f);
	return true;
}

inline const char* ScriptEntry(int nGameId, int nVer)
{
	return "main";
}

}
