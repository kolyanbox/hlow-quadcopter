#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <lpc_types.h>
#include <Interfaces/Actuators/Actuators.h>
#include <CoOS.h>
#include <General/Taskmanager/Taskmanager.h>

taskDef getHeartbeatTaskDefenition();
void HeartBeat (void* pdata);

#endif
