#ifndef ANGLETASK_H
#define ANGLETASK_H

#include <lpc_types.h>
#include <Interfaces/Actuators/Actuators.h>
#include <CoOS.h>

#include <Interfaces/Sensors/Sensors.h>
#include <General/Taskmanager/Taskmanager.h>

taskDef getAngleTaskDefenition();
void AngleTask (void* pdata);
char * getAngle(int argc, char *args[]);

#endif
