#ifndef SPEEDTASK_H
#define SPEEDTASK_H

#include <General/Taskmanager/Taskmanager.h>
#include <Interfaces/Actuators/Actuators.h>
#include <General/Filter/Filter.h>


char * printInfoSpeed(int argc, char *args[]);
void SpeedTask (void* pdata);
taskDef getSpeedTaskDefenition();

#endif