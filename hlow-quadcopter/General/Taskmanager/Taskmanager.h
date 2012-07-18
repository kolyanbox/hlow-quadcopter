#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include <lpc_types.h>
#include <CoOS.h>

/*
 * struct containing a singe task
 * */
typedef struct
{
	char* taskName;
	FUNCPtr task;
	U8 priority;
	OS_STK* stk;
	U16 stkSz;
} taskDef;

/*
 * struct containing all tasks.
 * */
typedef struct
{
	OS_TID taskIds[CFG_MAX_USER_TASKS];
	Bool taskRunnings[CFG_MAX_USER_TASKS];
	taskDef allTaskDefs[CFG_MAX_USER_TASKS];
}allTasks;

/*
 * First call this method before using others
 * */
Bool initializeTaskmanager();
Bool createTask(taskDef t);
Bool suspendTask(OS_TID id);
Bool suspendTaskByName(char* taskName);
int getCurrentAmountOfTasks();

#endif
