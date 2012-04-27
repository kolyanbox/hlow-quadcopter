#include <Controller/Controller.h>
float getOutputPID(struct PID *pid)
{
	float error = pid->setpoint - pid->processInput;
	pid->integral += error*pid->dt;
	float derivative = (error - pid->previousError)/pid->dt;
	float output = (pid->P*error) + (pid->I*pid->integral) + (pid->D* derivative);
	pid->previousError = error;
	if (output > 100)
	{
		output = 100;
	}
	if (output < -100)
	{
		output = -100;
	}
	return output;
}
