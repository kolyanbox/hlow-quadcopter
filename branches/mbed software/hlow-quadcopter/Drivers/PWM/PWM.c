#include <Drivers/PWM/PWM.h>


/*Global variables
COX_PWM_PI *pwm = &pi_pwm;

Bool SetPWM(uint16_t port, float dutyCycle){
	pwm->Duty(port, dutyCycle);
	return TRUE;
}
*/
//Bool PWMInit(uint16_t port1, uint16_t port2, uint16_t port3, uint16_t port4, float periodTime){
	/* Initialize the PWM peripheral*/
	//if (pwm->Init() != COX_SUCCESS)
	//{
	//	return FALSE;
	//}

	/* Configure PWM Period*/
	//if (pwm->Period(port1, periodTime) != COX_SUCCESS)
	//{
	//	return FALSE;
	//}

	/*set motors neutral*/
	//SetPWM(port1,0);
	//SetPWM(port2,0);
	//SetPWM(port3,0);
	//SetPWM(port4,0);

	/* Start PWM Output
	if (pwm->Start(port1) != COX_SUCCESS)
	{
		return FALSE;
	}
	if (pwm->Start(port2) != COX_SUCCESS)
	{
		return FALSE;
	}
	if (pwm->Start(port3) != COX_SUCCESS)
	{
		return FALSE;
	}
	if (pwm->Start(port4) != COX_SUCCESS)
	{
		return FALSE;
	}

	return TRUE;
}
*/
