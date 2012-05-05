#ifndef STARTUP_H_
#define STARTUP_H_

#include <lpc_types.h>
#include <System/Init.h>

enum ERRORMESSAGES{
	motorstartfail = 1
};

/*Function to startup all vital tasks and check if they work
 * Returns 0 if succeeded
 * If this function doesn't return 0, make a call to void StopQuadCopterImmediate(void);
 * to safely stop the quadcopters functions*/
int StartUpQuadCopter(void);

/*
 * This functions will stop all functions of the quadcopter
 * Returns true if succeeding to stop and false otherwise
 * */
Bool StopQuadCopterImmediate(void);

#endif
