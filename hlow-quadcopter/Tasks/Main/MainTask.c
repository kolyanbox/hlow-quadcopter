#include <Tasks/Main/MainTask.h>

#include <Interfaces/Actuators/Actuators.h>
#include <Drivers/Uart/Uart.h>
#include <CoOS.h>

#include <Tasks/Heartbeat/Heartbeat.h>
#include <Tasks/Distancetoground/DistanceToGroundTask.h>
#include <Tasks/Angle/AngleTask.h>
#include <Tasks/Temperature/Temperature.h>
#include <Tasks/Pressure/PressureTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <Tasks/Logging/LoggingTask.h>

#include <General/Taskmanager/Taskmanager.h>

#define HeartBeatStackSize 64
OS_STK	DistanceToGround_stk[HeartBeatStackSize];
taskDef distanceToGroundTask;

#define DistancToGroundStackSize 64
OS_STK	HeartBeat_stk[DistancToGroundStackSize];
taskDef heartbeatTask;

#define AngleStackSize 64
OS_STK	Angle_stk[AngleStackSize];
taskDef angleTask;

#define TemperatureStackSize 128
OS_STK	Temperature_stk[TemperatureStackSize];
taskDef temperatureTask;

#define PressureStackSize 128
OS_STK	Pressure_stk[PressureStackSize];
taskDef pressureTask;

#define DebugStackSize 256
OS_STK	Debug_stk[DebugStackSize];
taskDef debugTask;

#define LoggingStackSize 128
OS_STK	Logging_stk[LoggingStackSize];
taskDef loggingTask;

void MainTask (void* pdata)
{
	//Startup sequence. First startup Uart
	//If initialize fails go into infinite loop
	//If initialization of the telemetry succeeds, go on with initializing other parts of the system
	if (ActuatorsInitialization(ActuatorTelemetry) == FALSE)
	{
		while (1);
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
	debugTask.priority = 63;
	debugTask.stk = &Debug_stk[DebugStackSize-1];
	debugTask.stkSz = DebugStackSize;
	debugTask.task = DebugTask;
	debugTask.taskName = "Debug";
	if (createTask(debugTask) ==FALSE)
	{
		WriteDebugInfo("couldn't start debug task!");
		while(1);
	}
	/*start logging task*/
	loggingTask.priority = 63;
	loggingTask.stk = &Logging_stk[LoggingStackSize-1];
	loggingTask.stkSz = LoggingStackSize;
	loggingTask.task = LoggingTask;
	loggingTask.taskName = "Logging";
	if (createTask(loggingTask) ==FALSE)
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
	WriteDebugInfo("Initalize distance to ground sensor? (Y/N)\n\r/>");
	if (isAnswerFromUserYes())
	{
		if (sensorInitialization(SensorDistanceToGround) == TRUE)
		{
			/*Create distance task*/
			distanceToGroundTask.priority = 63;
			distanceToGroundTask.stk = &DistanceToGround_stk[DistancToGroundStackSize-1];
			distanceToGroundTask.stkSz = DistancToGroundStackSize;
			distanceToGroundTask.task = DistanceToGroundTask;
			distanceToGroundTask.taskName = "distance";
			if (createTask(distanceToGroundTask) ==FALSE)
			{
				WriteDebugInfo("couldn't start logging task!");
				while(1);
			}
			WriteDebugInfo("Distance to ground sensor is initialized.\n\r/>");
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
			angleTask.priority = 63;
			angleTask.stk = &Angle_stk[AngleStackSize-1];
			angleTask.stkSz = AngleStackSize;
			angleTask.task = AngleTask;
			angleTask.taskName = "angle";
			if (createTask(angleTask) ==FALSE)
			{
				WriteDebugInfo("couldn't start distance task!");
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
			heartbeatTask.priority = 63;
			heartbeatTask.stk = &HeartBeat_stk[HeartBeatStackSize-1];
			heartbeatTask.stkSz = HeartBeatStackSize;
			heartbeatTask.task = HeartBeat;
			heartbeatTask.taskName = "heartbeat";
			if (createTask(heartbeatTask) ==FALSE)
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

	//Because the debug task isn't launched we need to write debug info directly
	WriteDebugInfo("Initialize Actuator Motors? (Y/N)\n\r/>");
	if (isAnswerFromUserYes() == TRUE)
	{
		if (ActuatorsInitialization(ActuatorMotors) == TRUE)
		{
			WriteDebugInfo("Motors are initialized.\n\r");
		}
		else {
			WriteDebugInfo("Motors couldn't be initialized.\n\r");
			return FALSE;
		}
	}
	else {

		WriteDebugInfo("Motors didn't Initialize!\n\r");
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
