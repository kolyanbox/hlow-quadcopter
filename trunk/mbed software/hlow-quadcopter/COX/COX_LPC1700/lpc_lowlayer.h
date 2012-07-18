/***************************************************************************//**
 * @file      : lpc_lowlayer.h
 * @brief     : LPC17xx Low Layer APIs
 * @version   : V1.0
 * @date      : 25. Jul. 2010
 * @author    : CooCox
*******************************************************************************/
#ifndef __LPC_LOWERLAYER_H
#define __LPC_LOWERLAYER_H
#include "lpc_definition.h"

#define PIN_CFG_FUNC_0        (0)	/*!< default function */
#define PIN_CFG_FUNC_1        (1)	/*!< first alternate function */
#define PIN_CFG_FUNC_2        (2)	/*!< second alternate function */
#define PIN_CFG_FUNC_3        (3)	/*!< third or reserved alternate function */

/***************************************************************************//**
 * Macros define for Pin mode
*******************************************************************************/
#define PIN_CFG_MODE_PULLUP   (0)	/*!< Internal pull-up resistor */
#define PIN_CFG_MODE_TRISTATE (2)	/*!< Tri-state */
#define PIN_CFG_MODE_PULLDOWN (3) /*!< Internal pull-down resistor */


/***************************************************************************//**
 * Macros define for Pin mode (normal/open drain)
*******************************************************************************/
#define	PIN_CFG_OD_NORMAL     (0)	/*!< Pin is in the normal mode. */
#define	PIN_CFG_OD_OPENDRAIN  (1)	/*!< Pin is in the open drain mode */

/***************************************************************************//**
* Power Control for Peripherals Definitions
*******************************************************************************/
/** Timer/Counter 0 power/clock control bit */
#define	 LPC_CLK_TIM0    ((uint32_t)(1<<1))
/* Timer/Counter 1 power/clock control bit */
#define	 LPC_CLK_TIM1    ((uint32_t)(1<<2))
/** UART0 power/clock control bit */
#define	 LPC_CLK_UART0   ((uint32_t)(1<<3))
/** UART1 power/clock control bit */
#define	 LPC_CLK_UART1   ((uint32_t)(1<<4))
/** PWM1 power/clock control bit */
#define	 LPC_CLK_PWM1    ((uint32_t)(1<<6))
/** The I2C0 interface power/clock control bit */
#define	 LPC_CLK_I2C0    ((uint32_t)(1<<7))
/** The SPI interface power/clock control bit */
#define	 LPC_CLK_SPI     ((uint32_t)(1<<8))
/** The RTC power/clock control bit */
#define	 LPC_CLK_RTC     ((uint32_t)(1<<9))
/** The SSP1 interface power/clock control bit */
#define	 LPC_CLK_SSP1    ((uint32_t)(1<<10))
/** A/D converter 0 (ADC0) power/clock control bit */
#define	 LPC_CLK_AD      ((uint32_t)(1<<12))
/** CAN Controller 1 power/clock control bit */
#define	 LPC_CLK_AN1     ((uint32_t)(1<<13))
/** CAN Controller 2 power/clock control bit */
#define	 LPC_CLK_AN2     ((uint32_t)(1<<14))
/** GPIO power/clock control bit */
#define	 LPC_CLK_GPIO    ((uint32_t)(1<<15))
/** Repetitive Interrupt Timer power/clock control bit */
#define	 LPC_CLK_RIT     ((uint32_t)(1<<16))
/** Motor Control PWM */
#define  LPC_CLK_MC      ((uint32_t)(1<<17))
/** Quadrature Encoder Interface power/clock control bit */
#define  LPC_CLK_QEI     ((uint32_t)(1<<18))
/** The I2C1 interface power/clock control bit */
#define	 LPC_CLK_I2C1    ((uint32_t)(1<<19))
/** The SSP0 interface power/clock control bit */
#define	 LPC_CLK_SSP0    ((uint32_t)(1<<21))
/** Timer 2 power/clock control bit */
#define	 LPC_CLK_TIM2    ((uint32_t)(1<<22))
/** Timer 3 power/clock control bit */
#define	 LPC_CLK_TIM3    ((uint32_t)(1<<23))
/** UART 2 power/clock control bit */
#define	 LPC_CLK_UART2   ((uint32_t)(1<<24))
/** UART 3 power/clock control bit */
#define	 LPC_CLK_UART3   ((uint32_t)(1<<25))
/** I2C interface 2 power/clock control bit */
#define	 LPC_CLK_I2C2    ((uint32_t)(1<<26))
/** I2S interface power/clock control bit*/
#define	 LPC_CLK_I2S     ((uint32_t)(1<<27))
/** GP DMA function power/clock control bit*/
#define	 LPC_CLK_GPDMA   ((uint32_t)(1<<29))
/** Ethernet block power/clock control bit*/
#define	 LPC_CLK_ENET    ((uint32_t)(1<<30))
/** USB interface power/clock control bit*/
#define	 LPC_CLK_USB     ((uint32_t)(1<<31))


