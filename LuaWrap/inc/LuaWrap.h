#pragma once

#ifdef USE_LUA_JIT
	#include "lua.hpp"
	
	extern "C"
	{
		#include "tolua++.h"
	}
#else
	extern "C"
	{
		#include "lua.h"
		#include "lualib.h"
		#include "lauxlib.h"
		#include "lstring.h"
		#include "lundump.h"
		#include "tolua++.h"
	}
#endif // USE_LUA_JIT
