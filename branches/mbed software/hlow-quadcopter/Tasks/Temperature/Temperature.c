#include <Tasks/Temperature/Temperature.h>
#include <General/util.h>

void TemperatureTask (void* pdata)
{
	for(;;)
	{
		long lvalue = getCurrentTemperature();
		int value = (int)lvalue;
		char print[10];
		Itoa(value, print, 10);
		WriteDebugInformation("Temperature: ",DirectDebug);
		WriteDebugInformation(print,Temperature);
		WriteDebugInformation("\n",DirectDebug);
		CoTimeDelay(0,0,0,100);
	}
}
