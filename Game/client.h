#pragma once

#include "server.h"

namespace Demo
{
	typedef std::vector<unsigned char> BufferType;

	class CClient
	{
	public:
		CClient();
		~CClient();

		int Init(int socket);
		void Clear();

		BufferType& GetBuffer();
		int GetSocket();

		int OnRead(const char* buffer, int size);

		int GetPackage(char* bufferOut, int bufferSize);

	private:
		int m_socket;
		BufferType m_buffer;

	};
}
