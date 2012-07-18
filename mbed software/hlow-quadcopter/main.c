/*---------------------------- Include ---------------------------------------*/
#include <Interfaces/Actuators/Actuators.h>
#include <Interfaces/Sensors/Sensors.h>

#include <system_LPC17xx.h>

/*Startup of quadcopter*/
#include <CoOS.h>			              /*!< CoOS header file	         */

#include <General/Taskmanager/Taskmanager.h>

/*tasks*/
#include <Tasks/Main/MainTask.h>

/*---------------------------- Symbol Define -------------------------------*/
#define STACK_SIZE_DEFAULT 64              /*!< Define a Default task size */

/*---------------------------- Variable Define -------------------------------*/
OS_STK     	MainTask_stk[STACK_SIZE_DEFAULT];

int main (void)
{
	/*Initialize microcontroller*/
	SystemInit();

	/*Initialize CoOs so that all sensors and actuators can use OS related functions*/
	CoInitOS();

	taskDef t;
	t.priority = 63;
	t.stk = &MainTask_stk[STACK_SIZE_DEFAULT-1];
	t.stkSz = STACK_SIZE_DEFAULT;
	t.task = MainTask;
	t.taskName = "Main";
	if (createTask(t) ==FALSE)
	{
		while(1);
	}

	CoStartOS ();			    /* Start multitasking*/

	while (1);                /*!< The code don't reach here	   */
}
