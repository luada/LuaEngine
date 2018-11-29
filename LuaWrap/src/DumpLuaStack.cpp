#include "pch.h"
#include "DumpLuaStack.h"
#include "ResIO.h"
#include "Logger.h"


using namespace LuaWrap;

CDumpLuaStack::CDumpLuaStack()
{
}

CDumpLuaStack::~CDumpLuaStack()
{
	Clear();
}

void CDumpLuaStack::Clear()
{
	m_data.clear();
	m_check.clear();
}

const char* CDumpLuaStack::GetInfo() const
{
	return m_data.size() > 0 ? (char*)&m_data[0] : NULL;
}

void CDumpLuaStack::OnDumpLuaStack(const void* pData, int nLen)
{
	if (pData == NULL || nLen == 0)
	{
		return;
	}
	int nOldLen = m_data.size();
	m_data.resize(nLen + nOldLen);
	memcpy(&m_data[nOldLen], pData, nLen);
}

bool CDumpLuaStack::CheckRecord(const void* pRecord)
{
	for (List(const void*)::iterator it = m_check.begin(), endIt = m_check.end();
		it != endIt; ++it)
	{
		if (*it == pRecord)
		{
			return true;
		}
	}
	m_check.push_back(pRecord);
	return false;
}

void CDumpLuaStack::PushChar(char c)
{
	m_data.push_back(c);
}

void CDumpLuaStack::WriteDump(const char* pszFilename)
{
	if (pszFilename != NULL)
	{
		SaveRes(&m_data[0], pszFilename, m_data.size());
	}
}
