/**************************************************************************//**
 * @file      : cox_tick.h
 * @brief     : COX Tick Interface
 * @version   : V1.0
 * @date      : 15. Jul. 2010
 * @author    : CooCox
******************************************************************************/
#ifndef __COX_TIMER_H
#define __COX_TIMER_H

#include "cox_types.h"

/** Tick event mode definition */
typedef enum {
  COX_TICK_EVT_NONE     = 0,   /*!< None     event */
  COX_TICK_EVT_SINGLE   = 1,   /*!< Single   event */
  COX_TICK_EVT_PERIODIC = 2,   /*!< Periodic event */
} COX_TICK_Event_Def;

/** Tick event call-back function definition */
typedef void (*Event_Handler)(void);


/** COX Tick Interface Definition
*****************************************************************************/
typedef struct {
  COX_Status (*Init)     (float interval);
  void       (*Start)    (void);
  void       (*Stop)     (void);
  float      (*Read)     (void);
  void       (*Reset)    (void);
  COX_Status (*Interval) (float interval);
  void       (*Delay)    (float delay_time);
  COX_Status (*Event)    (uint8_t index, Event_Handler handler, float period, uint8_t evt_mode);
} COX_TICK_PI_Def;

typedef const COX_TICK_PI_Def COX_TICK_PI;

#endif
