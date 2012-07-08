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
		WriteDebugInformation(print,CommandRotationX);
		WriteDebugInfo("\n\r");

		value = getCurrentAngle(Y);
		Itoa(value, print, 10);
		WriteDebugInfo("Y: ");
		WriteDebugInformation(print,CommandRotationY);
		WriteDebugInfo("\n\r");

		value = getCurrentAngle(Z);
		Itoa(value, print, 10);
		WriteDebugInfo("Z: ");
		WriteDebugInformation(print,CommandRotationZ);
		WriteDebugInfo("\n\r");
		CoTimeDelay(0,0,15,0);
	}
}
