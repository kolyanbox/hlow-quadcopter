#include <Tasks/Pressure/PressureTask.h>

void PressureTask (void* pdata)
{
	for(;;)
	{
		long lvalue = getCurrentPressure();
		int value = (int)lvalue;
		char print[10];
		Itoa(value, print, 10);
		WriteDebugInfo("Pressure: ");
		WriteDebugInfo(print);
		WriteDebugInfo("\n");

		Ftoa(getCurrentPressureAtSeaLevel(80),print,3,'f');
		WriteDebugInfo("Pressure at sea: ");
		WriteDebugInfo(print);
		WriteDebugInfo("\n");

		Ftoa(getCurrentAltitude(),print,3,'f');
		WriteDebugInfo("Pressure at sea: ");
		WriteDebugInfo(print);
		WriteDebugInfo("\n");

		CoTimeDelay(0,0,0,100);
	}
}
