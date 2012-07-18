/******************************************************************************
 * @file      : cox_exti.h
 * @brief     : COX EXTI Interface
 * @version   : V1.0
 * @date      : 25. May. 2010
 * @author    : CooCox
******************************************************************************/
#ifndef __COX_EXTI_H
#define __COX_EXTI_H

#include "cox_types.h"


/** EXTI Event Mode Definition */
typedef enum {
  COX_EXTI_NONE                = 0,  /*!< None interrupt */
  COX_EXTI_EDGE_RISING         = 1,  /*!< Rising edge interrupt */
  COX_EXTI_EDGE_FALLING        = 2,  /*!< Falling edge interrupt */
  COX_EXTI_EDGE_RISING_FALLING = 3,  /*!< Both edge interrupt (Rising and Falling) */
  COX_EXTI_LEVEL_HIGH          = 4,  /*!< Hign Level interrupt */
  COX_EXTI_LEVEL_LOW           = 5,  /*!< Low Level interrupt */  
} EXTI_EVENT_Def;

/**
 * EXTI Interrupt Service Routine Call-Back Function Definition
 * When EXTI interrupt ocurrs, the EXTI interrupt handler will be called.
*****************************************************************************/
typedef void (*EXTI_Handler)(uint16_t pin);

/** COX EXTI Interface Definition
*****************************************************************************/
typedef struct {
  void       (*Handler) (EXTI_Handler  handler);      
  COX_Status (*Init)    (uint16_t pin);
  uint8_t    (*Event)   (uint16_t pin, uint8_t event);  
} COX_EXTI_PI_Def;

typedef const COX_EXTI_PI_Def COX_EXTI_PI;

#endif
