#pragma once

#include <memory>
#include "MemMgr.h"

namespace LuaWrap{

	template<typename T>
	class CAllocator: public std::allocator<T>
	{
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef const value_type* const_pointer;
		typedef const value_type& const_reference;
		typedef size_t size_type;

		template<class OTHER>
		struct rebind
		{
		    typedef CAllocator<OTHER> other;
        };

		inline pointer address(reference val) const             { return (&val); }
		inline const_pointer address(const_reference val) const { return (&val); }

		CAllocator() throw() {}
		CAllocator(const CAllocator<T>&) throw(){}

		template<class OTHER>
		inline CAllocator(const CAllocator<OTHER>&) throw(){}

		template<class OTHER>
		inline CAllocator<T>& operator=(const CAllocator<OTHER>&){ return (*this); }

		inline void deallocate(pointer ptr, size_type size)
		{
		    return Free(ptr, size);
        }

		inline pointer allocate(size_type count)
		{
			return static_cast<pointer>(Malloc(count * sizeof(T)));
		}

		inline pointer allocate(size_type count, const void*)       { return (allocate(count)); }
		inline void construct(pointer ptr, const T& val)            { std::_Construct(ptr, val); }
		inline void destroy(pointer ptr)                            { std::_Destroy(ptr); }

		inline size_t max_size() const throw()
		{
			size_t count = (size_t)(-1) / sizeof (T);
			return (0 < count ? count : 1);
		}
	};
}
