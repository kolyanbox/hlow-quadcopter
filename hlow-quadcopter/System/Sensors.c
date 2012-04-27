#include <System/Sensors.h>

OS_MutexID AccelleroMutex;

Bool SensorInit()
{
	if (ADC_init() != 0)
	{
		return FALSE;
	}

	AccelleroMutex = CoCreateMutex();
	if (AccelleroMutex == E_CREATE_FAIL)
	{
		return FALSE;
	}
	return TRUE;
}

Bool getValue(enum SENSOR Sensor, int ValueNumber, void *Value)
{
	switch (Sensor)
	{
	case accelero:
		return getAcceleroValue(ValueNumber, Value);
		break;
	case distanceToGround:
		return getDistanceToGround(ValueNumber, Value);
	default:
		return FALSE;
	}
}

Bool getAcceleroValue(int ValueNumber, uint16_t *Value)
{
	CoEnterMutexSection (AccelleroMutex);
	*Value = getADC(ValueNumber);
	CoLeaveMutexSection (AccelleroMutex);
	return TRUE;
}

Bool getDistanceToGround(int ValueNumber, float *Value)
{
	startMeasureDistanceToGround();

	*Value = getDistance();
	return TRUE;
}
