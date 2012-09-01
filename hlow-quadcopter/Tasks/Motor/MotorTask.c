#include <Tasks/Motor/MotorTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <Interfaces/Actuators/Actuators.h>
#include <General/util.h>

//#include <Drivers/Uart/Uart.h>

char commandSetMotor[] = {"setmotor"};
char wrongCommandSetMotor[] = {"Not a valid command!"};

int speedMotorFront = 0;
int speedMotorBack = 0;
int speedMotorLeft = 0;
int speedMotorRight = 0;

#define MotorStackSize 64
OS_STK	Motor_stk[MotorStackSize];

taskDef t;
taskDef getMotorTaskDefenition()
{
	t.priority = 63;
	t.stk = &Motor_stk[MotorStackSize-1];
	t.stkSz = MotorStackSize;
	t.task = MotorTask;
	t.taskName = "Motor";
	return t;
}

char * printInfoAngle(int argc, char *args[])
{
	if (argc != 2)
	{
		return "Unknown amount of parameters";
	}
	char f[] = {"f"};
	char b[] = {"b"};
	char l[] = {"l"};
	char r[] = {"r"};
	if (Strcmp(args[0],f) == 0)
	{
		speedMotorFront = Atoi(args[1]);
		return NULL;
	}
	else if (Strcmp(args[0],b) == 0)
	{
		speedMotorBack = Atoi(args[1]);
		return NULL;
	}
	else if (Strcmp(args[0],l) == 0)
	{
		speedMotorLeft = Atoi(args[1]);
		return NULL;
	}
	else if (Strcmp(args[0],r) == 0)
	{
		speedMotorRight = Atoi(args[1]);
		return NULL;
	}
	return wrongCommandSetMotor;
}

void MotorTask (void* pdata)
{
	//register angle app in cli
	registerInterface(commandSetMotor,printInfoAngle);

	for(;;)
	{
		setSpeedBack(speedMotorBack);
		setSpeedFront(speedMotorFront);
		setSpeedLeft(speedMotorLeft);
		setSpeedRight(speedMotorRight);
		CoTimeDelay(0,0,0,500);
	}
}