/***************************************************************************//**
 * Peripheral Clock Selection Definitions
*******************************************************************************/
/** Peripheral clock divider bit position for WDT */
#define	LPC_PCLK_WDT        ((uint32_t)(0))
/** Peripheral clock divider bit position for TIMER0 */
#define	LPC_PCLK_TIMER0     ((uint32_t)(2))
/** Peripheral clock divider bit position for TIMER1 */
#define	LPC_PCLK_TIMER1     ((uint32_t)(4))
/** Peripheral clock divider bit position for UART0 */
#define	LPC_PCLK_UART0      ((uint32_t)(6))
/** Peripheral clock divider bit position for UART1 */
#define	LPC_PCLK_UART1      ((uint32_t)(8))
/** Peripheral clock divider bit position for PWM1 */
#define	LPC_PCLK_PWM1       ((uint32_t)(12))
/** Peripheral clock divider bit position for I2C0 */
#define	LPC_PCLK_I2C0       ((uint32_t)(14))
/** Peripheral clock divider bit position for SPI */
#define	LPC_PCLK_SPI        ((uint32_t)(16))
/** Peripheral clock divider bit position for SSP1 */
#define	LPC_PCLK_SSP1       ((uint32_t)(20))
/** Peripheral clock divider bit position for DAC */
#define	LPC_PCLK_DAC        ((uint32_t)(22))
/** Peripheral clock divider bit position for ADC */
#define	LPC_PCLK_ADC        ((uint32_t)(24))
/** Peripheral clock divider bit position for CAN1 */
#define	LPC_PCLK_CAN1       ((uint32_t)(26))
/** Peripheral clock divider bit position for CAN2 */
#define	LPC_PCLK_CAN2       ((uint32_t)(28))
/** Peripheral clock divider bit position for ACF */
#define	LPC_PCLK_ACF        ((uint32_t)(30))
/** Peripheral clock divider bit position for QEI */
#define	LPC_PCLK_QEI        ((uint32_t)(32))
/** Peripheral clock divider bit position for PCB */
#define	LPC_PCLK_PCB        ((uint32_t)(36))
/** Peripheral clock divider bit position for  I2C1 */
#define	LPC_PCLK_I2C1       ((uint32_t)(38))
/** Peripheral clock divider bit position for SSP0 */
#define	LPC_PCLK_SSP0       ((uint32_t)(42))
/** Peripheral clock divider bit position for TIMER2 */
#define	LPC_PCLK_TIMER2     ((uint32_t)(44))
/** Peripheral clock divider bit position for  TIMER3 */
#define	LPC_PCLK_TIMER3     ((uint32_t)(46))
/** Peripheral clock divider bit position for UART2 */
#define	LPC_PCLK_UART2      ((uint32_t)(48))
/** Peripheral clock divider bit position for UART3 */
#define	LPC_PCLK_UART3      ((uint32_t)(50))
/** Peripheral clock divider bit position for I2C2 */
#define	LPC_PCLK_I2C2       ((uint32_t)(52))
/** Peripheral clock divider bit position for I2S */
#define	LPC_PCLK_I2S        ((uint32_t)(54))
/** Peripheral clock divider bit position for RIT */
#define	LPC_PCLK_RIT        ((uint32_t)(58))
/** Peripheral clock divider bit position for SYSCON */
#define	LPC_PCLK_SYSCON     ((uint32_t)(60))
/** Peripheral clock divider bit position for MC */
#define	LPC_PCLK_MC         ((uint32_t)(62))
                                      
/***************************************************************************//**
 * Public Functions
*******************************************************************************/
uint32_t LPC_CCLK (void);
uint32_t LPC_PCLK (uint32_t p);
void     LPC_PCLK_Set (uint32_t p, uint32_t div);
uint32_t LPC_PCLK_Div (uint32_t p);
void     LPC_PCLK_Cmd (uint32_t pclk, uint8_t cmd);
uint8_t  LPC_PCLK_Status (uint32_t pclk);


uint8_t LPC_Pin_Func ( uint8_t port, uint8_t pin, uint8_t func);
uint8_t LPC_Pin_Mode ( uint8_t port, uint8_t pin, uint8_t mode);
uint8_t LPC_Pin_OD   ( uint8_t port, uint8_t pin, uint8_t mode);


#endif
