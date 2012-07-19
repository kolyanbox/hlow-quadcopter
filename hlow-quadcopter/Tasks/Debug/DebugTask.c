#include <Tasks/Debug/DebugTask.h>

#include <Interfaces/Actuators/Actuators.h>
#include <CoOS.h>
#include <Interfaces/Sensors/Sensors.h>

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

#define DebugStackSize 256
OS_STK	Debug_stk[DebugStackSize];
taskDef debugTask;

taskDef t;
taskDef getDebugTaskDefenition()
{
	t.priority = 63;
	t.stk = &Debug_stk[DebugStackSize-1];
	t.stkSz = DebugStackSize;
	t.task = DebugTask;
	t.taskName = "Debug";
	return t;
}

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
		char *c[MAX_COMMAND_LENGTH];
		char command[MAX_COMMAND_LENGTH];
		Strcpy(command, lastReceivedCommand());
		char *delimiter = " ";
		if (strtok(command, delimiter) != NULL)
		{

			int i = 0;
			while ((c[i] = strtok(NULL, delimiter)) != NULL)
			{
				i++;
			}
		}
		int j;
		for (j=0;j<MAXAMOUNTOFINTERFACES;j++)
		{
			if (interfaces[j].occupiedSlot == TRUE && Strcmp(interfaces[j].command,strtok(command, delimiter)) == 0)
			{
				interfaces[j].parameters = c;
				callInterfaceById(j);
				WriteDebugInfo("\n\r/>");
				break;
			}
			else if (j == MAXAMOUNTOFINTERFACES-1 && command[0] != '\0')
			{
				WriteDebugInfo("\n\r/>");
			}
		}
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
