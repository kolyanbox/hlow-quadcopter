#include <CoOS.h>
#include <General/util.h>
#include <Interfaces/Actuators/Actuators.h>
#include <Tasks/Debug/DebugTask.h>
#include <Tasks/Logging/LoggingTask.h>
logtable *pAppLogTable = NULL;
unsigned char *pucIdCounter = NULL;

taskDef t;
#define LoggingStackSize 128
OS_STK	Logging_stk[LoggingStackSize];
taskDef loggingTask;

taskDef getLoggingTaskDefenition()
{
	t.priority = 63;
	t.stk = &Logging_stk[LoggingStackSize-1];
	t.stkSz = LoggingStackSize;
	t.task = LoggingTask;
	t.taskName = "Logging";
	return t;
}

Bool LoggingTaskInitialization()
{
    unsigned char i = 0;
    
    if(pAppLogTable)
    {
		/* Reset everything */
		for(i = 0; i < LOG_APP_AMOUNT; i++)
		{
			pAppLogTable[i].ucId = 0; /* 0 means this slot is free */
			pAppLogTable[i].uacApp[0] = '\0';
			pAppLogTable[i].logLevel = LOG_DEBUG;
		}
		return TRUE;
    }
    return FALSE;
}

void LoggingTask (void* pdata)
{
	logtable appLogTable[LOG_APP_AMOUNT];
	/* Id counter from 1 to 255 */
	unsigned char ucIdCounter = 1;

	/* Address global pointers */
	pAppLogTable = appLogTable;
	pucIdCounter = &ucIdCounter;

	LoggingTaskInitialization();
	for (;;)
	{
        CoTimeDelay(0,0,1,0);
    }
}

/* Function not in header file, only allowed to be used from this file */
void updateIdCounter()
{
     unsigned char ucIdOk = 0;
     unsigned char ucIdFree = 1;
     unsigned char i = 0;
     
     if(pucIdCounter == NULL || pAppLogTable == NULL)
    	 return;

     do
     {
         ucIdFree = 1;
         
         /* Update id counter */
         if(*pucIdCounter == 255)
         {
        	 *pucIdCounter = 1;
         }
         else
         {
        	 *pucIdCounter = *pucIdCounter + 1;
         }  
         
         /* Check if the id is in use */
         for(i = 0; i < LOG_APP_AMOUNT; i++)
         {
             if(pAppLogTable[i].ucId == *pucIdCounter)
             {
                 ucIdFree = 0;
                 break;                       
             }     
         }    
         
         /* If the id isn't in use, it is ok */
         if(ucIdFree)
         {
             ucIdOk = 1;            
         }
     } while(!ucIdOk);
}

unsigned char registerApp(unsigned char *uacApp, loglevels logLevel)
{
    unsigned char ucSlotFound = 0;
    unsigned char i = 0;

    if(pAppLogTable == NULL)
        return 0;
         
    if((uacApp[0] == '\0') && (Strlen((char *)uacApp) < LOG_MAX_APP_NAME))
    {
        /* Return error */
        return 0;
    }          
    
    /* Look for free slot */
    for(i = 0; i < LOG_APP_AMOUNT; i++)
    {
          if(pAppLogTable[i].ucId == 0)
          {
              ucSlotFound = 1;
              break;                         
          }
    }
    
    /* No slots available */
    if(!ucSlotFound)
    {
        /* Return error */
        return 0;                
    }
    
    /* Set slot to found slot */
    ucSlotFound = i;
    
    pAppLogTable[ucSlotFound].ucId = *pucIdCounter;
    Strcpy((char *)pAppLogTable[ucSlotFound].uacApp,(char *)uacApp);
    pAppLogTable[ucSlotFound].logLevel = logLevel;
    
    updateIdCounter();
    
    return pAppLogTable[ucSlotFound].ucId;
}

Bool deregisterApp(unsigned char ucAppId)
{
    unsigned char i = 0;
    
    if(pAppLogTable == NULL)
        return FALSE;

    if(ucAppId == 0)
        return FALSE;
        
    for(i = 0; i < LOG_APP_AMOUNT; i++)
    {
        if(pAppLogTable[i].ucId == ucAppId)
        {
        	pAppLogTable[i].ucId = 0; /* 0 means this slot is free */
        	pAppLogTable[i].uacApp[0] = '\0';
        	pAppLogTable[i].logLevel = LOG_DEBUG;
            return TRUE;                       
        }      
    }           
    return FALSE;
}

void writeLog(unsigned char ucAppId, unsigned char *uacLogMessage, loglevels logLevel)
{
     unsigned char i = 0;
     
     if(pAppLogTable == NULL)
    	 return;

     if(ucAppId < 1)
         return;
     
     for(i = 0; i < LOG_APP_AMOUNT; i++)
     {
         if(pAppLogTable[i].ucId == ucAppId)
         {
             /* Only log when loglevel is greater */
             if(pAppLogTable[i].logLevel <= logLevel)
             {
//                 WriteDebugInformation((char *)pAppLogTable[i].uacApp,DirectDebug);
//                 WriteDebugInformation(": ",DirectDebug);
//                 WriteDebugInformation((char *)uacLogMessage,DirectDebug);
//                 WriteDebugInformation("\n",DirectDebug);
                 break;                            
             }                        
         }      
     }
}

void cmd_printLogTable()
{
    unsigned char i = 0;
    char cSlotNumber[2];
    
    if(pAppLogTable == NULL)
    	return;

    for(i = 0; i < LOG_APP_AMOUNT; i++)
    {
        Itoa((i+1), cSlotNumber, 10);
        if(pAppLogTable[i].ucId == 0)
        {
//            WriteDebugInformation("Slot ",DirectDebug);
//            WriteDebugInformation(cSlotNumber,DirectDebug);
//            WriteDebugInformation(": [empty]\n\r",DirectDebug);
        }
        else
        {
//            WriteDebugInformation("Slot ",DirectDebug);
//            WriteDebugInformation(cSlotNumber,DirectDebug);
//            WriteDebugInformation(": ",DirectDebug);
//            WriteDebugInformation((char *)pAppLogTable[i].uacApp,DirectDebug);
//            WriteDebugInformation("\n\r",DirectDebug);
        }      
    } 
}

