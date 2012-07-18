#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include <lpc_types.h>
#include "lpc17xx_timer.h"
#include "lpc_exti.h"
#include <CoOS.h>
#include <Drivers/GPIO/GPIO.h>

#define ULTRASONICINPUTPIN COX_PIN(0, 6)	//p8
#define ULTRASONICOUTPUTPIN (1 << 31)		//p20

Bool initializeUltrasonicSensor(void);
int EXTINT_eint0 (void);
void startMeasureDistanceToGround(void);
void exti_handler (COX_PIN_Dev pin);
void TIMER0_IRQHandler (void);
int getDistanceToGround(void);

#endif
