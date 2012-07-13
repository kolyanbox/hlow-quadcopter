#include <Tasks/Main/MainTask.h>

OS_EventID osTimeSem;
int oStime = 0;

#define HeartBeatStackSize 64
OS_STK	DistanceToGround_stk[HeartBeatStackSize];

#define DistancToGroundStackSize 64
OS_STK	HeartBeat_stk[DistancToGroundStackSize];

#define AngleStackSize 64
OS_STK	Angle_stk[AngleStackSize];

#define TemperatureStackSize 128
OS_STK	Temperature_stk[TemperatureStackSize];

#define PressureStackSize 128
OS_STK	Pressure_stk[PressureStackSize];

#define DebugStackSize 128
OS_STK	Debug_stk[DebugStackSize];

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
	clearLastCommand();
	CoCreateTask(DebugTask,0,63,&Debug_stk[DebugStackSize-1],DebugStackSize);

	/*	osTimeSem = CoCreateSem(1,1,EVENT_SORT_TYPE_FIFO);
	if (osTimeSem == E_CREATE_FAIL)
	{
		setLed(led4,ENABLE);
		while(1);
	}
	CoPostSem(osTimeSem);

	 * */
	for (;;)
	{
		/*Give a status update every minute
		setOsTime(CoGetOSTime());
		char c[10];

		int osVersion = CoGetOSVersion();
		Itoa(osVersion,c,10);
		WriteDebugInformation("OsVersion: ",Other);
		WriteDebugInformation(c,Other);
		WriteDebugInformation("\n",Other);

		int curTaskId = CoGetCurTaskID();
		Itoa(curTaskId,c,10);
		WriteDebugInformation("CurTaskId: ",Other);
		WriteDebugInformation(c,Other);
		WriteDebugInformation("\n",Other);
*/
		CoTimeDelay(0,1,0,0);
	}
}

/*If a critical sensor fails this method will return FALSE otherwise it will return TRUE*/
Bool initializeSensors()
{
	//Initialize DistancetoGroundSensor
	WriteDebugInfo("Initalize distance to ground sensor? (Y/N)\n\r");
	if (isAnswerFromUserYes())
	{
		if (sensorInitialization(SensorDistanceToGround) == TRUE)
		{
			/*Create heartbeat task*/
			CoCreateTask (DistanceToGroundTask,0,63,&DistanceToGround_stk[DistancToGroundStackSize-1],DistancToGroundStackSize);
			WriteDebugInfo("Distance to ground sensor is initialized.\n\r");
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
	WriteDebugInfo("Initalize angle sensor? (Y/N)\n\r");
	if (isAnswerFromUserYes())
	{
		if (sensorInitialization(SensorAccelero) == TRUE)
		{
			/*Create heartbeat task*/
			CoCreateTask (AngleTask,0,63,&Angle_stk[AngleStackSize-1],AngleStackSize);
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
	WriteDebugInfo("Initalize Actuator Leds? (Y/N)\n\r");
	if (isAnswerFromUserYes())
	{
		if (ActuatorsInitialization(ActuatorLeds) == TRUE)
		{
			/*Create heartbeat task*/
			CoCreateTask (HeartBeat,0,63,&HeartBeat_stk[HeartBeatStackSize-1],HeartBeatStackSize);
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
	WriteDebugInfo("Initialize Actuator Motors? (Y/N)\n\r");
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

void setOsTime (int osTime)
{
	if (CoPendSem(osTimeSem,0) == E_OK){
		oStime = osTime;
		CoPostSem(osTimeSem);
	}
}
int getOsTime ()
{
	int temposTime = 0;
	if (CoPendSem(osTimeSem,0) == E_OK){
		temposTime = oStime;
		CoPostSem(osTimeSem);
	}
	return temposTime;
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
