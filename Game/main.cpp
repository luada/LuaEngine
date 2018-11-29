#include "server.h"

void main()
{
	Demo::CServer server;
	if (server.Init())
	{
		server.Run();
	}
}
