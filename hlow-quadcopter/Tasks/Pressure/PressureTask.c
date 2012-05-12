#include <Tasks/Pressure/PressureTask.h>

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

		CoTimeDelay(0,0,0,100);
	}
}
