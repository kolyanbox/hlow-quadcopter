#include <Controller/MotorControllerTask.h>

int prescaler = 12;

void ControllerTaskOne (void* pdata)
{
	struct PID pid;
	pid.P = 0.6;
	pid.I = 0.2;
	pid.D = 0.125;
	pid.dt = 0.2;
	pid.integral = 0;
	pid.setpoint = 2000/prescaler;
	pid.previousError = 0;
	uint16_t value = 0;
	int bInitialized = 0;
	for (;;)
	{
		if (CoTimeDelay(0,0,0,200) == E_OK)
		{
			//Get value from X ax of accelero
			//getValue(accelero, 1, &value);
			pid.processInput = value/prescaler;

			//Get output
			float output = getOutputPID(&pid);


			if (output > 30)
			{
				bInitialized = 1;
			}

			if (output > 50)
			{
				output = 50;
			}


			if (bInitialized == 1 && output < 30){
				output = 30;
			}

			SetMotor(motor1, output);

			//Ftoa(output,print,5,10);
			//DEBUG_Send("MotorOne: ");
			//DEBUG_Send(print);
			//DEBUG_Send("\n");
		}
		else
		{
			WriteDebugInfo("fail\n");
		}
	}
}

void ControllerTaskTwo (void* pdata)
{
	struct PID pid;
	pid.P = 0.6;
	pid.I = 0.5;
	pid.D = 0.125;
	pid.dt = 1;
	pid.integral = 0;
	pid.setpoint = 1;
	pid.previousError = 0;
	for (;;)
	{
		if (CoTimeDelay(0,0,1,0) == E_OK)
		{
			char print[6];
			pid.processInput = 0;
			Ftoa(getOutputPID(&pid),print,5,10);
			//DEBUG_Send("MotorTwo: ");
			//DEBUG_Send(print);
			//DEBUG_Send("\n");
		}
		else
		{
			WriteDebugInfo("fail\n");
		}
	}
}

void ControllerTaskThree (void* pdata)
{
	struct PID pid;
	pid.P = 0.6;
	pid.I = 0.2;
	pid.D = 0.125;
	pid.dt = 0.2;
	pid.integral = 0;
	pid.setpoint = 2000/prescaler;
	pid.previousError = 0;
	uint16_t value = 0;
	int bInitialized = 0;

	for (;;)
	{
		if (CoTimeDelay(0,0,0,200) == E_OK)
		{
			//Get value from X ax of accelero
			//getValue(accelero, 1, &value);
			pid.processInput = value/prescaler;
			//Get output
			float output = getOutputPID(&pid);

			output = -1*output;

			if (output > 30)
			{
				bInitialized = 1;
			}

			if (output > 50)
			{
				output = 50;
			}

			if (output < 0)
				output = 0;

			if (bInitialized == 1 && output < 30){
				output = 30;
			}

			SetMotor(motor3, (int)output);

			//char print[6];
			//pid.processInput = 0;
			//Ftoa(output,print,5,10);
			//DEBUG_Send("MotorThree: ");
			//DEBUG_Send(print);
			//DEBUG_Send("\n");
		}
		else
		{
			WriteDebugInfo("fail\n");
		}
	}
}
void ControllerTaskFour (void* pdata)
{
	struct PID pid;
	pid.P = 0.6;
	pid.I = 0.5;
	pid.D = 0.125;
	pid.dt = 1;
	pid.integral = 0;
	pid.setpoint = 1;
	pid.previousError = 0;
	for (;;)
	{
		if (CoTimeDelay(0,0,1,0) == E_OK)
		{
			char print[6];
			pid.processInput = 0;
			Ftoa(getOutputPID(&pid),print,5,10);
			//DEBUG_Send("MotorFour: ");
			//DEBUG_Send(print);
			//DEBUG_Send("\n");
		}
		else
		{
			WriteDebugInfo("fail\n");
		}
	}
}
