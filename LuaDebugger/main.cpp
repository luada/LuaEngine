#include "pch.h"
#include "RemoteDebugger.h"


int main()
{
	CRemoteDebugger debugger;
	if (debugger.Init())
	{
		debugger.Run();
		return 0;
	}
	return -1;
}
