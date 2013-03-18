#include <Tasks/Main/MainTask.h>

#include <Interfaces/Actuators/Actuators.h>
#include <Drivers/Uart/Uart.h>
#include <CoOS.h>

#include <Tasks/Heartbeat/Heartbeat.h>
#include <Tasks/Distancetoground/DistanceToGroundTask.h>
#include <Tasks/Angle/AngleTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <Tasks/Logging/LoggingTask.h>
#include <Tasks/Speed/SpeedTask.h>
#include <Tasks/Motor/MotorTask.h>
#include <Tasks/Controller/ControllerTask.h>

#include <General/Taskmanager/Taskmanager.h>

#define stackSize 128
OS_STK MainTask_stk[stackSize];
taskDef t;
taskDef getMainTaskDefenition()
{
	t.priority = 63;
	t.stk = &MainTask_stk[stackSize-1];
	t.stkSz = stackSize;
	t.task = MainTask;
	t.taskName = "Main";
	return t;
}

void MainTask (void* pdata)
{
	//Startup sequence. First startup Uart
	//If initialize fails go into infinite loop
	//If initialization of the telemetry succeeds, go on with initializing other parts of the system
	if (ActuatorsInitialization(ActuatorTelemetry) == FALSE)
	{
		while (1);
	}

	//Because the debug task isn't launched we need to write debug info directly
	WriteDebugInfo("Motors will be initialized\n\r/>");
	if (ActuatorsInitialization(ActuatorMotors) == TRUE)
	{
		if (createTask(getMotorTaskDefenition()) ==FALSE)
		{
			WriteDebugInfo("couldn't start motor task!");
			while(1);
		}
		WriteDebugInfo("Motors are initialized.\n\r");
		WriteDebugInfo("Start motor test?\n\r");
		if (isAnswerFromUserYes())
		{
			setSpeedBack(1);
			setSpeedFront(1);
			setSpeedLeft(1);
			setSpeedRight(1);
			WriteDebugInfo("Press y to stop motor test\n\r");
			if (isAnswerFromUserYes())
			{
				setSpeedBack(0);
				setSpeedFront(0);
				setSpeedLeft(0);
				setSpeedRight(0);
			}
		}
	}
	else {
		WriteDebugInfo("Motors couldn't be initialized.\n\r");
		while(1);
	}

	/*Initialize sensors and actuators*/
	WriteDebugInfo("Welcome!\n\r");
	if (initializeSensors() == FALSE)
	{
		WriteDebugInfo("Initialization stopped!\n\r");
		while(1);

	}
	if (initializeActuators() == FALSE)
	{
		WriteDebugInfo("Initialization stopped!\n\r");
		while(1);
	}

	if (createTask(getControllerTaskDefenition()) ==FALSE)
	{
		WriteDebugInfo("couldn't start controller task!\n\r");
		while(1);
	}

	/*Create debug task
	 * After the creation of this task the WriteDebugInfo shouldn't be used*/
	if (DebugTaskInitialization() == FALSE)
	{
		WriteDebugInfo("Debug task couldn't be initialized!\n\r");
		while(1);
	}
	/*clear all received commands*/
	clearLastCommand();


	if (initializeTaskmanager() == FALSE)
	{
		WriteDebugInfo("couldn't initialize taskmanager!");
		while (1);
	}

	/*Start debug task*/
	if (createTask(getDebugTaskDefenition()) ==FALSE)
	{
		WriteDebugInfo("couldn't start debug task!");
		while(1);
	}
	/*start logging task*/
	if (createTask(getLoggingTaskDefenition()) ==FALSE)
	{
		WriteDebugInfo("couldn't start logging task!");
		while(1);
	}

	 for (;;)
	{
		CoTimeDelay(0,0,0,500);
	}
}

