#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <lpc_types.h>
#include <Interfaces/Actuators/Actuators.h>
#include <CoOS.h>

#include <Interfaces/Sensors/Sensors.h>
#include <Tasks/Debug/DebugTask.h>

void TemperatureTask (void* pdata);

#endif
