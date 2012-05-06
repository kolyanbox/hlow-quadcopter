/*---------------------------- Include ---------------------------------------*/
#include <General/util.h>
#include <Interfaces/Actuators/Actuators.h>
#include <Interfaces/Sensors/Sensors.h>

/*Startup of quadcopter*/
#include <CoOS.h>			              /*!< CoOS header file	         */

/*tasks*/
#include <Tasks/Main/MainTask.h>

/*Controller*/
#include <Controller/MotorControllerTask.h>

/*---------------------------- Symbol Define -------------------------------*/
#define STACK_SIZE_DEFAULT 512              /*!< Define a Default task size */

/*---------------------------- Variable Define -------------------------------*/
OS_STK		Initialize_stk[STACK_SIZE_DEFAULT];

OS_STK     	SystemStatus_stk[STACK_SIZE_DEFAULT];	/*!< Define "SystemStatus" task stack   */
OS_STK     	MainTask_stk[STACK_SIZE_DEFAULT];	  	/*!< Define "HeartBeat" task stack   */
//OS_STK		AcceleroSensor_stk[STACK_SIZE_DEFAULT];
//OS_STK		Distance_stk[STACK_SIZE_DEFAULT];
//OS_STK		Controller_stk[STACK_SIZE_DEFAULT];
//OS_STK     	ControllerTaskTwo_stk[STACK_SIZE_DEFAULT];
//OS_STK     	ControllerTaskThree_stk[STACK_SIZE_DEFAULT];
//OS_STK     	ControllerTaskFour_stk[STACK_SIZE_DEFAULT];

void Initialize (void* pdata)
{
	/*If a sensor fails initializing the program has to stop. Because We have to initialize a led so that the user has
	 * debug information*/
	if (sensorInitialization() == FALSE)
	{
		GPIO_init(led4, 1, 1);
		setLed(led4,TRUE);
		while(1);
	}
	/*If an actuator fails to initialize we have to stop initializing the actuators
	 * We will give feedback by enable led 4 and 3 because we don't know if the uart is properly initialized*/
	if (ActuatorsInitialization() == FALSE)
	{
		setLed(led4,TRUE);
		setLed(led3,TRUE);
		while(1);
	}
	/*Create maintask*/
	CoCreateTask (MainTask,0,63,&MainTask_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
	CoExitTask();
}
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
		//value = getCurrentAngle(X);
		//Itoa(value, print, 10);
		//WriteDebugInfo("X: ");
		//WriteDebugInfo(print);
		//WriteDebugInfo("\n");

		//value = getCurrentAngle(Y);
		//Itoa(value, print, 10);
		//WriteDebugInfo("Y: ");
		//WriteDebugInfo(print);
		//WriteDebugInfo("\n");

		//value = getCurrentAngle(Z);
		//Itoa(value, print, 10);
		//WriteDebugInfo("Y: ");
		//WriteDebugInfo(print);
		//WriteDebugInfo("\n");


		//getRotation();

		//long lvalue = getCurrentTemperature();
		//value = (int)lvalue;
		//Itoa(value, print, 10);
		//WriteDebugInfo("temp: ");
		//WriteDebugInfo(print);
		//WriteDebugInfo("\n");

		//value = getCurrentPressure();
		//Itoa(value, print, 10);
		//WriteDebugInfo("pressure: ");
		//WriteDebugInfo(print);
		//WriteDebugInfo("\n");
CoExitTask();
	  CoTickDelay(300);
	}
}

int main (void)
{
	/*Initialize microcontroller*/
	SystemInit();

	/*Initialize CoOs so that all sensors and actuators can use OS related functions*/
	CoInitOS();


	/*First calls*/
	CoCreateTask (Initialize,0,0,&Initialize_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
	CoStartOS ();			    /* Start multitasking*/

	while (1);                /*!< The code don't reach here	   */
}
