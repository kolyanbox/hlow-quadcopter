#include <Interfaces/Actuators/Actuators.h>

Bool correctlyInitializedActuatorLeds = FALSE;
Bool correctlyInitializedActuatorMotors = FALSE;
Bool correctlyInitializedActuatorUart = FALSE;

Bool ActuatorsInitialization(enum ActuatorType actuatorType)
{
	switch (actuatorType)
	{
		case (ActuatorUart):{
			/*Initialize Uart for debug purposes*/
			if (UARTInit(LPC_UART0, 115200) == FALSE)
			{
				correctlyInitializedActuatorUart = FALSE;
				return FALSE;
			}
			correctlyInitializedActuatorUart = TRUE;
			return TRUE;
		}
		case (ActuatorLeds):{
			/*Initialize leds if one fails the other leds won't initialize*/
			if (GPIO_init(led4, 1, 1) == FALSE ||
				GPIO_init(led3, 1, 1) == FALSE ||
				GPIO_init(led2, 1, 1) == FALSE ||
				GPIO_init(led1, 1, 1) == FALSE)
			{
				correctlyInitializedActuatorLeds = FALSE;
				return FALSE;
			}
			correctlyInitializedActuatorLeds = TRUE;
			return TRUE;
		}
		case (ActuatorMotors):{
			/*Initialize PWM. If one fails the others won't initialize*/
			if (initializePWM(motor2, PERIOD) == FALSE ||
				initializePWM(motor3, PERIOD) == FALSE ||
				initializePWM(motor4, PERIOD) == FALSE ||
				initializePWM(motor1, PERIOD) == FALSE)
			{
				correctlyInitializedActuatorMotors = FALSE;
				return FALSE;
			}

			/*Set all speeds on 1 percent for initializing the ESC's
			 * Initializing will stop if one speed cannot be set*/
			correctlyInitializedActuatorMotors = TRUE;
			if (setSpeedBack(1) == FALSE ||
				setSpeedFront(1) == FALSE ||
				setSpeedLeft(1) == FALSE ||
				setSpeedRight(1) == FALSE)
			{
				correctlyInitializedActuatorMotors = FALSE;
				return FALSE;
			}
			correctlyInitializedActuatorMotors = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

Bool WriteDebugInfo(const char * sendBuffer)
{
	if (correctlyInitializedActuatorUart == FALSE)
	{
		return FALSE;
	}
	UART_Send(LPC_UART0, (uint8_t *)sendBuffer, Strlen(sendBuffer), BLOCKING);
	return TRUE;
}

Bool setLed(enum LED led ,Bool enable)
{
	if (correctlyInitializedActuatorLeds == FALSE)
	{
		return FALSE;
	}
	GPIO_set(1,led,enable);
	return TRUE;
}

Bool setSpeedFront(int speedPercent)
{
	if (correctlyInitializedActuatorMotors == FALSE)
	{
		return FALSE;
	}
	float fSpeed = 0.0;

	/*If input variables aren't correct*/
	if (speedPercent < 0 || speedPercent > 100)
	{
		return FALSE;
	}

	if (speedPercent == 0){
		speedPercent++;;
	}

	fSpeed = SPEED_PROCENT * (speedPercent-1)+SPEED_LOW;

	setPWM(motor3, fSpeed);
	return TRUE;
}

Bool setSpeedBack(int speedPercent)
{
	if (correctlyInitializedActuatorMotors == FALSE)
	{
		return FALSE;
	}
	float fSpeed = 0.0;

	/*If input variables aren't correct*/
	if (speedPercent < 0 || speedPercent > 100)
	{
		return FALSE;
	}

	if (speedPercent == 0){
		speedPercent++;;
	}

	fSpeed = SPEED_PROCENT * (speedPercent-1)+SPEED_LOW;

	setPWM(motor1, fSpeed);
	return TRUE;
}

Bool setSpeedLeft(int speedPercent)
{
	if (correctlyInitializedActuatorMotors == FALSE)
	{
		return FALSE;
	}
	float fSpeed = 0.0;

	/*If input variables aren't correct*/
	if (speedPercent < 0 || speedPercent > 100)
	{
		return FALSE;
	}

	if (speedPercent == 0){
		speedPercent++;;
	}

	fSpeed = SPEED_PROCENT * (speedPercent-1)+SPEED_LOW;

	setPWM(motor2, fSpeed);
	return TRUE;
}

Bool setSpeedRight(int speedPercent)
{
	if (correctlyInitializedActuatorMotors == FALSE)
	{
		return FALSE;
	}
	float fSpeed = 0.0;

	/*If input variables aren't correct*/
	if (speedPercent < 0 || speedPercent > 100)
	{
		return FALSE;
	}

	if (speedPercent == 0){
		speedPercent++;;
	}

	fSpeed = SPEED_PROCENT * (speedPercent-1)+SPEED_LOW;

	setPWM(motor4, fSpeed);
	return TRUE;
}
