#pragma once

#include "Type.h"

namespace LuaWrap{

	class CDumpLuaStack
	{
	public:
		CDumpLuaStack();
		~CDumpLuaStack();

		void Clear();
		void WriteDump(const char* pszFilename);
		void OnDumpLuaStack(const void* pData, int nLen);
		bool CheckRecord(const void* pRecord);
		void PushChar(char c);
		const char* GetInfo() const;
	private:
		Vector(char) m_data;
		List(const void*)  m_check;
	};
}
