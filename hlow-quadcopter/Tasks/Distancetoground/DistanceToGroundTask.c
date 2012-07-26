#include <Tasks/Distancetoground/DistanceToGroundTask.h>
#include <General/util.h>
char distanceToGroundVal[30];

#define DistancToGroundStackSize 128
OS_STK	Distance_stk[DistancToGroundStackSize];

const char commandDtg[] = {"getdtg"};

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

char * printInfoDistanceToGround(int argc, char *args[])
{
	if (argc != 0)
	{
		return "Unknown amount of parameters";
	}

	return distanceToGroundVal;
}

void DistanceToGroundTask (void* pdata)
{
	//register angle app in cli
	registerInterface(commandDtg,printInfoDistanceToGround);
	for(;;)
	{
		int distanceToGround = getCurrentHeightInCm();
		Itoa(distanceToGround,distanceToGroundVal,10);
		CoTimeDelay(0,0,1,0);
	}
}
