/***************************************************************************//**
 * @file      : lpc_exti.h
 * @brief     : LPC17xx PWM Interface
 * @version   : V1.0
 * @date      : 14. Jul. 2010
 * @author    : CooCox 
*******************************************************************************/
#include "cox_pwm.h"

#ifndef __LPC_PWM_H
#define __LPC_PWM_H

#define LPC_PWM_CHANNEL1_0       COX_PIN(1, 18)
#define LPC_PWM_CHANNEL1_1       COX_PIN(2,  0)

#define LPC_PWM_CHANNEL2_0       COX_PIN(1, 20)
#define LPC_PWM_CHANNEL2_1       COX_PIN(2,  1)
#define LPC_PWM_CHANNEL2_2       COX_PIN(3, 25)

#define LPC_PWM_CHANNEL3_0       COX_PIN(1, 21)
#define LPC_PWM_CHANNEL3_1       COX_PIN(2,  2)
#define LPC_PWM_CHANNEL3_2       COX_PIN(3, 26)

#define LPC_PWM_CHANNEL4_0       COX_PIN(1, 23)
#define LPC_PWM_CHANNEL4_1       COX_PIN(2,  3)

#define LPC_PWM_CHANNEL5_0       COX_PIN(1, 24)
#define LPC_PWM_CHANNEL5_1       COX_PIN(2,  4)

#define LPC_PWM_CHANNEL6_0       COX_PIN(1, 26)
#define LPC_PWM_CHANNEL6_1       COX_PIN(2,  5)

/***************************************************************************//**
 * Define PWM Peripheral Interface
 * Max Period : 0xFFFFFFFF / Core Clock
 * Exp :
 *       Core Clock     MAX Period (S)
 *       72000000        59.65232
 *       32000000       134.21772
*******************************************************************************/
extern COX_PWM_PI pi_pwm;

#endif
