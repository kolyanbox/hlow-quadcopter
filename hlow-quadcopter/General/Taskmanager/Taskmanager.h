/***************************************************************************//**
 * @file      : Taskmanager.h
 * @brief     : Task manager able to manage tasks of real time operating system CoOs
 * @version   : V0.1
 * @date      : 19. July. 2012
*******************************************************************************/

#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include <lpc_types.h>
#include <CoOS.h>

/**
 * struct containing a singe task
 */
typedef struct
{
	char* taskName;	///< Name of the task in plain text
	FUNCPtr task;	///< Pointer to the task
	U8 priority;	///< Priority of the task
	OS_STK* stk;	///< Pointer to the stack of the task
	U16 stkSz;		///< The size of the stack
} taskDef;

/**
 * struct containing all running tasks.
 */
typedef struct
{
	OS_TID taskIds[CFG_MAX_USER_TASKS];		///< Containing all id's from the tasks
	Bool taskRunnings[CFG_MAX_USER_TASKS];	///< Containing the running or not running status of the tasks
	taskDef allTaskDefs[CFG_MAX_USER_TASKS];///< Containing a reference to a specific task
}allRunningTasks;

/**
 * First call this method before using others
 */
Bool initializeTaskmanager();
/**
 * Method for creating a new task. Returns TRUE on success, False otherwise
 */
Bool createTask(taskDef t);
/**
 * Method to kill a task by task id
 */
Bool killTask(OS_TID id);
/**
 * Method to get the current amount of tasks
 */
int getCurrentAmountOfTasks();

#endif
