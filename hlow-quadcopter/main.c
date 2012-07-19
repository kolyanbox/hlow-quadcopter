/*---------------------------- Include ---------------------------------------*/
#include <Interfaces/Actuators/Actuators.h>
#include <Interfaces/Sensors/Sensors.h>

#include <system_LPC17xx.h>

/*Startup of quadcopter*/
#include <CoOS.h>			              /*!< CoOS header file	         */

#include <General/Taskmanager/Taskmanager.h>

/*tasks*/
#include <Tasks/Main/MainTask.h>

int main (void)
{
	/*Initialize microcontroller*/
	SystemInit();

	/*Initialize CoOs so that all sensors and actuators can use OS related functions*/
	CoInitOS();

	if (createTask(getMainTaskDefenition()) ==FALSE)
	{
		while(1);
	}

	CoStartOS ();			    /* Start multitasking*/

	while (1);                /*!< The code don't reach here	   */
}
