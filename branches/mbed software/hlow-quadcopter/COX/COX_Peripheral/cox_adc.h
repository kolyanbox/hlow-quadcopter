/**************************************************************************//**
 * @file      : cox_adc.h
 * @brief     : COX ADC Peripheral Interface
 * @version   : V1.0
 * @date      : 20. Jul. 2010
 * @author    : CooCox
******************************************************************************/
#ifndef __COX_ADC_H
#define __COX_ADC_H

#include "cox_types.h"

/** ADC Configuration Index Definition */
typedef enum {
  COX_ADC_CFG_CLOCK = 0,   /*!< Conversion Clock */
  COX_ADC_CFG_BITS  = 1,   /*!< Conversion Resolution */
} COX_ADC_CFG_INDEX_Def;


/** COX ADC Peripheral Interface Definition
*****************************************************************************/
typedef struct {
  COX_Status (*Init) (uint32_t clock);
  uint32_t   (*Read) (uint16_t ad_in);
  COX_Status (*Cfg)  (uint8_t index, uint32_t arg, uint32_t *pre_arg);
} COX_ADC_PI_Def;

typedef const COX_ADC_PI_Def COX_ADC_PI;

#endif
