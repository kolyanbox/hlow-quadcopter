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
	if (ActuatorsInitialization(ActuatorUart) == FALSE)
	{
		//Because there isn't an other actuator initialized you haven't debug info
		while (1);
	}
	//Because the debug task isn't launched we need to write debug info directly
	WriteDebugInfo("Welcome!\nInitalize Actuator Leds? (Y/N)\n");

	//Wait for command from uart (Y or N)
	unsigned char lastChar = getLastCharacterFromUart();
	char tempArray[1];
	tempArray[0] = lastChar;
	WriteDebugInfo(tempArray);

	while(tempArray[0] != 'N' && tempArray[0] != 'n' &&	tempArray[0] != 'y' && tempArray[0] != 'Y')
	{
		tempArray[0] = getLastCharacterFromUart();
	}

	if (tempArray[0] == 'y' || tempArray[0] == 'Y')
	{
		if (ActuatorsInitialization(ActuatorLeds) == TRUE)
		{
			/*Create heartbeat task*/
			CoCreateTask (HeartBeat,0,63,&HeartBeat_stk[HeartBeatStackSize-1],HeartBeatStackSize);
			WriteDebugInfo("Leds are initialized.\n");
		}
		else {
			WriteDebugInfo("Initialization stopped! Leds couldn't be initialized.\n");
			while (1);
		}
	}
	else {

		WriteDebugInfo("Leds didn't Initialize!\n");
	}


	//Because the debug task isn't launched we need to write debug info directly
	WriteDebugInfo("Initialize Actuator Motors? (Y/N)\n");
	lastChar = getLastCharacterFromUart();
	tempArray[0] = lastChar;
	while(tempArray[0] != 'N' && tempArray[0] != 'n' &&	tempArray[0] != 'y' && tempArray[0] != 'Y')
	{
		tempArray[0] = getLastCharacterFromUart();
	}

	if (tempArray[0] == 'y' || tempArray[0] == 'Y')
	{
		if (ActuatorsInitialization(ActuatorMotors) == TRUE)
		{
			WriteDebugInfo("Motors are initialized.\n");
		}
		else {
			WriteDebugInfo("Initialization stopped! Motors couldn't be initialized.\n");
			while (1);
		}
	}
	else {

		WriteDebugInfo("Motors didn't Initialize!\n");
	}


	osTimeSem = CoCreateSem(1,1,EVENT_SORT_TYPE_FIFO);
	if (osTimeSem == E_CREATE_FAIL)
	{
		setLed(led4,ENABLE);
		while(1);
	}
	CoPostSem(osTimeSem);

	/*Create debug task*/
	if (DebugTaskInitialization() == FALSE)
	{
		setLed(led4,ENABLE);
		while(1);
	}
	CoCreateTask(DebugTask,0,63,&Debug_stk[DebugStackSize-1],DebugStackSize);

	/*Create DistanceToGroundTask task*/
	CoCreateTask (DistanceToGroundTask,0,63,&DistanceToGround_stk[DistancToGroundStackSize-1],DistancToGroundStackSize);

	/*Create AngleTask task*/
	//CoCreateTask (AngleTask,0,63,&Angle_stk[AngleStackSize-1],AngleStackSize);

	/*Create Temperature task*/
	//CoCreateTask (TemperatureTask,0,63,&Temperature_stk[TemperatureStackSize-1],TemperatureStackSize);

	/*Create Pressure task*/
	//CoCreateTask (PressureTask,0,63,&Pressure_stk[PressureStackSize-1],PressureStackSize);
	for (;;)
	{
		/*Give a status update every minute*/
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

		CoTimeDelay(0,1,0,0);
	}
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
