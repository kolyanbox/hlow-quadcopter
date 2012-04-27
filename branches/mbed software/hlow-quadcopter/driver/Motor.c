#include <driver/Motor.h>
#include <lpc_pwm.h>

/*Global variables*/
COX_PWM_PI *pwm = &pi_pwm;

Bool SetMotor(enum MOTOR Motor, int Percent){

	float fSpeed = 0.0;

	/*If input variables aren't correct*/
	if (Percent < 0 || Percent > 100)
	{
		return FALSE;
	}

	if (Percent == 0){
		Percent++;;
	}

	fSpeed = SPEED_PROCENT * (Percent-1)+SPEED_LOW;

	pwm->Duty(Motor, fSpeed);

	return TRUE;
}

Bool MotorInit(void){
	/* Initialize the PWM peripheral*/
	if (pwm->Init() != COX_SUCCESS)
	{
		return FALSE;
	}

	/* Configure PWM Period*/
	if (pwm->Period(motor1, 0.018) != COX_SUCCESS)
	{
		return FALSE;
	}

	/*set motors neutral*/
	SetMotor(motor1,0);
	SetMotor(motor2,0);
	SetMotor(motor3,0);
	SetMotor(motor4,0);

	/* Start PWM Output */
	if (pwm->Start(motor1) != COX_SUCCESS)
	{
		return FALSE;
	}
	if (pwm->Start(motor2) != COX_SUCCESS)
	{
		return FALSE;
	}
	if (pwm->Start(motor3) != COX_SUCCESS)
	{
		return FALSE;
	}
	if (pwm->Start(motor4) != COX_SUCCESS)
	{
		return FALSE;
	}

	return TRUE;
}
