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
		I2CMutex = CoCreateMutex();
		currentTemperatureMutex = CoCreateMutex();
		currentPressureMutex = CoCreateMutex();
		PressureAtSeaLevelMutex = CoCreateMutex();
		if (currentTemperatureMutex == E_CREATE_FAIL || I2CMutex == E_CREATE_FAIL
				|| currentPressureMutex == E_CREATE_FAIL || PressureAtSeaLevelMutex == E_CREATE_FAIL)
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

/**
 * Only use this method after calling the gettemperature method max 1 second ago
 * this method returns the pressure in hPa*100
 * */
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
	float A = 1-(currentAltitude/44330);
	float B = Pow(A,5.255);
	CoEnterMutexSection(I2CMutex);
	getTemperature();
	float p = getPressure();
	CoLeaveMutexSection(I2CMutex);
	return p/100/B;
}

/**
 * If pressureAtSea is set to -1 the pressure calculated by the method
 * calculateCurrentPressureAtSeaLevel will be used
 */
float getCurrentAltitude(float pressureatSea)
{
	float pressure;
	if (pressureatSea == -1)
	{
		CoEnterMutexSection(PressureAtSeaLevelMutex);
		pressure = pressureAtSeaLevel;
		CoLeaveMutexSection(PressureAtSeaLevelMutex);
	}
	else
	{
		pressure = pressureatSea;
	}
	float A = ((float)getCurrentPressure()/100)/pressure;
	float B = 1/5.25588;
	float C = Pow(A,B);
	C = 1 - C;
	C = C /0.0000225577;

	return C;
}

/**
 * returns temperature in degrees celcius * 10
 */
long getCurrentTemperature()
{
	CoEnterMutexSection(I2CMutex);
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
	char c[10];
	float t = calculateCurrentPressureAtSeaLevel(34.3);

	Ftoa(t,c,2,'f');
	WriteDebugInfo("presure at sea: ");
	WriteDebugInfo(c);
	WriteDebugInfo("\n");

	//pressureAtSeaLevel = 1023.8;
	t = getCurrentAltitude(t);

	Ftoa(t,c,2,'f');
	WriteDebugInfo("altitude: ");
	WriteDebugInfo(c);
	WriteDebugInfo("\n");

	long temp = getCurrentTemperature();

	Itoa(temp,c,10);
	WriteDebugInfo(c);
	WriteDebugInfo("\n");
	CoEnterMutexSection(I2CMutex);
	temp = getPressure();
	CoLeaveMutexSection(I2CMutex);
	Itoa(temp,c,10);
	WriteDebugInfo(c);
	WriteDebugInfo("\n");


	return getDistanceToGround();
}

float getRotationAroundAxle(enum Axle axle)
{
	CoEnterMutexSection(I2CMutex);
	gyroscope_get();
	float f = gyroscope_get_value(axle);
	CoLeaveMutexSection(I2CMutex);
	return f;
}

//Only use this method in startup sequence
unsigned char getLastCharacterFromUart()
{
	unsigned char tempChar = lastReceivedChar;
	lastReceivedChar = 0;
	return tempChar;
}
