#include "client.h"

namespace Demo
{
	CClient::CClient()
		: m_socket(INVALID_SOCKET)
	{

	}

	CClient::~CClient()
	{
		Clear();
	}

	int CClient::Init(int socket)
	{
		Clear();
		m_socket = socket;
		return 0;
	}

	void CClient::Clear()
	{
		m_socket = INVALID_SOCKET;
		m_buffer.clear();
	}


	BufferType& CClient::GetBuffer()
	{
		return m_buffer;
	}

	int CClient::GetSocket()
	{
		return m_socket;
	}



	int CClient::OnRead(const char* buffer, int size)
	{

		if (buffer != NULL && size > 0) 
		{
			int clientBufferSize = m_buffer.size();
			m_buffer.resize(clientBufferSize + size);
			memcpy(&m_buffer[clientBufferSize], buffer, size);
		}

		return 0;
	}

	int CClient::GetPackage(char* bufferOut, int bufferSize)
	{
		int size = m_buffer.size();
		if (size < 2)
		{
			return -1;
		}

		short len = ntohs(*(short*)&m_buffer[0]);
		if (len > bufferSize - 1)
		{
			return -2;
		}

		memcpy(bufferOut, &m_buffer[2], len);
		bufferOut[len] = 0;

		int left = size - len - 2;
		if (left > 0)
		{
			memmove(&m_buffer[0], &m_buffer[len + 2], left);
		}
		m_buffer.resize(left);

		return len;

	}
}
