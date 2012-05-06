#include <Tasks/Main/MainTask.h>

#define HeartBeatStackSize 64
OS_STK	HeartBeat_stk[HeartBeatStackSize];

void MainTask (void* pdata)
{
	/*Create heartbeat task*/
	CoCreateTask (HeartBeat,0,63,&HeartBeat_stk[HeartBeatStackSize-1],HeartBeatStackSize);
	for (;;)
	{
		/*Give a status update every minute*/
		int osTime = CoGetOSTime();
		char c[10];
		Itoa(osTime,c,10);
		WriteDebugInfo("Ostime: ");
		WriteDebugInfo(c);
		WriteDebugInfo("\n");

		int osVersion = CoGetOSVersion();
		Itoa(osVersion,c,10);
		WriteDebugInfo("OsVersion: ");
		WriteDebugInfo(c);
		WriteDebugInfo("\n");

		int curTaskId = CoGetCurTaskID();
		Itoa(curTaskId,c,10);
		WriteDebugInfo("CurTaskId: ");
		WriteDebugInfo(c);
		WriteDebugInfo("\n");

		CoTimeDelay(0,1,0,0);
	}
}
