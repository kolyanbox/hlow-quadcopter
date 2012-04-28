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
