#pragma once

#include <stdarg.h>
#include <stdio.h>


namespace LuaWrap{

#ifdef __WIN__
    #define vsnprintf _vsnprintf
#endif

#define _USE_LOG_

#ifdef _USE_LOG_
	inline void Log(const char* pszFmt, ...)
	{
		if (pszFmt == NULL)
		{
			return;
		}

		char szBuff[1024];
		va_list ap;
		va_start(ap, pszFmt);
		vsnprintf(szBuff, sizeof(szBuff), pszFmt, ap);
		va_end(ap);
		fprintf(stdout, "LOG_MSG: %s\n", szBuff);
	}
#else
	inline void Log(const char* pszFmt, ...)
	{
	}
#endif //_USE_LOG_

}
