/***************************************************************************//**
 * @file      : lpc_exti.h
 * @brief     : LPC17xx EXTI Interface
 * @version   : V1.0
 * @date      : 12. Jul. 2010
 * @author    : CooCox 
*******************************************************************************/
#ifndef __LPC_EXTI_H
#define __LPC_EXTI_H

#include "cox_exti.h"

/**
 *       Define LPC17xx CoX EXTI Interface based on GPIO   \n
 *                                                         \n
 *Valid Pin :                                              \n
 *  PORT0: P0.0 - P0.11, P0.15-P0.30                       \n
 *  PORT2: P2.0 - P2.13                                    \n
 *                                                         \n
 *Valid Event :                                            \n
 *  -COX_EXTI_NONE                : None interrupt         \n
 *  -COX_EXTI_EDGE_RISING         : Rising edge interrupt  \n
 *  -COX_EXTI_EDGE_FALLING        : Falling edge interrupt \n
 *  -COX_EXTI_EDGE_RISING_FALLING : Both edge interrupt (Rising and Falling) 
*******************************************************************************/
extern COX_EXTI_PI pi_exti;

#endif
