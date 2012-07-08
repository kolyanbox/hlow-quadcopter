#include <Tasks/Debug/DebugTask.h>

int messageNumber = 0;
char* messages[maxMessages];
char* osTime;
char* Angle[3];

OS_EventID messagesSem;

Bool DebugTaskInitialization()
{
	messagesSem = CoCreateSem(1,1,EVENT_SORT_TYPE_FIFO);
	if (messagesSem == E_CREATE_FAIL)
	{
		return FALSE;
	}
	CoPostSem(messagesSem);
	return TRUE;
}

void DebugTask (void* pdata)
{
	for (;;)
	{
		switch (getCommand())
		{
			case (CommandRotationX):
			{
				WriteDebugInfo(AngleX);
				WriteDebugInfo("\n\r");
			}
			case (CommandRotationY):
			{
				WriteDebugInfo(AngleY);
				WriteDebugInfo("\n\r");
			}
			case (CommandRotationZ):
			{
				WriteDebugInfo(AngleZ);
				WriteDebugInfo("\n\r");
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
			CoTickDelay(10);
		}
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
		}
		case OsTime:
		{
			osTime = sendBuffer;
		}
		case AngleX:
		{
			Angle[0] = sendBuffer;
		}
		case AngleY:
		{
			Angle[1] = sendBuffer;
		}
		case AngleZ:
		{
			Angle[2] = sendBuffer;
		}
	}

	return TRUE;
}
