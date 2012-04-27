#include <Drivers/GPIO/GPIO.h>

void GPIO_init(uint32_t pin, uint8_t port, uint8_t direction)
{
	GPIO_SetDir(1, pin, 1);
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


