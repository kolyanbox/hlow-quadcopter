#include <Tasks/Speed/SpeedTask.h>
#include <General/util.h>
#include <CoOS.h>
#include <Tasks/Debug/DebugTask.h>
#include <Drivers/I2C/I2C.h>
#include <Drivers/WiiMotionPlus/WiiMotionPlus.h>
const char commandSpeed[] = {"getspeed"};
const char wrongCommandSpeed[] = {"Not a valid command!"};

float Speedx;
float Speedy;
float Speedz;
char retVal[15];
#define SpeedStackSize 64
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
		return "Unknown amount of parameters";
	}
	char x[] = {"x"};
	char y[] = {"y"};
	char z[] = {"z"};
	if (Strcmp(args[0],x) == 0)
	{
		Ftoa(Speedx, retVal,5,'f');
		return retVal;
	}
	else if (Strcmp(args[0],y) == 0)
	{
		Ftoa(Speedy, retVal,5,'f');
		return retVal;
	}
	else if (Strcmp(args[0],z) == 0)
	{
		Ftoa(Speedz, retVal,5,'f');
		return retVal;
	}
	return wrongCommandSpeed;
}

void SpeedTask (void* pdata)
{
	//register angle app in cli
	registerInterface(commandSpeed,printInfoSpeed);
	float tempval;
	for(;;)
	{
		gyroscope_get();
		tempval = gyroscope_get_value(0);
		Speedx += tempval;
		tempval = gyroscope_get_value(1);
		Speedy += tempval;
		tempval = gyroscope_get_value(2);
		Speedz +=tempval;

		CoTimeDelay(0,0,0,10);
	}
}
