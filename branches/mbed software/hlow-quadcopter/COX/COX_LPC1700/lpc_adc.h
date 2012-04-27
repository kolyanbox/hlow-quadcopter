/***************************************************************************//**
 * @file      : lpc_adc.h
 * @brief     : LPC17xx ADC Peripheral Interface
 * @version   : V1.0
 * @date      : 25. May. 2010
 * @author    : CooCox
*******************************************************************************/
#ifndef __LPC_ADC_H
#define __LPC_ADC_H
#include "cox_adc.h"

#define LPC_ADC_CHANNEL0     COX_PIN(0, 23)        /* AD0 : P0.23 */
#define LPC_ADC_CHANNEL1     COX_PIN(0, 24)        /* AD1 : P0.24 */
#define LPC_ADC_CHANNEL2     COX_PIN(0, 25)        /* AD2 : P0.25 */ 
#define LPC_ADC_CHANNEL3     COX_PIN(0, 26)        /* AD3 : P0.26 */
#define LPC_ADC_CHANNEL4     COX_PIN(1, 30)        /* AD4 : P1.30 */
#define LPC_ADC_CHANNEL5     COX_PIN(1, 31)        /* AD5 : P1.31 */ 
#define LPC_ADC_CHANNEL6     COX_PIN(0,  3)        /* AD6 : P0.3  */
#define LPC_ADC_CHANNEL7     COX_PIN(0,  2)        /* AD7 : P0.2  */

/** Define LPC17xx ADC Peripheral Interface
*******************************************************************************/
extern COX_ADC_PI pi_adc;

#endif
