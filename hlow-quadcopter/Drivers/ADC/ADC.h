#ifndef ADC_H_
#define ADC_H_

#include "lpc17xx_adc.h"

Bool ADC_init();

/**
 * Returns the value of ADC 0, 1 or 2
 */
uint16_t getADC(int channel);

/**
 * returns the value of ADC 3
 */
uint16_t getADC3();

#endif
