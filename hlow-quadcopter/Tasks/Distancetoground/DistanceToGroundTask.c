#include <Tasks/Distancetoground/DistanceToGroundTask.h>
#include <General/util.h>
char c[30];
void DistanceToGroundTask (void* pdata)
{
	for(;;)
	{
		int distanceToGround = getCurrentHeightInCm();
		Itoa(distanceToGround,c,10);
		WriteDebugInformation(c,DistanceToGround);
		CoTimeDelay(0,0,1,0);
	}
}
