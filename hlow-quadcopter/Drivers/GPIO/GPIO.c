#include <Drivers/GPIO/GPIO.h>

Bool GPIO_init(uint32_t pin, uint8_t port, uint8_t direction)
{
	GPIO_SetDir(port, pin, direction);
	return TRUE;
}

void GPIO_set(uint8_t port, uint32_t pin, Bool enable)
{
	if (enable == TRUE)
	{
		GPIO_SetValue(port, pin);
	}
	else
	{
		GPIO_ClearValue(port, pin);
	}
}


