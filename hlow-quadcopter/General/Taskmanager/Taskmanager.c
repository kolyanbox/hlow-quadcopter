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

void cleanupTaskList()
{
	int i;
	int j = 0;
	allTasks tempTaskList;
	for (i=0;i<CFG_MAX_USER_TASKS;i++)
	{
		if (tasks.taskRunnings[i] == TRUE)
		{
			tempTaskList.allTaskDefs[j] = tasks.allTaskDefs[i];
			tempTaskList.taskIds[j] = tasks.taskIds[i];
			tempTaskList.taskRunnings[j] = tasks.taskRunnings[i];
			j++;
		}
		else
		{
			currentAmmountOfTasks--;
		}
	}
	tasks = tempTaskList;
}

Bool suspendTask(OS_TID id)
{
	int i;
	Bool taskExists = FALSE;
	int taskNumber;
	for (i=0;i<CFG_MAX_USER_TASKS;i++)
	{
		if (tasks.taskIds[i] == id)
		{
			taskNumber = i;
			taskExists = TRUE;
		}
	}
	if (taskExists = FALSE)
	{
		return FALSE;
	}
	if (CoSuspendTask(id) == E_OK)
	{
		tasks.taskRunnings[taskNumber] = FALSE;
		cleanupTaskList();
		return TRUE;
	}
	return FALSE;
}

Bool suspendTaskByName(char* taskName)
{
	int i;
	for (i=0;i<CFG_MAX_USER_TASKS;i++)
	{
		if (Strcmp(tasks.allTaskDefs[i].taskName, taskName) == 0)
		{
			return suspendTask(tasks.taskIds[i]);
		}
	}
	return FALSE;
}
