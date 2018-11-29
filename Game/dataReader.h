#pragma once

#include "server.h"


namespace Demo
{
	class CDataReader
	{
		ScriptWrapImplBegin(Demo::CDataReader)
		ScriptWrapImplEnd()

		enum { MAX_STR_BUFF_SIZE = 1024 };
	public:
		//@BeginExportToLua
		CDataReader(const void* pData, int nLen);
		virtual ~CDataReader();

		void AttachData(const void* pData, int nLen);
		const void* Data() const { return m_pData; };
		const void* CurData() const { return m_pData + m_nCurIndex; }
		int DataLen() const { return m_nDataLen; }
		int LeftLen() const { return m_nDataLen - m_nCurIndex; }
		void Clear() { m_nDataLen = 0; m_pData = NULL; m_nCurIndex = 0; }

		int GetCur() const { return m_nCurIndex; }
		void SetCur(int nCur) { m_nCurIndex = nCur; }
		void Skip(int nLen) { m_nCurIndex += nLen; }

		UINT ReadUInt(UINT uDefault = 0);
		int ReadInt(int nDefault = 0);
		USHORT ReadUShort(USHORT usDefault = 0);
		short ReadShort(short sDefault = 0);
		float ReadFloat(float fDefault = 0.f);
		double ReadDouble(double dDefault = 0.0);
		bool ReadBool(bool bDefault = false);
		char ReadChar(char cDefault = '\0');
		BYTE ReadByte(BYTE btDefault = 0);
		INT8 ReadINT8(INT8 i8Default = 0);
		UINT8 ReadUINT8(UINT8 ui8Default = 0);
		INT16 ReadINT16(INT16 i16Default = 0);
		UINT16 ReadUINT16(UINT16 ui16Default = 0);
		INT32 ReadINT32(INT32 i32Default = 0);
		UINT32 ReadUINT32(UINT32 ui32Default = 0);
		void* ReadPointer(void* pDefault = NULL);
		const char* ReadStrBuffer(int nBuffLen, const char* pszDefault = "");
		const char* ReadString(const char* pszDefault = "");
		//@EndExportToLua

	public:
		template<class T>
		T ReadValue(const T& defaultValue)
		{
			if(LeftLen() < (int) sizeof(T))
			{
				return defaultValue;
			}

			T value = *((T*)(m_pData + m_nCurIndex));
			m_nCurIndex += sizeof(T);
			return value;
		}

		template<class T>
		bool Read(T& valueOut)
		{
			if (LeftLen() < (int) sizeof(T))
			{
				return false;
			}

			valueOut = *((T*)(m_pData + m_nCurIndex));
			m_nCurIndex += sizeof(T);
			return true;
		}


	private:
		const char* m_pData;
		char m_szBuff[MAX_STR_BUFF_SIZE];
		int m_nCurIndex;
		int m_nDataLen;
	};

	template<class T>
	CDataReader& operator & (CDataReader& dr, T& v)
	{
		if (!dr.Read(v))
		{
			dr.Clear();
		}
		return dr;
	}
}
