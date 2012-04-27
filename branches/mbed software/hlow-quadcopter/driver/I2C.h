#ifndef I2C_H_
#define I2C_H_

#include "lpc17xx_i2c.h"
#include <CoOS.h>
#include <Interfaces/Actuators/Actuators.h>
#include <driver/util.h>

extern volatile uint8_t MessageReceived;
extern volatile int totalMessages;

OS_EventID i2c_sem;

void I2C_Initialize(void);
#endif
