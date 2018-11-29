#include "DataWriter.h"

namespace Demo
{
	CDataWriter::CDataWriter(int nSize):
		m_pBuff(m_data), m_nCurIndex(0), m_nBuffLen(sizeof(m_data)), m_bExtraBuff(false)
	{
		m_data[0] = '\0';
		SetSize(nSize);
	}

	CDataWriter::CDataWriter(void* pBuff, int nSize): 
		m_pBuff((char*)pBuff), m_nCurIndex(0), m_nBuffLen(nSize), m_bExtraBuff(true)
	{
		m_data[0] = '\0';
	}

	CDataWriter::~CDataWriter()
	{
		Clear();
	}

	void CDataWriter::Clear()
	{
		if (m_pBuff != m_data && !m_bExtraBuff)
		{
			LuaWrap::Free(m_pBuff);
		}
		m_pBuff = m_data;
		m_bExtraBuff = false;
		m_nCurIndex = 0;
		m_nBuffLen = sizeof(m_data);
	}

	void* CDataWriter::Reserve(int nSize)
	{
		int nNxtIndex = m_nCurIndex + nSize;
		while (nNxtIndex > m_nBuffLen)
		{
			Resize();
		}

		void* pRet = m_pBuff + m_nCurIndex;
		m_nCurIndex = nNxtIndex;
		return pRet;
	}

	void CDataWriter::Resize()
	{
		SetSize(m_nBuffLen * 2);
	}

	void CDataWriter::SetSize(int nSize)
	{
		if (nSize <= m_nBuffLen)
		{
			return;
		}

		void* pNewBuff = LuaWrap::Malloc(nSize);
		memcpy(pNewBuff, m_pBuff, m_nCurIndex);
		if (m_pBuff != m_data && !m_bExtraBuff)
		{
			LuaWrap::Free(m_pBuff);
		}
		m_bExtraBuff = false;
		m_pBuff = (char*)pNewBuff;
		m_nBuffLen = nSize;
	}

	void CDataWriter::AddBlob(const void* pcData, int nDataLen)
	{
		void* pDst = Reserve(nDataLen);
		memcpy(pDst, pcData, nDataLen);
	}

	void CDataWriter::WriteUInt(UINT uValue)
	{
		return WriteValue(uValue);
	}

	void CDataWriter::WriteInt(int nValue)
	{
		return WriteValue(nValue);
	}

	void CDataWriter::WriteUShort(USHORT usValue)
	{
		return WriteValue(usValue);
	}

	void CDataWriter::WriteShort(short sValue)
	{
		return WriteValue(sValue);
	}

	void CDataWriter::WriteFloat(float fValue)
	{
		return WriteValue(fValue);
	}

	void CDataWriter::WriteDouble(double dValue)
	{
		return WriteValue(dValue);
	}

	void CDataWriter::WriteBool(bool bValue)
	{
		return WriteValue(bValue);
	}

	void CDataWriter::WriteChar(char cValue)
	{
		return WriteValue(cValue);
	}

	void CDataWriter::WriteByte(BYTE btValue)
	{
		return WriteValue(btValue);
	}

	void CDataWriter::WriteINT8(INT8 i8Value)
	{
		return WriteValue(i8Value);
	}

	void CDataWriter::WriteUINT8(UINT8 ui8Value)
	{
		return WriteValue(ui8Value);
	}

	void CDataWriter::WriteINT16(INT16 i16Value)
	{
		return WriteValue(i16Value);
	}

	void CDataWriter::WriteUINT16(UINT16 ui16Value)
	{
		return WriteValue(ui16Value);
	}

	void CDataWriter::WriteINT32(INT32 i32Value)
	{
		return WriteValue(i32Value);
	}

	void CDataWriter::WriteUINT32(UINT32 ui32Value)
	{
		return WriteValue(ui32Value);
	}

	void CDataWriter::WritePointer(void* pObj)
	{
		WriteValue(pObj);
	}

	void CDataWriter::WriteStrBuffer(const char* pszValue, int nBuffLen)
	{
		int nStrLen = strlen(pszValue);
		char* pDst = (char*) Reserve(nBuffLen);
		int nCpyLen = Min(nStrLen, nBuffLen);
		memcpy(pDst, pszValue, nCpyLen);
		pDst[nCpyLen] = '\0';
	}

	void CDataWriter::WriteString(const char* pszValue)
	{
		UINT16 ui16Len = (UINT16) strlen(pszValue) + 1;
		WriteUINT16(ui16Len);

		char* pDst = (char*) Reserve(ui16Len);
		memcpy(pDst, pszValue, ui16Len);
		pDst[ui16Len - 1] = '\0';
	}
}
