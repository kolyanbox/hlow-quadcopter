#ifndef ADC_H_
#define ADC_H_

#include "lpc17xx_adc.h"
#include <Interfaces/Actuators/Actuators.h>
#include <General/util.h>
Bool ADC_init();
uint16_t getADC(int channel);

#endif
