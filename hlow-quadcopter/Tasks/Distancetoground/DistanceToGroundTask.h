#ifndef DISTANCETOGROUNDTASK_H
#define DISTANCETOGROUNDTASK_H

#include <lpc_types.h>
#include <Interfaces/Actuators/Actuators.h>
#include <CoOS.h>
#include <Tasks/Debug/DebugTask.h>
#include <Interfaces/Sensors/Sensors.h>
#include <General/Taskmanager/Taskmanager.h>
#include <General/Filter/Filter.h>

taskDef getDistanceTaskDefenition();

void DistanceToGroundTask (void* pdata);

#endif
