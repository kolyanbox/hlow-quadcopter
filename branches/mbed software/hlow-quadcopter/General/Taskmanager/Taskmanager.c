#include <General/Taskmanager/Taskmanager.h>
#include <Tasks/Debug/DebugTask.h>
#include <General/util.h>
#include <Drivers/Uart/Uart.h>

allTasks tasks;
int currentAmmountOfTasks = 0;

char commandPs [] = {"ps"};
char completeString[400];
char * processStatus(char *args[])
{
	char *tempString;
	char *tempString2;
	int i;
	completeString[0] = '\0';
	Strcat(completeString,"ID\tRunning\tPriority\tStack size\tTask name\n\r");
	for(i=0;i<CFG_MAX_USER_TASKS;i++)
	{
		if (tasks.taskRunnings[i] == TRUE)
		{
			char c[5];
			Itoa(tasks.taskIds[i],c,10);
			Strcat(completeString,c);
			Strcat(completeString,"\t");
			Strcat(completeString,"TRUE");
			Strcat(completeString,"\t\t");
			Itoa(tasks.allTaskDefs[i].priority,c,10);
			Strcat(completeString,c);
			Strcat(completeString,"\t");
			Itoa(tasks.allTaskDefs[i].stkSz,c,10);
			Strcat(completeString,"\t");
			Strcat(completeString,c);
			Strcat(completeString,"\t");
			Strcat(completeString,tasks.allTaskDefs[i].taskName);
			Strcat(completeString,"\n\r");
		}
	}
	return completeString;
}

Bool initializeTaskmanager()
{
	//register taskmanager app in cli
	registerInterface(commandPs,processStatus);
	return TRUE;
}

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
