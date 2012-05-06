#include <Interfaces/Sensors/Sensors.h>

/*CurrentTemperatureMutex is protecting the currentTemperature variable*/
OS_MutexID currentTemperatureMutex;
long currentTemperature = 0;

/*CurrentPressureMutex is protecting the currentPressure variable*/
OS_MutexID currentPressureMutex;
long currentPressure = 0;

OS_MutexID I2CMutex;

/*This method returns TRUE if all sensors are correct initialized and FALSE if one sensor fails to initialize*/
Bool sensorInitialization(void)
{
	/*Initialize mutexes*/
	if (currentTemperatureMutex == E_CREATE_FAIL || I2CMutex == E_CREATE_FAIL)
	{
		return FALSE;
	}

	/*Initialize Ultrasonic sensor*/
	if (initializeUltrasonicSensor() == FALSE)
	{
		return FALSE;
	}
	/*Initialize Telecommand(pinout: p9, p10)*/
	if (UARTInit(LPC_UART3, 9600) == FALSE)
	{
		return FALSE;
	}
	/*Initialize Accelero*/
	if (ADC_init() == FALSE)
	{
		return FALSE;
	}
	/*Initialize I2C before initializing I2C based sensors*/
	if (I2C_Initialize() == FALSE)
	{
		return FALSE;
	}
	/*Initialize BMP085*/
	if (initializeBMP085() == FALSE)
	{
		return FALSE;
	}
	/*Initialize WiiMotionPlus*/
	if (initializeWiiMotionPlus() == FALSE)
	{
		return FALSE;
	}
	return TRUE;
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
