#ifndef MAINTASK_H
#define MAINTASK_H

#include <lpc_types.h>
#include <Interfaces/Actuators/Actuators.h>
#include <CoOS.h>

#include <Tasks/Heartbeat/Heartbeat.h>
#include <Tasks/Distancetoground/DistanceToGroundTask.h>
#include <Tasks/Angle/AngleTask.h>
#include <Tasks/Temperature/Temperature.h>
#include <Tasks/Pressure/PressureTask.h>
#include <Tasks/Debug/DebugTask.h>

void MainTask (void* pdata);
void setOsTime (int osTime);
int getOsTime ();
Bool isAnswerFromUserYes();
Bool initializeSensors();
Bool initializeActuators();

#endif
