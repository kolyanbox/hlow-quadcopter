#include <Tasks/Angle/AngleTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <Tasks/Logging/LoggingTask.h>
#include <General/util.h>

#include <Drivers/Uart/Uart.h>

char commandGetAngle[] = {"getangle"};
char wrongCommandGetAngle[] = {"Not a valid command!\nParameter shuld contain x,y or z"};

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

char * getAngle(int argc, char *args[])
{
	if (argc != 1)
	{
		return wrongCommandGetAngle;
	}
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
	return wrongCommandGetAngle;
}

void AngleTask (void* pdata)
{
	uint16_t value = 0;
	unsigned char angleApp = 0;

	//register angle app in cli
	registerInterface(commandGetAngle,getAngle);

	unsigned char appName[] = {"Angle task"};
	angleApp = registerApp(appName,LOG_DEBUG);

	for(;;)
	{
		unsigned char logMessage[] = {"inside angletask\n\r"};
		writeLog(angleApp,logMessage,LOG_DEBUG);

		value = (getCurrentAngle(X) - 1148) / 11.10556;
		Itoa(value, Anglex, 10);

		value = (getCurrentAngle(Y) - 1094) / 10.90556;
		Itoa(value, Angley, 10);

		value = getCurrentAngle(Z);
		Itoa(value, Anglez, 10);

		CoTimeDelay(0,0,0,50);
	}
}
