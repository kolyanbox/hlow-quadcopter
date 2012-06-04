#ifndef DEBUGTASK_H
#define DEBUGTASK_H

#define maxMessages 50

#include <lpc_types.h>
#include <Interfaces/Actuators/Actuators.h>
#include <CoOS.h>

enum SortData {
	Other = 0,
	DistanceToGround = 1,
	Temperature = 2,
	Pressure = 3,
	OsTime = 4,
};

void DebugTask (void* pdata);
Bool DebugTaskInitialization();
Bool WriteDebugInformation(const char* sendBuffer, enum SortData sortData);
#endif
