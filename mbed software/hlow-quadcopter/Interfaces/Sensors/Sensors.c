#include <Interfaces/Sensors/Sensors.h>

/*common includes*/
#include "lpc17xx_timer.h"
#include "lpc_exti.h"

#include <Drivers/GPIO/GPIO.h>
#include <Drivers/ADC/ADC.h>
#include <Drivers/Ultrasonic/Ultrasonic.h>
#include <Interfaces/Actuators/Actuators.h>
#include <Drivers/I2C/I2C.h>
#include <Drivers/BMP085/BMP085.h>
#include <Drivers/WiiMotionPlus/WiiMotionPlus.h>
#include <CoOS.h>
#include <Drivers/Uart/Uart.h>
#include <Tasks/Debug/DebugTask.h>

/*CurrentTemperatureMutex is protecting the currentTemperature variable*/
OS_MutexID currentTemperatureMutex;
long currentTemperature = 0;

/*CurrentPressureMutex is protecting the currentPressure variable*/
OS_MutexID currentPressureMutex;
long currentPressure = 0;

/*CurrentPressureMutex is protecting the currentPressure variable*/
OS_MutexID PressureAtSeaLevelMutex;
float pressureAtSeaLevel = 0;

OS_MutexID I2CMutex;

Bool firstTimeInitialization = TRUE;
/*True if the this is the first time I2C will be initialized*/
Bool firstTimeInitializeI2CSensor = TRUE;

/*This method returns TRUE if all sensors are correct initialized and FALSE if one sensor fails to initialize*/
Bool sensorInitialization(enum SensorType sensorType)
{
	//If there isn't initialized 1 sensor
	if (firstTimeInitialization == TRUE)
	{
		/*Initialize mutexes*/
		if (currentTemperatureMutex == E_CREATE_FAIL || I2CMutex == E_CREATE_FAIL)
		{
			return FALSE;
		}
		firstTimeInitialization = FALSE;
	}


	switch (sensorType)
	{
		/*Initialize Ultrasonic sensor*/
		case (SensorDistanceToGround):
		{
			return initializeUltrasonicSensor();
		}
		/*Initialize Accelero sensor*/
		case (SensorAccelero):
		{
			if (ADC_init() == FALSE)
			{
				return FALSE;
			}
			return TRUE;
		}
		/*Initialize Pressure Sensor*/
		case (SensorPressure):
		{
			if (firstTimeInitializeI2CSensor == TRUE)
			{
				/*Initialize I2C before initializing I2C based sensors*/
				if (I2C_Initialize() == FALSE)
				{
					return FALSE;
				}
			}
			/*Initialize BMP085*/
			if (initializeBMP085() == FALSE)
			{
				return FALSE;
			}
			return TRUE;
		}
		/*Initialize Rotation sensor*/
		case (SensorRotation):
		{
			if (firstTimeInitializeI2CSensor == TRUE)
			{
				/*Initialize I2C before initializing I2C based sensors*/
				if (I2C_Initialize() == FALSE)
				{
					return FALSE;
				}
			}
			/*Initialize WiiMotionPlus*/
			if (initializeWiiMotionPlus() == FALSE)
			{
				return FALSE;
			}
			return TRUE;
		}
		/*Initialize GPS sensor*/
		case (SensorGPS):
		{
			if (UARTInit(LPC_UART1, 4800) == FALSE)
			{
				return FALSE;
			}
			return TRUE;
		}
		/*If there isn't a sensor selected, return FALSE*/
		default:
		{
			return FALSE;
		}
	}
}

/*Only use this method after calling the gettemperature method max 1 second ago*/
long getCurrentPressure()
{
	long temp = 0;
	CoEnterMutexSection(I2CMutex);
	temp = getPressure();
	CoLeaveMutexSection(I2CMutex);
	CoEnterMutexSection(currentPressureMutex);
	currentPressure = temp;
	CoLeaveMutexSection(currentPressureMutex);
	return temp;
}

float calculateCurrentPressureAtSeaLevel(float currentAltitude)
{
	float t = getTemperature()/10;
	float p = getCurrentPressure()/100;
	float temp = 1-((0.0065*currentAltitude)/(t+(0.0065*currentAltitude)+273.15));
	temp = Pow(temp,-5.257);
	temp = p*temp;

	CoEnterMutexSection(PressureAtSeaLevelMutex);
	pressureAtSeaLevel = temp;
	CoLeaveMutexSection(PressureAtSeaLevelMutex);

	return temp;
}

float getCurrentAltitude()
{
	CoEnterMutexSection(PressureAtSeaLevelMutex);
	float p0 = pressureAtSeaLevel;
	CoLeaveMutexSection(PressureAtSeaLevelMutex);

	float p = getCurrentPressure()/100;
	float temp = Pow((p0/p),(1/5.257))-1;
	float t = getTemperature()/10;
	temp = temp * (t+273.15);
	return temp/0.0065;
}

