#ifndef DEBUGTASK_H
#define DEBUGTASK_H

#include <lpc_types.h>

#define MAXAMOUNTOFINTERFACES 20
#define MAX_COMMAND_LENGTH 30

typedef struct {
    char **parameters;
    char* (*interface)(char *arguments[]);
    char command[MAX_COMMAND_LENGTH];
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

void DebugTask (void* pdata);
Bool DebugTaskInitialization();
Bool WriteDebugInformation(const char* sendBuffer, enum SortData sortData);
void getFirstParameter(char* firstParameter);
#endif
