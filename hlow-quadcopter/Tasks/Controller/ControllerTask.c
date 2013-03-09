#include <Tasks/Controller/ControllerTask.h>
#include <Tasks/Angle/AngleTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <Interfaces/Actuators/Actuators.h>
#include <General/util.h>

char commandStopController[] = {"stopcontroller"};

#define ControllerStackSize 64
OS_STK	Controller_stk[ControllerStackSize];

taskDef t;
taskDef getControllerTaskDefenition()
{
	t.priority = 63;
	t.stk = &Controller_stk[ControllerStackSize-1];
	t.stkSz = ControllerStackSize;
	t.task = ControllerTask;
	t.taskName = "Controller";
	return t;
}

char * stopController(int argc, char *args[])
{
	return "Controller stopped";
}

void ControllerTask (void* pdata)
{
	//register angle app in cli
	registerInterface(commandStopController,stopController);

	int speedF = 0;
	int speedB = 0;
	int speedL = 0;
	int speedR = 0;

	int prevOutput = 0;
	int outL = 0;
	int outR = 0;

	float half = 747;
	for(;;)
	{
		char *args[] = {"y"};

		long int input = Strtol(getAngle(1, args), (char **)getAngle(1, args)+4,10) ;
		input = input-1317;
		input = input/14.94;
		int deviatie = input-50;
		int output = (deviatie*0.1);

		if (output < 0)
		{
			outL = output*-1+18;
			outR = 0;
		}
		else
		{
			outR = output+23;
			outL = 0;
		}

		char c[10];
		Itoa(outR,c,10);
		//WriteDebugInfo(c);
		//WriteDebugInfo(" R\n\r");

		Itoa(outL,c,10);
		//WriteDebugInfo(c);
		//WriteDebugInfo(" L\n\r");

		Itoa(output,c,10);
		//WriteDebugInfo(c);
		//WriteDebugInfo(" O\n\r");

		//setSpeedBack(speedB);
		//setSpeedFront(speedF);
		//setSpeedLeft(outL);
		//setSpeedRight(outR);
		CoTimeDelay(0,0,0,500);
	}
}
