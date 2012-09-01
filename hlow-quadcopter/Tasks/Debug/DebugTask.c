#include <Tasks/Debug/DebugTask.h>

#include <Interfaces/Actuators/Actuators.h>
#include <CoOS.h>
#include <Interfaces/Sensors/Sensors.h>
#include <General/util.h>
#include <Drivers/Uart/Uart.h>

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

const char help[] = {"help"};

interface interfaces[MAXAMOUNTOFINTERFACES];
int currentAmmountOfInterfaces = 0;

#define DebugStackSize 256
OS_STK	Debug_stk[DebugStackSize];
taskDef debugTask;

taskDef t;
taskDef getDebugTaskDefenition()
{
	t.priority = 1;
	t.stk = &Debug_stk[DebugStackSize-1];
	t.stkSz = DebugStackSize;
	t.task = DebugTask;
	t.taskName = "Debug";
	return t;
}

char complete[MAXAMOUNTOFINTERFACES*(MAX_COMMAND_LENGTH+1)+1];
char * getAllCommands(int argc, char *args[])
{
	complete[0] = '\0';
	int j = 0;
	for (j=0;j<MAXAMOUNTOFINTERFACES;j++)
	{
		if (interfaces[j].occupiedSlot == TRUE)
		{
			Strcat(complete,interfaces[j].command);
			Strcat(complete,"\n");
		}
	}
	return complete;
}

Bool cliInitialization()
{
	int i;
	for (i = 0; i<MAXAMOUNTOFINTERFACES;i++)
	{
		interfaces[i].occupiedSlot = FALSE;
	}

	registerInterface(help,getAllCommands);

	return TRUE;
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

int registerInterface(const char* command, void *interface)
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
		WriteDebugInfo(interfaces[interfaceNumber].interface(interfaces[interfaceNumber].amountOfCommands, interfaces[interfaceNumber].parameters));
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
		int i = 0;
		if (strtok(command, delimiter) != NULL)
		{
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
				interfaces[j].amountOfCommands = i;
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
