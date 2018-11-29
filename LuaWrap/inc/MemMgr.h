#pragma once

#include <memory>

//#define USE_SO_MEM_POOL

#ifdef USE_SO_MEM_POOL
	#include "../../Common/MemoryOp.h"
#endif // USE_SO_MEM_POOL

namespace LuaWrap{

inline void* Malloc(size_t size)
{
#ifdef USE_SO_MEM_POOL
	return Demo::SoMalloc(size);
#else
	void* ptr = (void*)new char[size];
	return ptr;
#endif // USE_SO_MEM_POOL
}

inline void Free(void* ptr, size_t size = 0)
{
#ifdef USE_SO_MEM_POOL
	return Demo::SoFree(ptr);
#else
	delete [] (char*)ptr;
#endif // USE_SO_MEM_POOL
}

}
