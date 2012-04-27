#ifndef ULTRASONICSENSOR_H_
#define ULTRASONICSENSOR_H_

#include <lpc_types.h>
#include "lpc_types.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_timer.h"

#include <CoOS.h>

#include "lpc_exti.h"
#include "cox_exti.h"

#include <Interfaces/Actuators/Actuators.h>

//p8
#define ULTRASONICINPUTPIN COX_PIN(0, 6)

//p20
#define ULTRASONICOUTPUTPIN (1 << 31)

void startMeasureDistanceToGround(void);
Bool initUltrasonicSensor(void);
void TIMER0_IRQHandler (void);
void EINT0_IRQHandler(void);
int EXTINT_eint0 (void);
void exti_handler (COX_PIN_Dev pin);
float getDistance(void);

#endif
