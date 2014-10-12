#include <Tasks/Distancetoground/DistanceToGroundTask.h>
#include <General/util.h>
char distanceToGroundVal[30];
char distanceToGroundSeaLevelVal[30];

#define DistancToGroundStackSize 256
OS_STK	Distance_stk[DistancToGroundStackSize];

#define SAMPLES 120

const char commandDtg[] = {"getdtg"};
const char commandSetDtg[] = {"setdtg"};
char wrongdtgCommand[] = {"Unknown amount of parameters!\nA valid parameter is a or r."};

OS_MutexID distanceToGroundMutex;

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
	CoEnterMutexSection(distanceToGroundMutex);
	if (Strcmp(args[0],a) == 0)
	{
		CoLeaveMutexSection(distanceToGroundMutex);
		return distanceToGroundSeaLevelVal;
	}
	else if (Strcmp(args[0],r) == 0)
	{
		CoLeaveMutexSection(distanceToGroundMutex);
		return distanceToGroundVal;
	}

	CoLeaveMutexSection(distanceToGroundMutex);
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

	float samples[SAMPLES];
	int i;
	for (i=0;i<SAMPLES-1;i++)
	{
		samples[i] = 0;
	}
	i=0;

	pressureAtSL = calculateCurrentPressureAtSeaLevel(0);
	for(;;)
	{
		float distanceToGround = (float)getCurrentHeightInCm();
		float distanceToGroundSeaLevel = getCurrentAltitude(pressureAtSL)*1000;

		samples[i] = distanceToGroundSeaLevel;
		distanceToGroundSeaLevel = lowPassFilter(samples,SAMPLES);
		if (i == SAMPLES)
		{
			i=0;
		}

		CoEnterMutexSection(distanceToGroundMutex);
		Ftoa(distanceToGround,distanceToGroundVal,5,'f');
		Ftoa(distanceToGroundSeaLevel,distanceToGroundSeaLevelVal,5,'f');
		CoLeaveMutexSection(distanceToGroundMutex);

		CoTimeDelay(0,0,0,250);
	}
}
