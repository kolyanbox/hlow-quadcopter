#include <Tasks/Angle/AngleTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <General/util.h>
void AngleTask (void* pdata)
{
	uint16_t value = 0;
	char print[50];
	for(;;)
	{
		value = getCurrentAngle(X);
		Itoa(value, print, 10);
		WriteDebugInformation(print,CommandRotationX);

		value = getCurrentAngle(Y);
		Itoa(value, print, 10);
		WriteDebugInformation(print,CommandRotationY);

		value = getCurrentAngle(Z);
		Itoa(value, print, 10);
		WriteDebugInformation(print,CommandRotationZ);
		CoTimeDelay(0,0,0,500);
	}
}
