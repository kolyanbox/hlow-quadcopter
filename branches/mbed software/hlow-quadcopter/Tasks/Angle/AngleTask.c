#include <Tasks/Angle/AngleTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <Tasks/Logging/LoggingTask.h>
#include <General/util.h>
void AngleTask (void* pdata)
{
	uint16_t value = 0;
	char print[50];
	unsigned char angleApp = 0;

	angleApp = registerApp("Angle task",LOG_DEBUG);
    if(angleApp < 1)
    {
       WriteDebugInformation("Couldn't register angle app!", DirectDebug);
    }

	for(;;)
	{
		writeLog(angleApp,"inside angletask",LOG_DEBUG);
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
