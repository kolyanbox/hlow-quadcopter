#include <Tasks/Heartbeat/Heartbeat.h>

#define HeartBeatStackSize 64
OS_STK	HeartBeat_stk[HeartBeatStackSize];

taskDef t;
taskDef getHeartbeatTaskDefenition()
{
	t.priority = 63;
	t.stk = &HeartBeat_stk[HeartBeatStackSize-1];
	t.stkSz = HeartBeatStackSize;
	t.task = HeartBeat;
	t.taskName = "Heartbeat";
	return t;
}

void HeartBeat (void* pdata)
{
  for (;;)
  {
	  setLed(led1,TRUE);
	  CoTimeDelay(0,0,1,0);
	  setLed(led1,FALSE);
	  CoTimeDelay(0,0,1,0);
  }
}
