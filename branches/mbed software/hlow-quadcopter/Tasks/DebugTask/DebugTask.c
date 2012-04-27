#include <Tasks/DebugTask/DebugTask.h>

void DebugTask (void* pdata)
{
	for(;;)
	{
		WriteDebugInfo("Hello task!\n");
		//setSpeedFront(30);
		CoTickDelay(300);
	}
}
