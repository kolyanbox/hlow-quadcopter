/**************************************************************************//**
 * @file      : cox_rtc.h
 * @brief     : COX RTC Interface
 * @version   : V1.0
 * @date      : 28. Jul. 2010
 * @author    : CooCox
******************************************************************************/
#ifndef __COX_TIMER_H
#define __COX_TIMER_H

#include "cox_types.h"

/** Day of Week Definition */
typedef enum {
  SUNDAY    = 0,
  MONDAY    = 1,
  TUESDAY   = 2,
  WEDNESDAY = 3,      
  THURSDAY  = 4,
  FRIDAY    = 5,
  SATURDAY  = 6
} COX_DAY_OF_WEEK_Def;

/** Calendar Definition */
typedef struct {
  uint32_t sec;     /*!< Seconds      */
  uint32_t min;     /*!< Minutes      */
  uint32_t hour;    /*!< Hours        */
  uint32_t mday;    /*!< Day of Month */
  uint32_t mon;     /*!< Month        */
  uint32_t year;    /*!< Years        */
  uint32_t wday;    /*!< Day of Week  */	
} Time;

/** RTC event call-back function definition */
typedef void (*RTC_Event_Handler)(void);

/** COX Tick Interface Definition
*****************************************************************************/
typedef struct {
  COX_Status (*Init)   (void);
  void       (*Start)  (void);
  void       (*Stop)   (void);
  void       (*Read)   (Time *tm);
  COX_Status (*Write)  (Time *tm);
  COX_Status (*Alarm)  (Time *tm, RTC_Event_Handler handler);
  COX_Status (*Event)  (uint8_t event, RTC_Event_Handler handler, uint32_t arg);
} COX_RTC_PI_Def;

typedef const COX_RTC_PI_Def COX_RTC_PI;

#endif
