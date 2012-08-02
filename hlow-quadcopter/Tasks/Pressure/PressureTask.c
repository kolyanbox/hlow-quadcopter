#include <Tasks/Pressure/PressureTask.h>
#include <General/util.h>

#include <Interfaces/Sensors/Sensors.h>
#include <Drivers/Uart/Uart.h>

#define PressureStackSize 128
OS_STK	Pressure_stk[PressureStackSize];

taskDef t;
taskDef getPressureTaskDefenition()
{
	t.priority = 63;
	t.stk = &Pressure_stk[PressureStackSize-1];
	t.stkSz = PressureStackSize;
	t.task = PressureTask;
	t.taskName = "Pressure";
	return t;
}

void PressureTask (void* pdata)
{
	long lvalue = getCurrentPressure();
	int value = (int)lvalue;
	char print[10];
	Itoa(value, print, 10);
	WriteDebugInfo("Pressure: ");
	WriteDebugInfo(print);
	WriteDebugInfo("\n");

	Ftoa(calculateCurrentPressureAtSeaLevel(38),print,3,'f');
	WriteDebugInfo("Pressure at sea: ");
	WriteDebugInfo(print);
	WriteDebugInfo("\n");
	for(;;)
	{
		Ftoa(getCurrentAltitude(),print,3,'f');
		WriteDebugInfo("Current altitude: ");
		WriteDebugInfo(print);
		WriteDebugInfo("\n");

		lvalue = getCurrentPressure();
		value = (int)lvalue;

		Itoa(value, print, 10);
		WriteDebugInfo("Pressure: ");
		WriteDebugInfo(print);
		WriteDebugInfo("\n");

		CoTimeDelay(0,0,1,0);
	}
}
