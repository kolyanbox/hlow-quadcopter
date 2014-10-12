#include <Tasks/Angle/AngleTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <Tasks/Logging/LoggingTask.h>
#include <General/util.h>

#include <Drivers/Uart/Uart.h>

char commandGetAngle[] = { "getangle" };
char wrongCommandGetAngle[] = {
		"Not a valid command!\nParameter shuld contain x,y or z" };

OS_EventID semAngleX;
char Anglex[5];
char Angley[5];
char Anglez[5];

#define MOVINGAVERAGESAMPLES 8

#define AngleStackSize 128
OS_STK Angle_stk[AngleStackSize];

taskDef t;
taskDef getAngleTaskDefenition() {
	t.priority = 63;
	t.stk = &Angle_stk[AngleStackSize - 1];
	t.stkSz = AngleStackSize;
	t.task = AngleTask;
	t.taskName = "Angle";
	return t;
}

char * getAngle(int argc, char *args[]) {
	if (argc != 1) {
		return wrongCommandGetAngle;
	}
	char x[] = { "x" };
	char y[] = { "y" };
	char z[] = { "z" };
	if (Strcmp(args[0], x) == 0) {
//		char Angle[5];
//		StatusType s1 = CoPendSem(semAngleX,0xff);
//		if (s1 ==  E_OK){
//			Strcpy(Angle, Anglex);
//			CoPostSem(semAngleX);
//		}
//		else
//		{
//			return 0;
//		}
		return Anglex;
	} else if (Strcmp(args[0], y) == 0) {
		return Angley;
	} else if (Strcmp(args[0], z) == 0) {
		return Anglez;
	}
	return wrongCommandGetAngle;
}

void AngleTask(void* pdata) {
	semAngleX = CoCreateSem(0, 1, EVENT_SORT_TYPE_FIFO);

	uint16_t value = 0;
	float movingAverageY[MOVINGAVERAGESAMPLES];
	float movingAverageX[MOVINGAVERAGESAMPLES];
	int i = 0;
	for (i = 0; i < MOVINGAVERAGESAMPLES / 1; i++) {
		movingAverageY[i] = 90;
		movingAverageX[i] = 90;
	}
	i = 0;
	//register angle app in cli
	registerInterface(commandGetAngle, getAngle);

	for (;;) {
		value = (getCurrentAngle(X) - 1148) / 11.10556;
		movingAverageX[i] = value;
		value = lowPassFilter(movingAverageX, MOVINGAVERAGESAMPLES);
		Itoa(value, Anglex, 10);

		value = (getCurrentAngle(Y) - 1094) / 10.90556;
		movingAverageY[i] = value;
		value = lowPassFilter(movingAverageY, MOVINGAVERAGESAMPLES);

		Itoa(value, Angley, 10);

//		WriteDebugInfo(Angley);
//		WriteDebugInfo(",");
//		WriteDebugInfo(Anglex);
//		WriteDebugInfo("\r\n");

		value = getCurrentAngle(Z);
		Itoa(value, Anglez, 10);

		i++;
		if (i > MOVINGAVERAGESAMPLES - 1) {
			i = 0;
		}

		CoTimeDelay(0, 0, 0, 50);
	}
}
