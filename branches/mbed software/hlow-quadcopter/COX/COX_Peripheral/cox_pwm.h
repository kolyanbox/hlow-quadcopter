/***************************************************************************//**
 * @file      : ssi_pwm.h
 * @brief     : COX PWM Peripheral Interface
 * @version   : V1.0
 * @date      : 23 Jul. 2010
 * @author    : CooCox
*******************************************************************************/
#ifndef __COX_PWM_H
#define __COX_PWM_H

#include "cox_types.h"


/** COX PWM Peripheral Interface Definition
*******************************************************************************/
typedef struct {
  COX_Status (*Init)     (void);
  COX_Status (*Period)   (uint16_t pwmout, float period);
  COX_Status (*Width)    (uint16_t pwmout, float width);
  COX_Status (*usPeriod) (uint16_t pwmout, uint32_t period);
  COX_Status (*usWidth)  (uint16_t pwmout, uint32_t width);
  COX_Status (*Duty)     (uint16_t pwmout, float duty);
  COX_Status (*Start)    (uint16_t pwmout);
  COX_Status (*Stop)     (uint16_t pwmout);
} COX_PWM_PI_Def;

typedef const COX_PWM_PI_Def COX_PWM_PI;

#endif
