#include <Tasks/Distancetoground/DistanceToGroundTask.h>

void DistanceToGroundTask (void* pdata)
{
	for(;;)
	{
		int distanceToGround = getCurrentHeightInCm();
		char c[30];
		Itoa(distanceToGround,c,10);
		WriteDebugInfo("Distance: ");
		WriteDebugInfo(c);
		WriteDebugInfo("\n");
		CoTimeDelay(0,0,30,0);
	}
}
