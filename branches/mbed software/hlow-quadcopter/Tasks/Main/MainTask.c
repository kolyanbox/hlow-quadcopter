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
	//Startup sequence. First statup Uart
	if (ActuatorsInitialization(ActuatorUart) == FALSE)
	{
		//Because there isn't an other actuator initialized you haven't debug info
		while (1);
	}
	//Because the debug task isn't launched we need to wite debug info directly
	WriteDebugInfo("Welcome!\nInitalize Actuator Leds? (Y/N)\n");
	//Wait for command from uart (Y or N)
	unsigned char lastChar = getLastCharacterFromUart();
	char tempArray[1];
	tempArray[0] = lastChar;
	WriteDebugInfo(tempArray);

	if (ActuatorsInitialization(ActuatorLeds) == TRUE)
			{
				WriteDebugInfo("jow4");
			}

	while(tempArray[0] != 'N' &&
			tempArray[0] != 'n' &&
			tempArray[0] != 'y' &&
			tempArray[0] != 'Y')
	{
		lastChar = getLastCharacterFromUart();

	}
	WriteDebugInfo("jow5");
	if (lastChar == 'y' || lastChar == 'Y')
	{
		if (ActuatorsInitialization(ActuatorLeds) == TRUE)
		{
			WriteDebugInfo("jow4");
		}
		//else {
			{WriteDebugInfo("Initialization stopped! Leds couldn't be initialized.");
			while (1);
		}
	}
	else {
		WriteDebugInfo("jow");
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

	/*Create heartbeat task*/
	CoCreateTask (HeartBeat,0,63,&HeartBeat_stk[HeartBeatStackSize-1],HeartBeatStackSize);

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
