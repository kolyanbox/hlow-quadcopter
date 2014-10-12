#include <Tasks/Heartbeat/Heartbeat.h>
#include <Interfaces/Sensors/Sensors.h>
#include <General/util.h>

#define HeartBeatStackSize 128
OS_STK HeartBeat_stk[HeartBeatStackSize];

taskDef t;
taskDef getHeartbeatTaskDefenition() {
	t.priority = 63;
	t.stk = &HeartBeat_stk[HeartBeatStackSize - 1];
	t.stkSz = HeartBeatStackSize;
	t.task = HeartBeat;
	t.taskName = "Heartbeat";
	return t;
}

char *x[] = { "x\0" };
char *y[] = { "y\0" };
char *z[] = { "z\0" };
char *r[] = { "r\0" };
char *a[] = { "a\0" };
void HeartBeat(void* pdata) {
	for (;;) {

		char val[80] = "";
		char valX[15];
		Ftoa(getSpeed(X), valX, 5, 'f');
		char valY[15];
		Ftoa(getSpeed(X), valY, 5, 'f');
		char valZ[15];
		Ftoa(getSpeed(X), valZ, 5, 'f');

		strcpy(val,valX);
		strcat(valX,",");
		strcat(valX,valY);
		strcat(valX,",");
		strcat(valX,valZ);

		WriteDebugInfo( valX);
//		WriteDebugInfo(",");
//		WriteDebugInfo(valY);
//		WriteDebugInfo(",");
//		WriteDebugInfo(valZ);

		WriteDebugInfo(",");
		WriteDebugInfo(getAngle(1, x));
		WriteDebugInfo(",");
		WriteDebugInfo(getAngle(1, y));
		WriteDebugInfo(",");
		WriteDebugInfo(getAngle(1, z));

//		WriteDebugInfo(",");
//		WriteDebugInfo(printInfoDistanceToGround(1, r));
//		WriteDebugInfo(",");
//		WriteDebugInfo(printInfoDistanceToGround(1, a));

		WriteDebugInfo("\r\n");

		setLed(led1, TRUE);
		CoTimeDelay(0, 0, 0, 50);
		setLed(led1, FALSE);
		CoTimeDelay(0, 0, 0, 50);
	}
}
