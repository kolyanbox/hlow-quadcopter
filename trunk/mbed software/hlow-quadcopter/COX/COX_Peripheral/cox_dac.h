/**************************************************************************//**
 * @file      : cox_dac.h
 * @brief     : COX DAC Peripheral Interface
 * @version   : V1.0
 * @date      : 20. Jul. 2010
 * @author    : CooCox
******************************************************************************/
#ifndef __COX_DAC_H
#define __COX_DAC_H

#include "cox_types.h"

/** DAC Configuration Index Definition */
typedef enum {
  COX_DAC_CFG_BITS    = 0,   /*!< Conversion Resolution */
  COX_DAC_CFG_CURRENT = 1,   /*!< Maximum current   */  
} COX_DAC_CFG_INDEX_Def;


/** COX DAC Peripheral Interface Definition
*****************************************************************************/
typedef struct {
  COX_Status (*Init)  (void);
  void       (*Write) (uint16_t ad_out, uint32_t value);
  COX_Status (*Cfg)   (uint8_t index, uint32_t arg, uint32_t *pre_arg);
} COX_DAC_PI_Def;

typedef const COX_DAC_PI_Def COX_DAC_PI;

#endif
