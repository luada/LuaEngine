#pragma once

#include "Singleton.h"
#include "Type.h"

typedef int (*GetMd5BufferFn)(unsigned char *pbyBuf, int iBufLen, unsigned char *pbyOut);


namespace LuaWrap{

	class CLuaDumpMgr: public CSingletonEx<CLuaDumpMgr>
	{
		DECLARE_SINGLETONEX(CLuaDumpMgr)
	private:
		struct STVerItem;
		struct STDmpTagItem;

		typedef List(STVerItem)		VerItemContainer;
		typedef List(STDmpTagItem)	DmpTagContainer;

	public:
		void AddVerRef(int nVer);
		void ReleaseVerRef(int nVer, int nCurVer);
		bool OnLuaDump(const char* pszInfo, int nVer, const char* pszPath, bool isAppend, GetMd5BufferFn pGetMd5BuffFn);

	private:
		VerItemContainer::iterator FindVerItem(int nVer);

	private:
		VerItemContainer m_verItems;
	};
}
