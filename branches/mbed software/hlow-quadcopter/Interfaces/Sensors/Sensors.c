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
	long up = getUpBMP085(0xf6);

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
	long temp = getTemperature();
	char c[10];
	Itoa(temp,c,10);
	WriteDebugInfo(c);
	WriteDebugInfo("\n");
	temp = getPressure();
	Itoa(temp,c,10);
	WriteDebugInfo(c);
	WriteDebugInfo("\n");

	return getDistanceToGround();
}

float getRotationAroundAxle(enum Axle axle)
{
	gyroscope_get();
	return gyroscope_get_value(axle);
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
//		WriteDebugInfo("in while");
//		WriteDebugInfo(stringOne);
//		WriteDebugInfo(stringTwo);
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
//Only use this method in startup sequence
unsigned char getLastCharacterFromUart()
{
	unsigned char tempChar = lastReceivedChar;
	lastReceivedChar = 0;
	return tempChar;
}
