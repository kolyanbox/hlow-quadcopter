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
		CoTimeDelay(0,0,0,100);
	}
}
