#ifndef LED_H_
#define LED_H_

#include "lpc_types.h"
#include "lpc17xx_gpio.h"

Bool GPIO_init(uint32_t pin, uint8_t port, uint8_t direction);
void GPIO_set(uint8_t port, uint32_t pin, Bool enable);


#endif
