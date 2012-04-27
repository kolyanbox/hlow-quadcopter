#include <system_LPC17xx.h>
#include <CoOS.h>
#include <Tasks/MainTask/Maintask.h>

/*---------------------------- Symbol Define -------------------------------*/
#define STACK_SIZE_DEFAULT 32              /*!< Define a Default task size */

/*---------------------------- Variable Define -------------------------------*/
OS_STK     	MainTaskStack[STACK_SIZE_DEFAULT];	/*!< Define "SystemStatus" task stack   */


int main(void)
{
	/*Always start with initializing the system!!!!*/
	SystemInit();

	/*Initialize CoOs*/
	CoInitOS();
	/*Start the main task*/
	CoCreateTask (MainTask,0,64,&MainTaskStack[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
	/*Start CoOs*/
	CoStartOS();
	/*Code won't reach this*/
	while (1);
}
