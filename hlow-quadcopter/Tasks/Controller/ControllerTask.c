#include <Tasks/Controller/ControllerTask.h>
#include <Tasks/Angle/AngleTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <Interfaces/Actuators/Actuators.h>
#include <General/util.h>
#include <Tasks/Logging/LoggingTask.h>

char commandStopController[] = { "stopcontroller" };
char commandSetKD[] = { "setkd" };
char commandSetKI[] = { "setki" };
char commandSetKP[] = { "setkp" };
char wrongKDCommand[] = { "error!!" };

#define ControllerStackSize 128
OS_STK Controller_stk[ControllerStackSize];

taskDef t;
taskDef getControllerTaskDefenition() {
	t.priority = 63;
	t.stk = &Controller_stk[ControllerStackSize - 1];
	t.stkSz = ControllerStackSize;
	t.task = ControllerTask;
	t.taskName = "Controller";
	return t;
}

char * stopController(int argc, char *args[]) {
	return "Controller stopped";
}

float kd = 0.125;
char * setkd(int argc, char *args[]) {
	if (argc != 1) {
		return wrongKDCommand;
	}
	kd = Atof(args[0]);

	return "kd is set";
}

float ki = 0.05;
char * setki(int argc, char *args[]) {
	if (argc != 1) {
		return wrongKDCommand;
	}
	ki = Atof(args[0]);

	return "ki is set";
}

float kp = 0.6;
char * setkp(int argc, char *args[]) {
	if (argc != 1) {
		return wrongKDCommand;
	}
	kp = Atof(args[0]) / 100;

	return "kp is set";
}

void ControllerTask(void* pdata) {
	//register angle app in cli
	registerInterface(commandStopController, stopController);
	registerInterface(commandSetKD, setkd);
	registerInterface(commandSetKI, setki);
	registerInterface(commandSetKP, setkp);

	int outF = 0;
	int outB = 0;

	float previousError = 0;
	float integral = 0;
	float dt = 0.05;

	unsigned char appNumber = 0;
	unsigned char appName[] = { "Controller" };

	appNumber = registerApp(appName, LOG_CRIT);

	for (;;) {
		char *args[] = { "y" };

		long int input = Strtol(getAngle(1, args),
				(char **) getAngle(1, args) + 4, 10);

		float setpoint = 90;
		float error = setpoint - input;
		integral = integral + error * dt;
		float derivative = (error - previousError) / dt;
		float output = kp * error +ki*integral+kd*derivative;
		previousError = error;

		char c[10];

		if (output > 0) {
			outF = output;
			outB = 0;
		} else {
			outF = 0;
			outB = output * -1;
		}
		if (outB > 30) {
			outB = 30;
		}
		if (outF > 30) {
			outF = 30;
		}

		Itoa(outB, c, 10);
		writeLog(appNumber,c,LOG_DEBUG);

		writeLog(appNumber,">>",LOG_DEBUG);
		Itoa(input, c, 10);
		writeLog(appNumber,c,LOG_DEBUG);
		//WriteDebugInfo(c);
		//WriteDebugInfo(" B\n\r");

		Itoa(outF, c, 10);
		//WriteDebugInfo(c);
		//WriteDebugInfo(" F\n\r");

		Itoa(input, c, 10);
		//WriteDebugInfo(c);
		//WriteDebugInfo(" O\n\r");

		//setSpeed Back(speedB);
		//setSpeedFront(speedF);
		setSpeedFront(++outF);
		setSpeedBack(++outB);
		CoTimeDelay(0, 0, 0, 50);
	}
}
