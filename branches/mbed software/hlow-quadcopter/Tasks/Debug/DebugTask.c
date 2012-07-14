#include <Tasks/Debug/DebugTask.h>

#include <Interfaces/Actuators/Actuators.h>
#include <CoOS.h>
#include <Interfaces/Sensors/Sensors.h>

int messageNumber = 0;
char* messages[maxMessages];

OS_EventID messagesSem;

//Angle semaphore
OS_EventID angleSem;
char* Angle[3];

//distance to ground semaphore
OS_EventID distanceToGroundSem;
char* distanceToGround;

Bool DebugTaskInitialization()
{
	messagesSem = CoCreateSem(1,1,EVENT_SORT_TYPE_FIFO);
	if (messagesSem == E_CREATE_FAIL)
	{
		return FALSE;
	}
	CoPostSem(messagesSem);

	angleSem = CoCreateSem(1,1,EVENT_SORT_TYPE_FIFO);
	if (angleSem == E_CREATE_FAIL)
	{
		return FALSE;
	}
	CoPostSem(angleSem);

	distanceToGroundSem = CoCreateSem(1,1,EVENT_SORT_TYPE_FIFO);
	if (distanceToGroundSem == E_CREATE_FAIL)
	{
		return FALSE;
	}
	CoPostSem(distanceToGroundSem);

	return TRUE;
}

void DebugTask (void* pdata)
{
	WriteDebugInfo("/>");
	for (;;)
	{
		switch (getCommand())
		{
			case (CommandRotationX):
			{
				if (CoPendSem(messagesSem,0) == E_OK){
					WriteDebugInfo(Angle[0]);
					CoPostSem(messagesSem);
				}
				WriteDebugInfo("\n\r/>");
				break;
			}
			case (CommandRotationY):
			{
				if (CoPendSem(messagesSem,0) == E_OK){
					WriteDebugInfo(Angle[1]);
					CoPostSem(messagesSem);
				}
				WriteDebugInfo("\n\r/>");
				break;
			}
			case (CommandRotationZ):
			{
				if (CoPendSem(messagesSem,0) == E_OK){
					WriteDebugInfo(Angle[2]);
					CoPostSem(messagesSem);
				}
				WriteDebugInfo("\n\r/>");
				break;
			}
			case (commandOsTime):
			{
				char osTime[10];
				int t = CoGetOSTime();
				Itoa(t,osTime,10);
				WriteDebugInfo(osTime);
				WriteDebugInfo("\n\r/>");
				break;
			}
			case (commandDistanceToGround):
			{
				WriteDebugInfo("3");
				if (CoPendSem(distanceToGroundSem,0) == E_OK){
					WriteDebugInfo(distanceToGround);
					CoPostSem(distanceToGroundSem);
				}
				WriteDebugInfo("\n\r/>");
				break;
			}
		}

		if (CoPendSem(messagesSem,0) == E_OK){

			int message = 0;
			for (;message < messageNumber;message++)
			{
				WriteDebugInfo(messages[message]);
			}
			messageNumber = 0;

			CoPostSem(messagesSem);
		}
		CoTimeDelay(0,0,1,0);
	}
}

Bool WriteDebugInformation(const char* sendBuffer, enum SortData sortData)
{
	switch (sortData)
	{
		case DirectDebug:
		{
			if (CoPendSem(messagesSem,0) == E_OK){
				if (messageNumber >= maxMessages-1)
				{
					return FALSE;
				}
				messages[messageNumber] = sendBuffer;
				messageNumber++;
				CoPostSem(messagesSem);
			}
			break;
		}
		case AngleX:
		{
			if (CoPendSem(angleSem,0) == E_OK){
				Angle[0] = sendBuffer;
				CoPostSem(angleSem);
			}
			break;
		}
		case AngleY:
		{
			if (CoPendSem(angleSem,0) == E_OK){
				Angle[1] = sendBuffer;
				CoPostSem(angleSem);
			}
			break;
		}
		case AngleZ:
		{
			if (CoPendSem(angleSem,0) == E_OK){
				Angle[2] = sendBuffer;
				CoPostSem(angleSem);
			}
			break;
		}
		case DistanceToGround:
		{
			if (CoPendSem(distanceToGroundSem,0) == E_OK){
				distanceToGround = sendBuffer;
				CoPostSem(distanceToGroundSem);
			}
			break;
		}
	}

	return TRUE;
}
