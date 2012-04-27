#include <Tasks/InitTask/InitTask.h>

/*---------------------------- Symbol Define -------------------------------*/
#define STACK_SIZE_DEFAULT 512              /*!< Define a Default task size */

/*---------------------------- Variable Define -------------------------------*/
OS_STK     	DebugTaskStack[STACK_SIZE_DEFAULT];	/*!< Define "SystemStatus" task stack   */

void InitTask (void* pdata)
{
	if (InitializeActuators() == TRUE)
	{
		WriteDebugInfo("Actuators started!\n");
	}
	CoCreateTask (DebugTask,0,64,&DebugTaskStack[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);
	CoExitTask();
}
