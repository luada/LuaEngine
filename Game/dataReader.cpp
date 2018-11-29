#include "DataReader.h"

namespace Demo
{
	CDataReader::CDataReader(const void* pData, int nLen):
		m_pData((const char*)pData), m_nCurIndex(0),
		m_nDataLen(pData != NULL ? nLen : 0)
	{
		m_szBuff[0] = '0';
	}

	CDataReader::~CDataReader()
	{
		m_pData = NULL;
	}

	void CDataReader::AttachData(const void* pData, int nLen)
	{
		m_pData = (const char*) pData;
		m_nDataLen = pData != NULL ? nLen : 0;
		m_nCurIndex = 0;
	}

	UINT CDataReader::ReadUInt(UINT uDefault)
	{
		return ReadValue(uDefault);
	}

	int CDataReader::ReadInt(int nDefault)
	{
		return ReadValue(nDefault);
	}

	USHORT CDataReader::ReadUShort(USHORT usDefault)
	{
		return ReadValue(usDefault);
	}

	short CDataReader::ReadShort(short sDefault)
	{
		return ReadValue(sDefault);
	}

	float CDataReader::ReadFloat(float fDefault)
	{
		return ReadValue(fDefault);
	}

	double CDataReader::ReadDouble(double dDefault)
	{
		return ReadValue(dDefault);
	}

	bool CDataReader::ReadBool(bool bDefault)
	{
		return ReadValue(bDefault);
	}

	char CDataReader::ReadChar(char cDefault)
	{
		return ReadValue(cDefault);
	}

	BYTE CDataReader::ReadByte(BYTE btDefault)
	{
		return ReadValue(btDefault);
	}

	INT8 CDataReader::ReadINT8(INT8 i8Default)
	{
		return ReadValue(i8Default);
	}

	UINT8 CDataReader::ReadUINT8(UINT8 ui8Default)
	{
		return ReadValue(ui8Default);
	}

	INT16 CDataReader::ReadINT16(INT16 i16Default)
	{
		return ReadValue(i16Default);
	}

	UINT16 CDataReader::ReadUINT16(UINT16 ui16Default)
	{
		return ReadValue(ui16Default);
	}

	INT32 CDataReader::ReadINT32(INT32 i32Default)
	{
		return ReadValue(i32Default);
	}

	UINT32 CDataReader::ReadUINT32(UINT32 ui32Default)
	{
		return ReadValue(ui32Default);
	}

	void* CDataReader::ReadPointer(void* pDefault)
	{
		return ReadValue(pDefault);
	}

	const char* CDataReader::ReadStrBuffer(int nBuffLen, const char* pszDefault)
	{
		if (LeftLen() < nBuffLen || m_pData == NULL)
		{
			return pszDefault;
		}
		int nCanReadLen = Min(nBuffLen, MAX_STR_BUFF_SIZE - 1);
		memcpy(m_szBuff, m_pData + m_nCurIndex, nCanReadLen);
		m_szBuff[nCanReadLen] = '\0';
		m_nCurIndex += nBuffLen;
		return m_szBuff;
	}

	const char* CDataReader::ReadString(const char* pszDefault)
	{
		UINT16 ui16Len = ReadUINT16();
		if (LeftLen() < ui16Len)
		{
			return pszDefault;
		}

		int nCanReadLen = Min((int)ui16Len, MAX_STR_BUFF_SIZE - 1);
		memcpy(m_szBuff, m_pData + m_nCurIndex, nCanReadLen);
		m_szBuff[nCanReadLen] = '\0';
		m_nCurIndex += ui16Len;
		return m_szBuff;
	}
}
