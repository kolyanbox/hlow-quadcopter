#include <Control/Startup.h>

/*Function to startup all vital tasks and check if they work
 * Returns 0 if succeeded
 * If this function doesn't return 0, make a call to void StopQuadCopterImmediate(void);
 * to safely stop the quadcopters functions*/
int StartUpQuadCopter(void)
{
	/*Initialize all to use Peripherals*/
	int PeripheralError = init_Peripherals();
	if (PeripheralError != 0)
	{
		return PeripheralError;
	}
	/*Initialize motors*/
	if (!MotorInit())
	{
		return 10;
	}
	return 0;
}

/*
 * This functions will stop all functions of the quadcopter
 * Returns true if succeeding to stop and false otherwise
 * */
Bool StopQuadCopterImmediate(void)
{
	/*if stopping motors isn't successful*/
	if (!SetMotor(motor1,0) || !SetMotor(motor2,0) || !SetMotor(motor3,0) || !SetMotor(motor4,0))
	{
		return FALSE;
	}
	return TRUE;
}
