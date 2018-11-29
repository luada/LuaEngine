#pragma once

#include <assert.h>

namespace LuaWrap{
/**
 *	This class is used to implement singletons. Generally singletons should be
 *	avoided. If they _need_ to be used, try to use this as the base class.
 *
 *	If creating a singleton class CMyApp:
 *
 *	class CMyApp : public CSingleton< CMyApp >
 *	{
 *	};
 *
 *	In cpp file,
 *	SINGLETON_STORAGE(CMyApp)
 *
 *	To use:
 *	CMyApp app; // This will be registered as the singleton
 *
 *	...
 *	CMyApp * pApp = CMyApp::pInstance();
 *	CMyApp & app = CMyApp::Instance();
 */
template <class T>
class CSingleton
{
protected:
	static T * s_pInstance;

public:
	CSingleton()
	{
		assert(NULL == s_pInstance);
		s_pInstance = static_cast<T*>(this);
	}

	~CSingleton()
	{
		assert(this == s_pInstance);
		s_pInstance = NULL;
	}

	inline static T& Instance()
	{
		assert(s_pInstance);
		return *s_pInstance;
	}

	inline static T* InstancePtr()
	{
		return s_pInstance;
	}
};

/**
 *	This should appear in the cpp of the derived singleton class.
 */
#define SINGLETON_STORAGE(TYPE)								\
template <>													\
TYPE * CSingleton< TYPE >::s_pInstance = NULL;				\

//--------------------------------------------------------------------------------

/*	If creating a CSingletonEx class CMyApp:
 *
 *	class CMyApp : public CSingletonEx< CMyApp >
 *	{
 *		DECLARE_SINGLETONEX(CMyApp)
 *	};
 *
 *
 *	To use:
 *	CMyApp app; // This will be registered as the singleton
 *
 *	...
 *	CMyApp * pApp = CMyApp::pInstance();
 *	CMyApp & app = CMyApp::Instance();
 */

template<class T>
class CSingletonEx
{
public:
	inline static T& Instance()
	{
		static T s_instance;
		return s_instance;
	}

	inline static T * InstancePtr()
	{
		return *Instance();
	}
};

#define  DECLARE_SINGLETONEX(ClassName)  \
	private: \
		explicit ClassName(); \
		~ClassName();		  \
		ClassName(const ClassName&); \
		const ClassName& operator = (const ClassName&); \
		friend class CSingletonEx<ClassName>;

}


