#ifndef DEBUGTASK_H
#define DEBUGTASK_H

#include <lpc_types.h>
#include <General/Taskmanager/Taskmanager.h>

#define MAXAMOUNTOFINTERFACES 20
#define MAX_COMMAND_LENGTH 30

typedef struct {
    char **parameters;
    char* (*interface)(int totalParams, char *arguments[]);
    char command[MAX_COMMAND_LENGTH];
    int amountOfCommands;
    Bool occupiedSlot;
}interface;

#define maxMessages 50

enum SortData {
	DirectDebug = 0,
	DistanceToGround = 1,
	Temperature = 2,
	Pressure = 3,
	AngleX = 4,
	AngleY = 5,
	AngleZ = 6
};

int registerInterface(char* interfaceName, void *interface);
Bool callInterfaceById (int interfaceNumber);
taskDef getDebugTaskDefenition();
void DebugTask (void* pdata);
Bool DebugTaskInitialization();
void getFirstParameter(char* firstParameter);
#endif
