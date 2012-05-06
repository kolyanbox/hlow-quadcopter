#include <Tasks/Angle/AngleTask.h>

void AngleTask (void* pdata)
{
	uint16_t value = 0;
	char print[50];
	for(;;)
	{
		value = getCurrentAngle(X);
		Itoa(value, print, 10);
		WriteDebugInfo("X: ");
		WriteDebugInfo(print);
		WriteDebugInfo("\n");

		value = getCurrentAngle(Y);
		Itoa(value, print, 10);
		WriteDebugInfo("Y: ");
		WriteDebugInfo(print);
		WriteDebugInfo("\n");

		value = getCurrentAngle(Z);
		Itoa(value, print, 10);
		WriteDebugInfo("Z: ");
		WriteDebugInfo(print);
		WriteDebugInfo("\n");
		CoTimeDelay(0,0,15,0);
	}
}
