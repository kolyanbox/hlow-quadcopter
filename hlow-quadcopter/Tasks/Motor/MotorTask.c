#include <Tasks/Motor/MotorTask.h>
#include <Tasks/Debug/DebugTask.h>
#include <Interfaces/Actuators/Actuators.h>
#include <General/util.h>

//#include <Drivers/Uart/Uart.h>

char commandSetMotor[] = {"setmotor"};
char wrongCommandSetMotor[] = {"Not a valid command!\nCommand should have parameter f,b,l,r or a and a number."};
char motorHasBeenSet[] = {"Motor has been set."};

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
		return wrongCommandSetMotor;
	}
	char f[] = {"f"};
	char b[] = {"b"};
	char l[] = {"l"};
	char r[] = {"r"};
	char a[] = {"a"};
	if (Strcmp(args[0],f) == 0)
	{
		speedMotorFront = Atoi(args[1]);
		return motorHasBeenSet;
	}
	else if (Strcmp(args[0],b) == 0)
	{
		speedMotorBack = Atoi(args[1]);
		return motorHasBeenSet;
	}
	else if (Strcmp(args[0],l) == 0)
	{
		speedMotorLeft = Atoi(args[1]);
		return motorHasBeenSet;
	}
	else if (Strcmp(args[0],r) == 0)
	{
		speedMotorRight = Atoi(args[1]);
		return motorHasBeenSet;
	}
	else if (Strcmp(args[0],a) == 0)
	{
		int motorspeed = Atoi(args[1]);
		speedMotorBack = motorspeed;
		speedMotorFront = motorspeed;
		speedMotorLeft = motorspeed;
		speedMotorRight = motorspeed;
		return motorHasBeenSet;
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
