#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include <lpc_types.h>
#include <CoOS.h>

typedef struct
{
	char* taskName;
	FUNCPtr task;
	U8 priority;
	OS_STK* stk;
	U16 stkSz;
} taskDef;

typedef struct
{
	OS_TID taskIds[CFG_MAX_USER_TASKS];
	Bool taskRunnings[CFG_MAX_USER_TASKS];
	taskDef allTaskDefs[CFG_MAX_USER_TASKS];
}allTasks;


Bool createTask(taskDef t);
Bool suspendTask(void);
allTasks getAllTaskStatus();
int getCurrentAmountOfTasks();

#endif
