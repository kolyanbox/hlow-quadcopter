#include <Tasks/Angle/AngleTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <Tasks/Logging/LoggingTask.h>
#include <General/util.h>

const char commandx[] = {"getanglex"};
const char commandy[] = {"getangley"};
const char commandz[] = {"getanglez"};

char * printInfoAngleX()
{
	static char bla[] = {"Value of angle x: "};
	return bla;
}
char * printInfoAngleY()
{
	static char bla[] = {"Value of angle y: "};
	return bla;
}
char * printInfoAngleZ()
{
	static char bla[] = {"Value of angle z: "};
	return bla;
}

void AngleTask (void* pdata)
{
	uint16_t value = 0;
	char Anglex[10];
	char Angley[10];
	char Anglez[10];
	unsigned char angleApp = 0;

	//register angle app in cli
	registerInterface(commandx,printInfoAngleX);
	registerInterface(commandy,printInfoAngleY);
	registerInterface(commandz,printInfoAngleZ);

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
		WriteDebugInformation(Anglex,AngleX);

		value = getCurrentAngle(Y);
		Itoa(value, Angley, 10);
		WriteDebugInformation(Angley,AngleY);

		value = getCurrentAngle(Z);
		Itoa(value, Anglez, 10);
		WriteDebugInformation(Anglez,AngleZ);
		CoTimeDelay(0,0,0,500);
	}
}
