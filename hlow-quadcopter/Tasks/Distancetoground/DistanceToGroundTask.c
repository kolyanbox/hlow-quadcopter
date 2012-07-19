#include <Tasks/Distancetoground/DistanceToGroundTask.h>
#include <General/util.h>
char c[30];

#define DistancToGroundStackSize 128
OS_STK	Distance_stk[DistancToGroundStackSize];

taskDef t;
taskDef getDistanceTaskDefenition()
{
	t.priority = 63;
	t.stk = &Distance_stk[DistancToGroundStackSize-1];
	t.stkSz = DistancToGroundStackSize;
	t.task = DistanceToGroundTask;
	t.taskName = "Distance";
	return t;
}

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
