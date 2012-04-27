#include <Tasks/MainTask/Maintask.h>

/*---------------------------- Symbol Define -------------------------------*/
#define STACK_SIZE_DEFAULT 512              /*!< Define a Default task size */

/*---------------------------- Variable Define -------------------------------*/
OS_STK     	InitTaskStack[STACK_SIZE_DEFAULT];	/*!< Define "SystemStatus" task stack   */

void MainTask (void* pdata)
{
	CoCreateTask (InitTask,0,64,&InitTaskStack[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
	for (;;)
	{
		CoTimeDelay(0,0,0,500);
	}
}
