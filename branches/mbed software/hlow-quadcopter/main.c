/*---------------------------- Include ---------------------------------------*/
#include <Interfaces/Sensors/Sensors.h>
#include <System/Init.h>
#include <General/util.h>
#include <Interfaces/Actuators/Actuators.h>
#include <Interfaces/Sensors/Sensors.h>

#include <driver/gyroscope.h>

/*Startup of quadcopter*/
#include <Control/Startup.h>
#include <CoOS.h>			              /*!< CoOS header file	         */

/*Controller*/
#include <Controller/MotorControllerTask.h>

/*---------------------------- Symbol Define -------------------------------*/
#define STACK_SIZE_DEFAULT 512              /*!< Define a Default task size */

/*---------------------------- Variable Define -------------------------------*/
OS_STK     	SystemStatus_stk[STACK_SIZE_DEFAULT];	/*!< Define "SystemStatus" task stack   */
OS_STK     	HeartBeat_stk[STACK_SIZE_DEFAULT];	  	/*!< Define "HeartBeat" task stack   */
OS_STK		AcceleroSensor_stk[STACK_SIZE_DEFAULT];
OS_STK		Distance_stk[STACK_SIZE_DEFAULT];
OS_STK		Controller_stk[STACK_SIZE_DEFAULT];
OS_STK     	ControllerTaskTwo_stk[STACK_SIZE_DEFAULT];
OS_STK     	ControllerTaskThree_stk[STACK_SIZE_DEFAULT];
OS_STK     	ControllerTaskFour_stk[STACK_SIZE_DEFAULT];

/**
 *******************************************************************************
 * @brief      	Accelero task
 * @param[in]   None
 * @param[out]  None
 * @retval      None
 * @par Description
 * @details    get values from accelerosensor
 *******************************************************************************
 */
void AcceleroTask (void* pdata)
{
	char print[50];
	uint16_t value = 0;
	//gyroscope_init();
	for (;;)
	{
		value = getCurrentAngle(X);
		Itoa(value, print, 10);
		WriteDebugInfo("X: ");
		WriteDebugInfo(print);
		WriteDebugInfo("\n");

		value = getCurrentAngle(Y);
		Itoa(value, print, 10);
		WriteDebugInfo("Y: ");
		WriteDebugInfo(print);
		WriteDebugInfo("\n");

		value = getCurrentAngle(Z);
		Itoa(value, print, 10);
		WriteDebugInfo("Y: ");
		WriteDebugInfo(print);
		WriteDebugInfo("\n");


		getRotation();


	  CoTickDelay(300);
	}
}

void DistanceTask (void* pdata)
{
  for (;;)
  {
	  float distance = 0;
	  int distanceToGround = getCurrentHeightInCm();
	  //getValue(distanceToGround,0, &distance);
	  char c[30];
	  Itoa(distanceToGround,c,10);
	  //Ftoa(distance,c,'5','f');
	  WriteDebugInfo("Distance: ");
	  WriteDebugInfo(c);
	  WriteDebugInfo("\n");
	  CoTimeDelay(0,0,0,500);
  }
}

void HeartBeat (void* pdata)
{
  for (;;)
  {
	  setLed(led1,TRUE);
	  CoTimeDelay(0,0,0,500);

	  setLed(led1,FALSE);
	  CoTimeDelay(0,0,0,500);
  }
}

int main (void)
{
	/*Initialize microcontroller and debugging unit*/
	/*Initialize microcontroller*/
	SystemInit();
	/*Set UART0 as debugging UART*/
	CoInitOS();
	ActuatorsInitialization();
	sensorInitialization();

	/*First calls*/
	WriteDebugInfo("Welcome\n");
	WriteDebugInfo("Begin startup sequence\n");
	/*Initial CooCox CoOS*/
	WriteDebugInfo("Launch OS\n");


	/*Startup quadcopter*/
	WriteDebugInfo("Startup all sensors\n");
	int quadcopterStartupError = StartUpQuadCopter();
	if (quadcopterStartupError == 0)
	{
		/*Create all tasks tasks*/
		WriteDebugInfo("Create tasks\n");
		CoCreateTask (HeartBeat,0,64,&HeartBeat_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
		CoCreateTask (ControllerTaskTwo,0,0,&ControllerTaskTwo_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
		CoCreateTask (ControllerTaskThree,0,0,&ControllerTaskThree_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
		CoCreateTask (ControllerTaskFour,0,0,&ControllerTaskFour_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
		CoCreateTask (AcceleroTask,0,4,&AcceleroSensor_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
		CoCreateTask (DistanceTask,0,4,&Distance_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
		CoCreateTask (ControllerTaskOne,0,0,&Controller_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
		WriteDebugInfo("Launch all tasks\n");
		CoStartOS ();			    /* Start multitasking*/

	}
	/*if there an error occured while starting up the quadcopter*/
	else
	{
		while (StopQuadCopterImmediate())
		{
			WriteDebugInfo("Quadcopter didnt't stop!\n\r");
		}
		WriteDebugInfo("The quadcopter exited with debugingcode: " + quadcopterStartupError);
	}

	while (1);                /*!< The code don't reach here	   */
}
