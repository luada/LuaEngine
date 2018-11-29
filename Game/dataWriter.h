#pragma once

#include "server.h"

namespace Demo
{
	class CDataWriter
	{
		ScriptWrapImplBegin(Demo::CDataWriter)
		ScriptWrapImplEnd()
		
	public:
		//@BeginExportToLua
		CDataWriter(int nSize = MAX_BUFF_SIZE);
		CDataWriter(void* pBuff, int nSize);
		virtual ~CDataWriter();

		void* Data() const { return m_pBuff; }
		int DataLen() { return m_nCurIndex; }
		int BufferLen() { return m_nBuffLen; }

		void SetDataLen(int nLen) { m_nCurIndex = nLen; }
		void Resize();

		void Clear();
		void AddBlob(const void* pcData, int nDataLen);

		void WriteUInt(UINT uValue);
		void WriteInt(int nValue);
		void WriteUShort(USHORT usValue);
		void WriteShort(short sValue);
		void WriteFloat(float fValue);
		void WriteDouble(double dValue);
		void WriteBool(bool bValue);
		void WriteChar(char cValue);
		void WriteByte(BYTE btValue);
		void WriteINT8(INT8 i8Value);
		void WriteUINT8(UINT8 ui8Value);
		void WriteINT16(INT16 i16Value);
		void WriteUINT16(UINT16 ui16Value);
		void WriteINT32(INT32 i32Value);
		void WriteUINT32(UINT32 ui32Value);
		void WritePointer(void* pObj);
		void WriteStrBuffer(const char* pszValue, int nBuffLen);
		void WriteString(const char* pszValue);
		//@EndExportToLua

		void* Reserve(int nSize);
		void SetSize(int nSize);

		template<class T>
		void WriteValue(const T& value)
		{
			void* pDst = Reserve(sizeof(T));
			memcpy(pDst, &value, sizeof(T));
		}

	private:
		char m_data[MAX_BUFF_SIZE];
		char* m_pBuff;
		int m_nCurIndex;
		int m_nBuffLen;
		bool m_bExtraBuff;
	};

	template<class T>
	CDataWriter& operator & (CDataWriter& dw, const T& v)
	{
		dw.WriteValue(v);
		return dw;
	}
}