long getCurrentTemperature()
{
	CoEnterMutexSection(I2CMutex);
	long i = 0;
	while(i<100000)
	{
		i++;
	}
	writeDataBmp085(0xF4, 0x2E);
	//wait 4.5ms
	i = 0;
	while(i<1000000)
	{
		i++;
	}
	long ut = getUtBMP085(0xf6);
	CoTickDelay(300);
	short oss = 0;
	uint8_t transmitData = 0x34;
	transmitData += oss<<6;
	writeDataBmp085(0xF4, transmitData);
	i = 0;
	while(i<1000000)
	{
		i++;
	}
	long up = getUpBMP085(0xf6, oss);

	long temp = getTemperature();
	CoLeaveMutexSection(I2CMutex);
	CoEnterMutexSection(currentTemperatureMutex);
	currentTemperature = temp;
	CoLeaveMutexSection(currentTemperatureMutex);
	return temp;
}

int getCurrentAngle(enum Axle axle)
{
	return getADC(axle);
}

int getCurrentHeightInCm()
{
	return getDistanceToGround();
}

int getRotation()
{
	gyroscope_get();
	return 0;
}

Bool sameString(unsigned char* stringOne, unsigned char* stringTwo)
{
	//int length = Strlen(stringOne);
	//if (length != Strlen(stringTwo))
	//{
	//	return FALSE;
	//}
	int i = 0;
	while (stringOne[i] != '\0' || stringTwo[i] != '\0')
	{
		WriteDebugInfo("in while");
		WriteDebugInfo(stringOne);
		WriteDebugInfo(stringTwo);
		if (stringTwo[i] != stringOne[i])
		{
			if (stringOne[i] != '\0')
			{
				WriteDebugInfo("1 niet 0");
			}
			if (stringOne[i+1] == '\0')
						{
							WriteDebugInfo("1 wel 0");
						}
			WriteDebugInfo("in while");
			return FALSE;
		}
		i++;
	}
	return TRUE;
}

enum Command getCommand()
{
	unsigned char getAngleX[] = {"getanglex"};
	unsigned char getAngleY[] = {"getangley"};
	unsigned char getAngleZ[] = {"getanglez"};
	unsigned char getOsTime[] = {"getostime\0"};
	unsigned char getDistanceToGround[] = {"getdtg"};
	unsigned char getAllCommands0[] = {"?"};
	unsigned char getAllCommands1[] = {"--help"};
	unsigned char getAllCommands2[] = {"--h"};
	unsigned char getStatusAllTasks[] = {"getstatustasks"};
	unsigned char stopTaskWithId[] = {"stoptask"};
	unsigned char* lrc = lastReceivedCommand();

	char c2[10];
	Itoa(Strlen(lrc),c2,10);
	WriteDebugInfo(c2);
	WriteDebugInfo("<-\n\r");
	Itoa(Strlen(stopTaskWithId),c2,10);
	WriteDebugInfo(c2);
	WriteDebugInfo(lrc);
	WriteDebugInfo("<-\n\r");

	//check if there are spaces in lrc
	int i;
	int spacePosition[COMMANDLENGTH];
	int amountOfSpaces = 0;
	int lengthLrc = Strlen(lrc);
	for (i=0;i<lengthLrc;i++)
	{
		if (lrc[i] == ' ')
		{
			spacePosition[amountOfSpaces] = i;
			amountOfSpaces++;
		}
	}
	//get first command
	unsigned char firstCommand[spacePosition[0]+1];
	if (amountOfSpaces > 0)
	{
		int j;
		for (j=0;j<spacePosition[0];j++)
		{
			firstCommand[j] = lrc[j];
		}
		firstCommand[j] = '\0';
	}

	if (sameString(getAngleX,lrc))
	{
		return CommandRotationX;
	}
	else if (sameString(getAngleY,lrc))
	{
		return CommandRotationY;
	}
	else if (sameString(getAngleZ,lrc))
	{
		return CommandRotationZ;
	}
	else if (sameString(getOsTime,lrc))
	{
		return commandOsTime;
	}
	else if (sameString(getDistanceToGround,lrc))
	{
		return commandDistanceToGround;
	}
	else if (sameString(getAllCommands0,lrc) || sameString(getAllCommands1,lrc) || sameString(getAllCommands2,lrc))
	{
		return CommandHelp;
	}
	else if (sameString(getStatusAllTasks,lrc))
	{
		return CommandAllTaskStatus;
	}
	else if (sameString(stopTaskWithId,firstCommand))
	{
		//get fisrt parameter
		unsigned char firstParam[spacePosition[1]-(spacePosition[0])];
		if (amountOfSpaces > 1)
		{
			int j;
			for (j=spacePosition[1]+1;j<spacePosition[1]-(spacePosition[0]);j++)
			{
				firstParam[j-spacePosition[0]+1] = lrc[j-spacePosition[0]];
			}
			firstParam[j-spacePosition[0]] = '\0';
		}
		char c[10];
		Itoa(spacePosition[1],c,10);
		getFirstParameter(firstParam);
		return CommandStopTaskWithId;
	}
	else if (lrc[0] != '\0'){
		WriteDebugInformation(lrc,DirectDebug);
		WriteDebugInformation("Not a valid command.\n\r/>", DirectDebug);
		clearLastCommand();
	}
	return CommandNoCommand;
}

//Only use this method in startup sequence
unsigned char getLastCharacterFromUart()
{
	unsigned char tempChar = lastReceivedChar;
	lastReceivedChar = 0;
	return tempChar;
}
