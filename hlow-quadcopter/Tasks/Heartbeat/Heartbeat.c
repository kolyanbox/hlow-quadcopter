#include <Tasks/Heartbeat/Heartbeat.h>

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
