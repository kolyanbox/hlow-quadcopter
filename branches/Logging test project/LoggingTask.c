#include "LoggingTask.h"

#include <stdio.h>
#include <string.h>

logtable appLogTable[LOG_APP_AMOUNT];
loglevels currentLogLevel = LOG_DEBUG;
/* Id counter from 1 to 255 */
unsigned char ucIdCounter = 1;

void logInit()
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
         
    if((uacApp[0] == '\0') && (strlen(uacApp) < LOG_MAX_APP_NAME))
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
    strcpy(appLogTable[ucSlotFound].uacApp,uacApp);
    appLogTable[ucSlotFound].logLevel = logLevel;
    
    updateIdCounter();
    
    return appLogTable[ucSlotFound].ucId;
}

unsigned char deregisterApp(unsigned char ucAppId)
{
    unsigned char i = 0;
    
    if(ucAppId == 0)
        return 0;
        
    for(i = 0; i < LOG_APP_AMOUNT; i++)
    {
        if(appLogTable[i].ucId == ucAppId)
        {
            appLogTable[i].ucId = 0; /* 0 means this slot is free */
            appLogTable[i].uacApp[0] = '\0';
            appLogTable[i].logLevel = LOG_DEBUG;
            return 1;                       
        }      
    }           
    return 0;
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
                 printf("%s: %s\n",appLogTable[i].uacApp,uacLogMessage);
                 break;                            
             }                        
         }      
     }
}

void cmd_printLogTable()
{
    unsigned char i = 0;
    
    for(i = 0; i < LOG_APP_AMOUNT; i++)
    {
        if(appLogTable[i].ucId == 0)
        {
            printf("Slot %d: [empty]\n",(i+1));                                                   
        }
        else
        {
            printf("Slot %d: %s\n",(i+1),appLogTable[i].uacApp);     
        }      
    } 
}
