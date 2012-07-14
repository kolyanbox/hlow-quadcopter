#ifndef DEBUGTASK_H
#define DEBUGTASK_H

#include <lpc_types.h>

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

void DebugTask (void* pdata);
Bool DebugTaskInitialization();
Bool WriteDebugInformation(const char* sendBuffer, enum SortData sortData);
#endif
