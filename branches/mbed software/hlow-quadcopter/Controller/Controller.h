#include <Interfaces/Actuators/Actuators.h>
#include <General/util.h>

struct PID {
   float P;
   float I;
   float D;
   float dt;
   float processInput;
   float integral;
   float previousError;
   float setpoint;
};

/*Get an output for the PID*/
float getOutputPID(struct PID *pid);
