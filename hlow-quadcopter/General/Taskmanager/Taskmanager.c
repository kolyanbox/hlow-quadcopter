#include <General/Taskmanager/Taskmanager.h>
#include <Tasks/Debug/DebugTask.h>
#include <General/util.h>
#include <Drivers/Uart/Uart.h>
#include <Tasks/Main/MainTask.h>
#include <Tasks/Logging/LoggingTask.h>
#include <Tasks/Heartbeat/Heartbeat.h>
#include <Tasks/Distancetoground/DistanceToGroundTask.h>

allRunningTasks tasks;
int currentAmmountOfTasks = 0;

char commandPs [] = {"ps"};
char commandKill [] = {"kill"};
char commandStart [] = {"start"};
char completeString[400];
char * processStatus(int argc, char *args[])
{
	int i;
	completeString[0] = '\0';
	Strcat(completeString,"ID\tosID\tRunning\tPriority\tStack size\tTask name\n\r");
	for(i=0;i<CFG_MAX_USER_TASKS;i++)
	{
		if (tasks.taskRunnings[i] == TRUE)
		{
			char c[5];
			Itoa(i,c,10);
			Strcat(completeString,c);
			Strcat(completeString,"\t");
			Itoa(tasks.taskIds[i],c,10);
			Strcat(completeString,c);
			Strcat(completeString,"\t");
			Strcat(completeString,"TRUE");
			Strcat(completeString,"\t\t");
			Itoa(tasks.allTaskDefs[i].priority,c,10);
			Strcat(completeString,c);
			Strcat(completeString,"\t");
			Itoa(tasks.allTaskDefs[i].stkSz,c,10);
			Strcat(completeString,c);
			Strcat(completeString,"\t\t");
			Strcat(completeString,tasks.allTaskDefs[i].taskName);
			Strcat(completeString,"\n\r");
		}
	}
	Strcat(completeString,"Free slots: ");
	char c[5];
	Itoa((CFG_MAX_USER_TASKS-getCurrentAmountOfTasks()),c,10);
	Strcat(completeString,c);
	Strcat(completeString,"\n\r");
	return completeString;
}

char * killProcess(int argc, char *args[])
{
	int processId = Atoi(args[0]);
	if (processId < currentAmmountOfTasks && killTask(tasks.taskIds[processId]) == TRUE)
	{
		return "Process killed";
	}
	else
	{
		return "No such process";
	}
}
#include <Tasks/Angle/AngleTask.h>
char * startProcess(int argc, char *args[])
{
	Bool retVal = FALSE;
	if (Strcmp(args[0],getLoggingTaskDefenition().taskName) == 0)
	{
		retVal = createTask(getLoggingTaskDefenition());
	}
	else if (Strcmp(args[0],getMainTaskDefenition().taskName) == 0)
	{
		retVal = createTask(getMainTaskDefenition());
	}
	else if (Strcmp(args[0],getDebugTaskDefenition().taskName) == 0)
	{
		retVal = createTask(getDebugTaskDefenition());
	}
	else if (Strcmp(args[0],getAngleTaskDefenition().taskName) == 0)
	{
		retVal = createTask(getAngleTaskDefenition());
	}
	else if (Strcmp(args[0],getHeartbeatTaskDefenition().taskName) == 0)
	{
		retVal = createTask(getHeartbeatTaskDefenition());
	}
	else if (Strcmp(args[0],getDistanceTaskDefenition().taskName) == 0)
	{
		retVal = createTask(getDistanceTaskDefenition());
	}
	if (retVal == FALSE)
	{
		return "Task couldn't be created";
	}
	return "task started";
}

Bool initializeTaskmanager()
{
	//register taskmanager app in cli
	registerInterface(commandPs,processStatus);
	registerInterface(commandKill,killProcess);
	registerInterface(commandStart,startProcess);
	return TRUE;
}

Bool createTask(taskDef t)
{
	int i;
	// check if task already exists
	for (i=0;i<CFG_MAX_USER_TASKS;i++)
	{
		if (tasks.allTaskDefs[i].task == t.task && tasks.taskRunnings[i] == TRUE)
		{
			return FALSE;
		}
	}
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

static void cleanupTaskList()
{
	int i;
	int j = 0;
	allRunningTasks tempTaskList;
	for (i=0;i<CFG_MAX_USER_TASKS;i++)
	{
		tempTaskList.taskRunnings[i] = FALSE;
		if (tasks.taskRunnings[i] == TRUE)
		{
			tempTaskList.allTaskDefs[j] = tasks.allTaskDefs[i];
			tempTaskList.taskIds[j] = tasks.taskIds[i];
			tempTaskList.taskRunnings[j] = tasks.taskRunnings[i];
			j++;
		}
	}
	currentAmmountOfTasks = j;
	tasks = tempTaskList;
}

Bool killTask(OS_TID id)
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
			break;
		}
	}
	if (taskExists == FALSE)
	{
		return FALSE;
	}
	if (CoDelTask(id) == E_OK)
	{
		WriteDebugInfo("deleted task\n\r");
		tasks.taskRunnings[taskNumber] = FALSE;
		tasks.taskIds[taskNumber] = -1;
		cleanupTaskList();
		return TRUE;
	}
	return FALSE;
}
