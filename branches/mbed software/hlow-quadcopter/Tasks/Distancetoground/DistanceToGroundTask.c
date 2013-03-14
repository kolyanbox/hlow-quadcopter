#include <Tasks/Distancetoground/DistanceToGroundTask.h>
#include <General/util.h>
char distanceToGroundVal[30];
char distanceToGroundSeaLevelVal[30];

#define DistancToGroundStackSize 128
OS_STK	Distance_stk[DistancToGroundStackSize];

const char commandDtg[] = {"getdtg"};
const char commandSetDtg[] = {"setdtg"};
char wrongdtgCommand[] = {"Unknown amount of parameters!\nA valid parameter is a or r."};

float pressureAtSL = 0;

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
	if (argc != 1)
	{
		return wrongdtgCommand;
	}

	char a[] = {"a"};
	char r[] = {"r"};
	if (Strcmp(args[0],a) == 0)
	{
		return distanceToGroundSeaLevelVal;
	}
	else if (Strcmp(args[0],r) == 0)
	{
		return distanceToGroundVal;
	}

	return wrongdtgCommand;
}

char * setDistanceToGround(int argc, char *args[])
{
	if (argc != 1)
	{
		return "Unknown amount of parameters.\nParameter should contain a number";
	}

	int meter = Atoi(args[0]);

	pressureAtSL = calculateCurrentPressureAtSeaLevel(meter);
	return "Distance has been set.";
}

void DistanceToGroundTask (void* pdata)
{
	//register angle app in cli
	registerInterface(commandDtg,printInfoDistanceToGround);
	registerInterface(commandSetDtg,setDistanceToGround);

	pressureAtSL = calculateCurrentPressureAtSeaLevel(0);
	for(;;)
	{
		float distanceToGround = (float)getCurrentHeightInCm();
		float distanceToGroundSeaLevel = getCurrentAltitude(pressureAtSL)*100;

		Ftoa(distanceToGround,distanceToGroundVal,5,'f');
		Ftoa(distanceToGroundSeaLevel,distanceToGroundSeaLevelVal,5,'f');

		CoTimeDelay(0,0,0,50);
	}
}
