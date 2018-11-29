#include "pch.h"
#include "LuaDumpMgr.h"
#include "ResIO.h"

namespace LuaWrap{

	struct CLuaDumpMgr::STVerItem
	{
		int	nRef;
		int	nVer;
		DmpTagContainer dmpTags;
	};

	struct CLuaDumpMgr::STDmpTagItem
	{
		unsigned char szMd5[MD5_DIGEST_LENGTH];
	};

	CLuaDumpMgr::CLuaDumpMgr()
	{
	}

	CLuaDumpMgr::~CLuaDumpMgr()
	{
		m_verItems.clear();
	}

	void CLuaDumpMgr::AddVerRef(int nVer)
	{
		VerItemContainer::iterator it = FindVerItem(nVer);
		if (it != m_verItems.end())
		{
			++it->nRef;
			return;
		}

		STVerItem verItem;
		verItem.nVer = nVer;
		verItem.nRef = 1;
		m_verItems.push_back(verItem);
	}

	void CLuaDumpMgr::ReleaseVerRef(int nVer, int nCurVer)
	{
		VerItemContainer::iterator it = FindVerItem(nVer);
		if (it == m_verItems.end())
		{
			return;
		}

		if (--it->nRef < 1 && it->nVer < nCurVer)
		{
			m_verItems.erase(it);
		}
	}

	bool CLuaDumpMgr::OnLuaDump(const char* pszInfo, int nVer, const char* pszPath, bool isAppend, GetMd5BufferFn pGetMd5BuffFn)
	{
		if (pszInfo == NULL || pszPath == NULL)
		{
			return false;
		}

		int size = strlen(pszInfo);
		if (isAppend)
		{
			SaveRes(pszInfo, pszPath, size);
			return false;
		}

		VerItemContainer::iterator findIt = FindVerItem(nVer);
		if (findIt == m_verItems.end())
		{
			return false;
		}
		
		STDmpTagItem item;
		pGetMd5BuffFn((unsigned char*)pszInfo, size, item.szMd5);

		for (DmpTagContainer::iterator it = findIt->dmpTags.begin(), endIt = findIt->dmpTags.end(); it != endIt; ++it)
		{
			if (memcmp(it->szMd5, item.szMd5, MD5_DIGEST_LENGTH) == 0)
			{
				return false;
			}
		}
		findIt->dmpTags.push_back(item);
		SaveRes(pszInfo, pszPath, size);

		return true;
	}

	CLuaDumpMgr::VerItemContainer::iterator CLuaDumpMgr::FindVerItem(int nVer)
	{
		for (VerItemContainer::iterator it = m_verItems.begin(), endIt = m_verItems.end(); it != endIt; ++it)
		{
			if (it->nVer == nVer)
			{
				return it;
			}
		}
		return m_verItems.end();
	}
}
