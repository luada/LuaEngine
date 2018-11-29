#pragma once

#include <vector>
#include <list>
#include <map>
#include <string>
#include <stdarg.h>
#include <string.h>
#include "Allocator.h"

using namespace std;

namespace LuaWrap{

	#define BaseString(Type)    basic_string<Type, char_traits<Type>, CAllocator<Type> >
	#define Vector(Type)        vector<Type, CAllocator<Type> >
	#define List(Type)          list<Type, CAllocator<Type> >

	typedef BaseString(char)  String;

	typedef vector<double, CAllocator<double> > NumberVEC;
	typedef vector<String, CAllocator<String> > StringVEC;
	typedef map<String, String, less<String>, CAllocator<pair<const String, String > > > StringMap;
	typedef map<int, String, less<int>, CAllocator<pair<const int, String > > > IntStrMap;
}
