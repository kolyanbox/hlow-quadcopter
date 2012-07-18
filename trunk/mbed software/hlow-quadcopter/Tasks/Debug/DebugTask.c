#include <Tasks/Debug/DebugTask.h>

#include <Interfaces/Actuators/Actuators.h>
#include <CoOS.h>
#include <Interfaces/Sensors/Sensors.h>
#include <General/Taskmanager/Taskmanager.h>

int messageNumber = 0;
char* messages[maxMessages];

OS_EventID messagesSem;

//Angle semaphore
OS_EventID angleSem;
char* Angle[3];

//distance to ground semaphore
OS_EventID distanceToGroundSem;
char* distanceToGround;

char* firstParam;

interface interfaces[MAXAMOUNTOFINTERFACES];
int currentAmmountOfInterfaces = 0;

Bool cliInitialization()
{
	int i;
	for (i = 0; i<MAXAMOUNTOFINTERFACES;i++)
	{
		interfaces[i].occupiedSlot = FALSE;
	}
}

Bool DebugTaskInitialization()
{
	cliInitialization();
	messagesSem = CoCreateSem(1,1,EVENT_SORT_TYPE_FIFO);
	if (messagesSem == E_CREATE_FAIL)
	{
		return FALSE;
	}
	CoPostSem(messagesSem);

	angleSem = CoCreateSem(1,1,EVENT_SORT_TYPE_FIFO);
	if (angleSem == E_CREATE_FAIL)
	{
		return FALSE;
	}
	CoPostSem(angleSem);

	distanceToGroundSem = CoCreateSem(1,1,EVENT_SORT_TYPE_FIFO);
	if (distanceToGroundSem == E_CREATE_FAIL)
	{
		return FALSE;
	}
	CoPostSem(distanceToGroundSem);

	return TRUE;
}

int registerInterface(char* command, void *interface)
{
	int i = 0;
	int prevAmmountOfInterfaces = currentAmmountOfInterfaces;
	while (i < MAXAMOUNTOFINTERFACES)
	{
		if (interfaces[i].occupiedSlot == FALSE)
		{
			interfaces[i].occupiedSlot = TRUE;
			interfaces[i].interface = interface;
			Strcpy(interfaces[i].command,command);
			currentAmmountOfInterfaces++;
			break;
		}
		i++;
	}
	if (prevAmmountOfInterfaces == currentAmmountOfInterfaces)
	{
		return -1;
	}
	return currentAmmountOfInterfaces-1;
}

Bool callInterfaceById (int interfaceNumber)
{
	if (interfaces[interfaceNumber].occupiedSlot == TRUE)
	{
		WriteDebugInfo(interfaces[interfaceNumber].interface(interfaces[interfaceNumber].parameters));
		return TRUE;
	}
	return FALSE;
}

