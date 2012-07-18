#include <Drivers/PWM/PWM.h>

COX_PWM_PI *pwm = &pi_pwm;
Bool initialized = FALSE;
Bool initializePWM(uint16_t pwmPin, float period){
	if (!initialized)
		{
		/* Initialize the PWM peripheral*/
		if (pwm->Init() != COX_SUCCESS)
		{
			return FALSE;
		}

		/* Configure PWM Period*/
		if (pwm->Period(pwmPin, period) != COX_SUCCESS)
		{
			return FALSE;
		}
		initialized = TRUE;
	}
	/*set PWM neutral*/
	setPWM(pwmPin,0);

	/* Start PWM Output */
	if (pwm->Start(pwmPin) != COX_SUCCESS)
	{
		return FALSE;
	}
	return TRUE;
}

Bool setPWM(uint16_t pwmPin, float duty){
	if (pwm->Duty(pwmPin, duty) == COX_SUCCESS)
	{
		return TRUE;
	}
	return FALSE;
}
