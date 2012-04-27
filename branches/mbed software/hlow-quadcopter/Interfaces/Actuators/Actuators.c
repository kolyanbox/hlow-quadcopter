#include <Interfaces/Actuators/Actuators.h>

Bool InitializeActuators(void)
{
	DEBUG_Init(115200);
	//InitializePWM();

	return TRUE;
}
Bool WriteDebugInfo(const char * sendBuffer)
{
	DEBUG_Send(sendBuffer);
	return TRUE;
}

Bool InitializePWM()
{
	return TRUE;//PWMInit(pwm1,pwm2,pwm3, pwm4, 0.018);
}

Bool setSpeedFront(int Percent)
{return TRUE;
	/*float fSpeed = 0.0;

	If input variables aren't correct
	if (Percent < 0 || Percent > 100)
	{
		return FALSE;
	}

	if (Percent == 0)
	{
		Percent++;
	}

	fSpeed = SPEED_PROCENT * (Percent-1)+SPEED_LOW;

	SetPWM(pwm1,fSpeed);

	return TRUE;*/
}
