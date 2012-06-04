#include <Tasks/Debug/DebugTask.h>

int messageNumber = 0;
char* messages[maxMessages];
char* osTime;

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
		getCommand();
		if (CoPendSem(messagesSem,0) == E_OK){

			int message = 0;
			for (;message < messageNumber;message++)
			{
				WriteDebugInfo(messages[message]);
			}
			messageNumber = 0;

			CoTickDelay(10);

			CoPostSem(messagesSem);
		}
	}
}

Bool WriteDebugInformation(const char* sendBuffer, enum SortData sortData)
{
	switch (sortData)
	{
		case Other:
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
	}

	return TRUE;
}
