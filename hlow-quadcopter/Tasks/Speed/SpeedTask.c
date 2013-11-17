#include <Tasks/Speed/SpeedTask.h>
#include <General/util.h>
#include <CoOS.h>
#include <Tasks/Debug/DebugTask.h>
#include <Interfaces/Sensors/Sensors.h>
char commandSpeed[] = {"getspeed"};
char wrongCommandSpeed[] = {"Not a valid command!\ncommand should have value x, y or z."};

float speed[3];
char retVal[15];
#define SpeedStackSize 128
OS_STK	Speed_stk[SpeedStackSize];

taskDef t;
taskDef getSpeedTaskDefenition()
{
	t.priority = 63;
	t.stk = &Speed_stk[SpeedStackSize-1];
	t.stkSz = SpeedStackSize;
	t.task = SpeedTask;
	t.taskName = "Speed";
	return t;
}

char * printInfoSpeed(int argc, char *args[])
{
	if (argc != 1)
	{
		return wrongCommandSpeed;
	}
	char x[] = {"x"};
	char y[] = {"y"};
	char z[] = {"z"};
	if (Strcmp(args[0],x) == 0)
	{
		Ftoa(speed[X], retVal,5,'f');
		return retVal;
	}
	else if (Strcmp(args[0],y) == 0)
	{
		Ftoa(speed[Y], retVal,5,'f');
		return retVal;
	}
	else if (Strcmp(args[0],z) == 0)
	{
		Ftoa(speed[Z], retVal,5,'f');
		return retVal;
	}
	return wrongCommandSpeed;
}

void SpeedTask (void* pdata)
{
	//register angle app in cli
	registerInterface(commandSpeed,printInfoSpeed);

	for(;;)
	{
		float *f = getRotationAroundAxle();

		speed[X] = f[X];
		speed[Y] = f[Y];
		speed[Z] = f[Z];

		char speedChar[5];
		Ftoa(speed[X],speedChar,1,'f');
		WriteDebugInfo(speedChar);
		WriteDebugInfo(",");

		Ftoa(speed[Y],speedChar,1,'f');
		WriteDebugInfo(speedChar);
		WriteDebugInfo(",");

		Ftoa(speed[Z],speedChar,1,'f');
		WriteDebugInfo(speedChar);
		WriteDebugInfo("\r\n");


		CoTimeDelay(0,0,0,50);
	}
}
