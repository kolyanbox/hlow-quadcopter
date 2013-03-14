#include <Tasks/Controller/ControllerTask.h>
#include <Tasks/Angle/AngleTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <Interfaces/Actuators/Actuators.h>
#include <General/util.h>

char commandStopController[] = {"stopcontroller"};
char commandSetKD[] = {"setkd"};
char commandSetKI[] = {"setki"};
char commandSetKP[] = {"setkp"};
char wrongKDCommand[] = {"error!!"};

#define ControllerStackSize 128
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

float kd = 0.125;
char * setkd(int argc, char *args[])
{
	if (argc != 1)
	{
		return wrongKDCommand;
	}
	kd = Atof(args[0]);

	return "kd is set";
}

float ki = 0.05;
char * setki(int argc, char *args[])
{
	if (argc != 1)
	{
		return wrongKDCommand;
	}
	ki = Atof(args[0]);

	return "ki is set";
}

float kp = 0.6;
char * setkp(int argc, char *args[])
{
	if (argc != 1)
	{
		return wrongKDCommand;
	}
	kp = Atof(args[0]);

	return "kp is set";
}

void ControllerTask (void* pdata)
{
	//register angle app in cli
	registerInterface(commandStopController,stopController);
	registerInterface(commandSetKD,setkd);
	registerInterface(commandSetKI,setki);
	registerInterface(commandSetKP,setkp);


	int speedF = 0;
	int speedB = 0;
	int speedL = 0;
	int speedR = 0;

	int prevOutput = 0;
	int outF = 0;
	int outB = 0;

	float half = 747;


	float previousError = 0;
	float integral = 0;
	float dt = 0.05;


	for(;;)
	{
		char *args[] = {"y"};

		long int input = Strtol(getAngle(1, args), (char **)getAngle(1, args)+4,10) ;

		float setpoint = 90;
		float error = setpoint - input;
		integral = integral + error*dt;
		float derivative = (error - previousError)/dt;
		float output = kp*error+ki*integral+kd*derivative;
		previousError = error;

//		char c[10];
//		Ftoa(output,c,10,'f');
//		WriteDebugInfo(c);
//		WriteDebugInfo(" output\n\r");
//		Ftoa(error,c,10,'f');
//		WriteDebugInfo(c);
//		WriteDebugInfo(" error\n\r");

		//input = input-1317;
		//input = input/14.94;

		//int deviatie = input-90;
		//int output = (deviatie*0.1);

		if (output > 0)
		{
			outF = output;
			outB = 0;
		}
		else
		{
			outB = output*-1;
			outF = 0;
		}
		if (outB > 30)
		{
			outB = 30;
		}
		if (outF > 30)
		{
			outF = 30;
		}

//		Itoa(outB,c,10);
//		WriteDebugInfo(c);
//		WriteDebugInfo(" B\n\r");
//
//		Itoa(outF,c,10);
//		WriteDebugInfo(c);
//		WriteDebugInfo(" F\n\r");

		//Itoa(output,c,10);
		//WriteDebugInfo(c);
		//WriteDebugInfo(" O\n\r");

		//setSpeed Back(speedB);
		//setSpeedFront(speedF);
		setSpeedFront(outF);
		setSpeedBack(outB);
		CoTimeDelay(0,0,0,50);
	}
}
