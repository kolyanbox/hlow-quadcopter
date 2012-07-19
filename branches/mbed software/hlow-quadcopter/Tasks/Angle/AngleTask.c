#include <Tasks/Angle/AngleTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <Tasks/Logging/LoggingTask.h>
#include <General/util.h>

#include <Drivers/Uart/Uart.h>

const char commandx[] = {"getanglex"};
const char commandy[] = {"getangley"};
const char commandz[] = {"getanglez"};
const char command[] = {"getangle"};
const char wrongCommand[] = {"Not a valid command!"};

char Anglex[5];
char Angley[5];
char Anglez[5];

#define AngleStackSize 64
OS_STK	Angle_stk[AngleStackSize];

taskDef t;
taskDef getAngleTaskDefenition()
{
	t.priority = 63;
	t.stk = &Angle_stk[AngleStackSize-1];
	t.stkSz = AngleStackSize;
	t.task = AngleTask;
	t.taskName = "Angle";
	return t;
}

char * printInfoAngle(char *args[])
{
	char x[] = {"x"};
	char y[] = {"y"};
	char z[] = {"z"};
	if (Strcmp(args[0],x) == 0)
	{
		return Anglex;
	}
	else if (Strcmp(args[0],y) == 0)
	{
		return Angley;
	}
	else if (Strcmp(args[0],z) == 0)
	{
		return Anglez;
	}
	return wrongCommand;
}

void AngleTask (void* pdata)
{
	uint16_t value = 0;
	unsigned char angleApp = 0;

	//register angle app in cli
	registerInterface(command,printInfoAngle);


	//angleApp = registerApp("Angle task",LOG_DEBUG);
    //if(angleApp < 1)
    //{
    //   WriteDebugInformation("Couldn't register angle app!\n\r", DirectDebug);
    //}

	for(;;)
	{
		//writeLog(angleApp,"inside angletask\n\r",LOG_DEBUG);

		value = getCurrentAngle(X);
		Itoa(value, Anglex, 10);

		value = getCurrentAngle(Y);
		Itoa(value, Angley, 10);

		value = getCurrentAngle(Z);
		Itoa(value, Anglez, 10);

		CoTimeDelay(0,0,0,500);
	}
}