/*If a critical sensor fails this method will return FALSE otherwise it will return TRUE*/
Bool initializeSensors()
{
	//Initialize DistancetoGroundSensor
	WriteDebugInfo("Initialize distance to ground sensors? (Y/N)\n\r/>");
	if (isAnswerFromUserYes())
	{
		if (sensorInitialization(SensorDistanceToGround) == TRUE)
		{
			WriteDebugInfo("First distance to ground sensor initialized.\n\r");

			if (sensorInitialization(SensorPressure) == TRUE)
			{
				/*Create distance task*/
				if (createTask(getDistanceTaskDefenition()) ==FALSE)
				{
					WriteDebugInfo("couldn't start distance to ground sensor task!\n\r");
					while(1);
				}
				WriteDebugInfo("Distance to ground sensors are initialized.\n\r/>");
			}
			else {
				WriteDebugInfo("Second distance to ground sensor couldn't be initialized!\n\r");
			}
		}
		else {
			WriteDebugInfo("Distance to ground sensor couldn't be initialized!\n\r");
		}
	}
	//If the answer was no
	else {

		WriteDebugInfo("Distance to ground sensor didn't Initialize!\n\r");
	}

	/*Initialize Angle sensor*/
	WriteDebugInfo("Initalize angle sensor? (Y/N)\n\r/>");
	if (isAnswerFromUserYes())
	{
		if (sensorInitialization(SensorAccelero) == TRUE)
		{
			/*Create angle task*/
			if (createTask(getAngleTaskDefenition()) ==FALSE)
			{
				WriteDebugInfo("couldn't start angle task!");
				while(1);
			}
			WriteDebugInfo("Angle sensor is initialized.\n\r");
		}
		else {
			WriteDebugInfo("Angle sensor couldn't be initialized!\n\r");
		}
	}
	//If the answer was no
	else {

		WriteDebugInfo("Angle sensor didn't Initialize!\n\r");
	}

	/*Initialize Speed sensor*/
	WriteDebugInfo("Initalize speed sensor? (Y/N)\n\r/>");
	if (isAnswerFromUserYes())
	{
		if (sensorInitialization(SensorRotation) == TRUE)
		{
			/*Create angle task*/
			if (createTask(getSpeedTaskDefenition()) ==FALSE)
			{
				WriteDebugInfo("couldn't start speed task!");
				while(1);
			}
			WriteDebugInfo("Speed sensor is initialized.\n\r");
		}
		else {
			WriteDebugInfo("Speed sensor couldn't be initialized!\n\r");
		}
	}
	//If the answer was no
	else {

		WriteDebugInfo("Speed sensor didn't Initialize!\n\r");
	}

	return TRUE;
}

/*If a critical actuator fails this method will return FALSE otherwise it will return TRUE*/
Bool initializeActuators()
{
	//initialize leds
	WriteDebugInfo("Initalize Actuator Leds? (Y/N)\n\r/>");
	if (isAnswerFromUserYes())
	{
		if (ActuatorsInitialization(ActuatorLeds) == TRUE)
		{
			/*Create heartbeat task*/
			if (createTask(getHeartbeatTaskDefenition()) ==FALSE)
			{
				WriteDebugInfo("couldn't start distance task!");
				while(1);
			}
			WriteDebugInfo("Leds are initialized.\n\r");
		}
		else {
			WriteDebugInfo("Leds couldn't be initialized.\n\r");
		}
	}
	//If the answer was no
	else {

		WriteDebugInfo("Leds didn't Initialize!\n\r");
	}

	return TRUE;
}

/*This method waits till a user sends an y or a n to the debug interface*/
Bool isAnswerFromUserYes()
{
	//Wait for command from uart (Y or N)
	unsigned char lastChar = getLastCharacterFromUart();
	char tempArray[1];
	tempArray[0] = lastChar;

	//Waiting for receiving an answer
	while(tempArray[0] != 'N' && tempArray[0] != 'n' &&	tempArray[0] != 'y' && tempArray[0] != 'Y')
	{
		tempArray[0] = getLastCharacterFromUart();
	}
	if (tempArray[0] == 'y' || tempArray[0] == 'Y')
	{
		return TRUE;
	}
	return FALSE;
}
