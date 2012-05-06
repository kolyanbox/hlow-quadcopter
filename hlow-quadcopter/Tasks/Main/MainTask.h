#ifndef MAINTASK_H
#define MAINTASK_H

#include <lpc_types.h>
#include <Interfaces/Actuators/Actuators.h>
#include <CoOS.h>

#include <Tasks/Heartbeat/Heartbeat.h>
#include <Tasks/Distancetoground/DistanceToGroundTask.h>
#include <Tasks/Angle/AngleTask.h>

void MainTask (void* pdata);

#endif
