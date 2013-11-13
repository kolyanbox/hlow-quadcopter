#include <Tasks/Battery/BatteryTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <Tasks/Logging/LoggingTask.h>
#include <General/util.h>

char commandGetBattery[] = {"getbattery"};

uint16_t battery;

#define BatteryStackSize 128
OS_STK	Battery_stk[BatteryStackSize];

taskDef t;
taskDef getBatteryTaskDefenition()
{
	t.priority = 63;
	t.stk = &Battery_stk[BatteryStackSize-1];
	t.stkSz = BatteryStackSize;
	t.task = BatteryTask;
	t.taskName = "Battery";
	return t;
}

char * getBattery(int argc, char *args[])
{
	//3936 = 100% = 4,17V
	char c[10];
	Itoa(battery, c, 10);
	char returnValue[200] = {"Current value: "};
	Strcat(returnValue, c);

	Strcat(returnValue, "\n\rPercent: ");
	float battery2 = battery / (3936/100);
	char c2[10];
	Ftoa(battery2,c2,5,'f');
	Strcat(returnValue, c2);

	Strcat(returnValue, "\n\rVoltage: ");
	float battery3 = battery / (3936/4.17);
	char c3[10];
	Ftoa(battery3,c3,5,'f');
	Strcat(returnValue, c3);
	return returnValue;
}

void BatteryTask (void* pdata)
{
	//register battery app in cli
	registerInterface(commandGetBattery,getBattery);

	for(;;)
	{
		battery = getCurrentBatteryStatus();
		CoTimeDelay(0,0,0,50);
	}
}
