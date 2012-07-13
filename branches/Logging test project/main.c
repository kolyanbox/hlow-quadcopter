#include <stdio.h>
#include <stdlib.h>

#include "LoggingTask.h"

int main(int argc, char *argv[])
{
    unsigned char test1Id = 0;
    unsigned char test2Id = 0;
    unsigned char test3Id = 0;
    
    /* Init log system */
    logInit();
    
    test1Id = registerApp("app1",LOG_WARNING);
    if(test1Id < 1)
    {
       printf("Failed to register app1 to logging service.\n");           
    }
    writeLog(test1Id,"Debug log app 1",LOG_DEBUG);
    writeLog(test1Id,"Warning log app 1",LOG_WARNING);
    writeLog(test1Id,"Critical log app 1",LOG_CRIT);
    
    test2Id = registerApp("app2",LOG_EMERG);
    if(test2Id < 1)
    {
       printf("Failed to register app2 to logging service.\n");           
    }
    writeLog(test2Id,"Debug log app 2",LOG_DEBUG);
    writeLog(test2Id,"Emergency log app 2",LOG_EMERG);
    writeLog(test2Id,"Critical log app 2",LOG_CRIT);
    
    cmd_printLogTable();
    
    deregisterApp(test1Id);
    /* Shouldn't work */
    writeLog(test1Id,"Deregistered shouldn't work.",LOG_WARNING);
    
    test3Id = registerApp("app3",LOG_ERR);
    if(test3Id < 1)
    {
       printf("Failed to register app2 to logging service.\n");           
    }
    writeLog(test3Id,"Debug log app 3",LOG_DEBUG);
    writeLog(test3Id,"Error log app 3",LOG_ERR);
    writeLog(test3Id,"Critical log app 3",LOG_CRIT);
    
    cmd_printLogTable();
  
    system("PAUSE");	
    return 0;
}
