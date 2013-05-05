#ifndef BATTERYTASK_H
#define BATTERYTASK_H

#include <lpc_types.h>
#include <Interfaces/Actuators/Actuators.h>
#include <CoOS.h>

#include <Interfaces/Sensors/Sensors.h>
#include <General/Taskmanager/Taskmanager.h>

taskDef getBatteryTaskDefenition();
void BatteryTask (void* pdata);
char * getBattery(int argc, char *args[]);

#endif
