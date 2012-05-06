#include <Tasks/Temperature/Temperature.h>

void TemperatureTask (void* pdata)
{
	for(;;)
	{
		long lvalue = getCurrentTemperature();
		int value = (int)lvalue;
		char print[10];
		Itoa(value, print, 10);
		WriteDebugInfo("Temperature: ");
		WriteDebugInfo(print);
		WriteDebugInfo("\n");
		CoTimeDelay(0,0,0,100);
	}
}
