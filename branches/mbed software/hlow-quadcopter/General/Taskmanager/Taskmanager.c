#include <General/Taskmanager/Taskmanager.h>

#include <General/util.h>

allTasks tasks;
int currentAmmountOfTasks = 0;

Bool createTask(taskDef t)
{
	if (currentAmmountOfTasks+1 > CFG_MAX_USER_TASKS)
	{
		return FALSE;
	}
	tasks.taskIds[currentAmmountOfTasks] = CoCreateTask (t.task,0,t.priority,t.stk,t.stkSz);
	if (tasks.taskIds[currentAmmountOfTasks] < 0)
	{
		return FALSE;
	}
	tasks.allTaskDefs[currentAmmountOfTasks] = t;
	tasks.taskRunnings[currentAmmountOfTasks] = TRUE;

	currentAmmountOfTasks++;
	return TRUE;
}

allTasks getAllTaskStatus()
{
	return tasks;
}

int getCurrentAmountOfTasks()
{
	return currentAmmountOfTasks;
}

Bool suspendTask(void)
{
	return TRUE;
}
