#ifndef MAINTASK_H
#define MAINTASK_H

#include <lpc_types.h>
#include <General/Taskmanager/Taskmanager.h>

taskDef getMainTaskDefenition();
void MainTask (void* pdata);
void setOsTime (int osTime);
int getOsTime ();
Bool isAnswerFromUserYes();
Bool initializeSensors();
Bool initializeActuators();

#endif