void DebugTask (void* pdata)
{
	for (;;)
	{
		char *command = lastReceivedCommand();
		int i;
		for (i=0;i<MAXAMOUNTOFINTERFACES;i++)
		{
			if (interfaces[i].occupiedSlot == TRUE && Strcmp(interfaces[i].command,command) == 0)
			{
				callInterfaceById(i);
				WriteDebugInfo("\n\r/>");
				break;
			}
		}

/*		switch (getCommand())
		{
			case (CommandRotationX):
			{
				if (CoPendSem(messagesSem,0) == E_OK){
					WriteDebugInfo(Angle[0]);
					CoPostSem(messagesSem);
				}
				WriteDebugInfo("\n\r/>");
				break;
			}
			case (CommandRotationY):
			{
				if (CoPendSem(messagesSem,0) == E_OK){
					WriteDebugInfo(Angle[1]);
					CoPostSem(messagesSem);
				}
				WriteDebugInfo("\n\r/>");
				break;
			}
			case (CommandRotationZ):
			{
				if (CoPendSem(messagesSem,0) == E_OK){
					WriteDebugInfo(Angle[2]);
					CoPostSem(messagesSem);
				}
				WriteDebugInfo("\n\r/>");
				break;
			}
			case (commandOsTime):
			{
				char osTime[20];
				int t = CoGetOSTime();
				Itoa(t,osTime,10);
				WriteDebugInfo(osTime);
				WriteDebugInfo("\n\r/>");
				break;
			}
			case (commandDistanceToGround):
			{
				if (CoPendSem(distanceToGroundSem,0) == E_OK){
					WriteDebugInfo(distanceToGround);
					CoPostSem(distanceToGroundSem);
				}
				WriteDebugInfo("\n\r/>");
				break;
			}
			case (CommandHelp):
			{
				WriteDebugInfo("These are all available commands:\n\r"
								"Get current os time:            getostime\n\r"
								"Get current angle x:            getanglex\n\r"
								"Get current angle y:            getangley\n\r"
								"Get current angle z:            getanglez\n\r"
								"Get current distance to ground: getdtc\n\r"
								"/>");
				break;
			}
			case (CommandAllTaskStatus):
			{
				allTasks tasks = getAllTaskStatus();
				int i = 0;
				for (i=0;i<getCurrentAmountOfTasks();i++)
				{
					char number[1];
					Itoa(tasks.taskIds[i],number,10);
					WriteDebugInfo("Tasknumber: ");
					WriteDebugInfo(number);
					WriteDebugInfo("\n\rTaskname: ");
					WriteDebugInfo(tasks.allTaskDefs[i].taskName);
					WriteDebugInfo("\n\rRunning: ");
					if (tasks.taskRunnings[i] == TRUE)
					{
						WriteDebugInfo("true\n\r");
					}
					else
					{
						WriteDebugInfo("false\n\r");
					}
					WriteDebugInfo("Priority: ");
					Itoa(tasks.allTaskDefs[i].priority,number,10);
					WriteDebugInfo(number);
					WriteDebugInfo("\n\r");
				}
				WriteDebugInfo("\n\r/>");
				break;
			}
			case (CommandStopTaskWithId):
			{
				int taskId = Atoi(firstParam);
				if (suspendTask(taskId) == TRUE)
				{
					WriteDebugInfo("stop a task\n\r/>");
				}
				else {
					WriteDebugInfo("Task couldn't be stopped!\n\r/>");
				}
				break;
			}
		}

		if (CoPendSem(messagesSem,0) == E_OK){

			int message = 0;
			for (;message < messageNumber;message++)
			{
				WriteDebugInfo(messages[message]);
			}
			messageNumber = 0;

			CoPostSem(messagesSem);
		}
		*/
		CoTimeDelay(0,0,1,0);
	}
}

Bool WriteDebugInformation(const char* sendBuffer, enum SortData sortData)
{
	switch (sortData)
	{
		case DirectDebug:
		{
			if (CoPendSem(messagesSem,0) == E_OK){
				if (messageNumber >= maxMessages-1)
				{
					return FALSE;
				}
				messages[messageNumber] = sendBuffer;
				messageNumber++;
				CoPostSem(messagesSem);
			}
			break;
		}
		case AngleX:
		{
			if (CoPendSem(angleSem,0) == E_OK){
				Angle[0] = sendBuffer;
				CoPostSem(angleSem);
			}
			break;
		}
		case AngleY:
		{
			if (CoPendSem(angleSem,0) == E_OK){
				Angle[1] = sendBuffer;
				CoPostSem(angleSem);
			}
			break;
		}
		case AngleZ:
		{
			if (CoPendSem(angleSem,0) == E_OK){
				Angle[2] = sendBuffer;
				CoPostSem(angleSem);
			}
			break;
		}
		case DistanceToGround:
		{
			if (CoPendSem(distanceToGroundSem,0) == E_OK){
				distanceToGround = sendBuffer;
				CoPostSem(distanceToGroundSem);
			}
			break;
		}
	}

	return TRUE;
}


void getFirstParameter(char* firstParameter)
{
	int i;
	int lengthParam = Strlen(firstParameter);
	char param[lengthParam];
	for (i=0;i<lengthParam;i++)
	{
		param[i] = firstParameter[i];
	}
	firstParam = param;
}
