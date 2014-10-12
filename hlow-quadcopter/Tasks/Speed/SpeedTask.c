#include <Tasks/Speed/SpeedTask.h>
#include <General/util.h>
#include <CoOS.h>
#include <Tasks/Debug/DebugTask.h>
#include <Interfaces/Sensors/Sensors.h>
char commandSpeed[] = { "getspeed" };
char wrongCommandSpeed[] = {
		"Not a valid command!\ncommand should have value x, y or z." };

float speed[3];
char retVal[15];
#define SpeedStackSize 128
#define FILTERARGUMENTS 3
OS_STK Speed_stk[SpeedStackSize];

OS_MutexID speedMutex;

taskDef t;
taskDef getSpeedTaskDefenition() {
	t.priority = 63;
	t.stk = &Speed_stk[SpeedStackSize - 1];
	t.stkSz = SpeedStackSize;
	t.task = SpeedTask;
	t.taskName = "Speed";
	return t;
}

float getSpeed(enum Axle axle) {
	if (axle == X) {
		CoEnterMutexSection(speedMutex);
		float returnValue = speed[X];
		CoLeaveMutexSection(speedMutex);
		return returnValue;
	} else if (axle == Y) {
		CoEnterMutexSection(speedMutex);
		float returnValue = speed[Y];
		CoLeaveMutexSection(speedMutex);
		return returnValue;
	} else {
		CoEnterMutexSection(speedMutex);
		float returnValue = speed[Z];
		CoLeaveMutexSection(speedMutex);
		return returnValue;
	}
}

char * printInfoSpeed(int argc, char *args[]) {
	if (argc != 1) {
		return wrongCommandSpeed;
	}
	char x[] = { "x" };
	char y[] = { "y" };
	char z[] = { "z" };
	CoEnterMutexSection(speedMutex);
	if (Strcmp(args[0], x) == 0) {
		Ftoa(speed[X], retVal, 5, 'f');
		CoLeaveMutexSection(speedMutex);
		return retVal;
	} else if (Strcmp(args[0], y) == 0) {
		Ftoa(speed[Y], retVal, 5, 'f');
		CoLeaveMutexSection(speedMutex);
		return retVal;
	} else if (Strcmp(args[0], z) == 0) {
		Ftoa(speed[Z], retVal, 5, 'f');
		CoLeaveMutexSection(speedMutex);
		return retVal;
	}
	CoLeaveMutexSection(speedMutex);
	return wrongCommandSpeed;
}

void SpeedTask(void* pdata) {
	//register angle app in cli
	registerInterface(commandSpeed, printInfoSpeed);

	float dataX[FILTERARGUMENTS];
	float dataY[FILTERARGUMENTS];
	float dataZ[FILTERARGUMENTS];

	int i;
	for (i = 0; i < FILTERARGUMENTS - 1; i++) {
		dataX[i] = 0;
		dataY[i] = 0;
		dataZ[i] = 0;
	}
	i = 0;

	speedMutex = CoCreateMutex();

	for (;;) {
		CoEnterMutexSection(speedMutex);
		float *f = getRotationAroundAxle();

		dataX[i] = f[X];
		dataY[i] = f[Y];
		dataZ[i] = f[Z];

		speed[X] = lowPassFilter(dataX, FILTERARGUMENTS);
		speed[Y] = lowPassFilter(dataY, FILTERARGUMENTS);
		speed[Z] = lowPassFilter(dataZ, FILTERARGUMENTS);
		CoLeaveMutexSection(speedMutex);

		i++;
		if (i == FILTERARGUMENTS) {
			i = 0;
		}

		CoTimeDelay(0, 0, 0, 50);
	}
}
