#include <Interfaces/Actuators/Actuators.h>

Bool ActuatorsInitialization(void)
{
	/*Initialize Uart for debug perposes*/
	UARTInit(LPC_UART0, 115200);
	/*Initialize leds*/
	GPIO_init(1, led1, 1);
	GPIO_init(1, led2, 1);
	GPIO_init(1, led3, 1);
	GPIO_init(1, led4, 1);
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
