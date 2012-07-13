#include <CoOS.h>
#include <General/util.h>
#include <Interfaces/Actuators/Actuators.h>
#include <Tasks/Debug/DebugTask.h>
#include <Tasks/Logging/LoggingTask.h>

logtable appLogTable[LOG_APP_AMOUNT];
loglevels currentLogLevel = LOG_DEBUG;
/* Id counter from 1 to 255 */
unsigned char ucIdCounter = 1;

Bool DebugTaskInitialization()
{
    unsigned char i = 0;
 
    /* Set id to one, zero means empty slot */
    ucIdCounter = 1;
    
    /* Reset everything */
    for(i = 0; i < LOG_APP_AMOUNT; i++)
    {
        appLogTable[i].ucId = 0; /* 0 means this slot is free */
        appLogTable[i].uacApp[0] = '\0';
        appLogTable[i].logLevel = LOG_DEBUG;      
    }    	
	return TRUE;
}

void LoggingTask (void* pdata)
{
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
     
     do
     {
         ucIdFree = 1;
         
         /* Update id counter */
         if(ucIdCounter == 255)
         {
             ucIdCounter = 1;               
         }
         else
         {
             ucIdCounter++;    
         }  
         
         /* Check if the id is in use */
         for(i = 0; i < LOG_APP_AMOUNT; i++)
         {
             if(appLogTable[i].ucId == ucIdCounter)
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
         
    if((uacApp[0] == '\0') && (Strlen(uacApp) < LOG_MAX_APP_NAME))
    {
        /* Return error */
        return 0;
    }          
    
    /* Look for free slot */
    for(i = 0; i < LOG_APP_AMOUNT; i++)
    {
          if(appLogTable[i].ucId == 0)
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
    
    appLogTable[ucSlotFound].ucId = ucIdCounter;  
    Strcpy(appLogTable[ucSlotFound].uacApp,uacApp);
    appLogTable[ucSlotFound].logLevel = logLevel;
    
    updateIdCounter();
    
    return appLogTable[ucSlotFound].ucId;
}

Bool deregisterApp(unsigned char ucAppId)
{
    unsigned char i = 0;
    
    if(ucAppId == 0)
        return FALSE;
        
    for(i = 0; i < LOG_APP_AMOUNT; i++)
    {
        if(appLogTable[i].ucId == ucAppId)
        {
            appLogTable[i].ucId = 0; /* 0 means this slot is free */
            appLogTable[i].uacApp[0] = '\0';
            appLogTable[i].logLevel = LOG_DEBUG;
            return TRUE;                       
        }      
    }           
    return FALSE;
}

void writeLog(unsigned char ucAppId, unsigned char *uacLogMessage, loglevels logLevel)
{
     unsigned char i = 0;
     
     if(ucAppId < 1)
         return;
     
     for(i = 0; i < LOG_APP_AMOUNT; i++)
     {
         if(appLogTable[i].ucId == ucAppId)
         {
             /* Only log when loglevel is greater */
             if(appLogTable[i].logLevel <= logLevel)
             {
                 WriteDebugInformation(appLogTable[i].uacApp,DirectDebug);
                 WriteDebugInformation(": ",DirectDebug);
                 WriteDebugInformation(uacLogMessage,DirectDebug);
                 WriteDebugInformation("\n",DirectDebug);
                 break;                            
             }                        
         }      
     }
}

void cmd_printLogTable()
{
    unsigned char i = 0;
    char cSlotNumber[2];
    
    for(i = 0; i < LOG_APP_AMOUNT; i++)
    {
        Itoa((i+1), cSlotNumber, 10);
        if(appLogTable[i].ucId == 0)
        {
            WriteDebugInformation("Slot ",DirectDebug); 
            WriteDebugInformation(cSlotNumber,DirectDebug); 
            WriteDebugInformation(": [empty]\n",DirectDebug);                                                    
        }
        else
        {
            WriteDebugInformation("Slot ",DirectDebug); 
            WriteDebugInformation(cSlotNumber,DirectDebug); 
            WriteDebugInformation(": ",DirectDebug); 
            WriteDebugInformation(appLogTable[i].uacApp,DirectDebug);    
            WriteDebugInformation("\n",DirectDebug);    
        }      
    } 
}

