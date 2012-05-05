#include <Interfaces/Actuators/Actuators.h>

Bool ActuatorsInitialization(void)
{
	/*Initialize Uart for debug perposes*/
	UARTInit(LPC_UART0, 115200);
	/*Initialize leds*/
	GPIO_init(led1, 1, 1);
	GPIO_init(led2, 1, 1);
	GPIO_init(led3, 1, 1);
	GPIO_init(led4, 1, 1);
	/*Initialize PWM*/

	if (initializePWM(motor2, PERIOD) == FALSE)
	{
		setLed(3,ENABLE);
	}
	if (initializePWM(motor3, PERIOD) == FALSE)
	{
		setLed(4,ENABLE);
	}
	if (initializePWM(motor4, PERIOD) == FALSE)
	{
		setLed(1,ENABLE);
	}
	if (initializePWM(motor1, PERIOD) == FALSE)
	{
		setLed(3,ENABLE);
	}

	setSpeedBack(1);
	setSpeedFront(1);
	setSpeedLeft(1);
	setSpeedRight(1);
	setLed(led2,DISABLE);
	return TRUE;
}

Bool WriteDebugInfo(const char * sendBuffer)
{
	UART_Send(LPC_UART0, (uint8_t *)sendBuffer, Strlen(sendBuffer), BLOCKING);
	return TRUE;
}

Bool setLed(enum LED led ,Bool enable)
{
	GPIO_set(1,led,enable);
	return TRUE;
}

Bool setSpeedFront(int speedPercent)
{
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
