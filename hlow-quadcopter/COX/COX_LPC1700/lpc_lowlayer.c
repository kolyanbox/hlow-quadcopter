/***************************************************************************//**
 * @file      : lpc_lowlayer.c
 * @brief     : LPC17xx Low Layer APIs
 * @version   : V1.0
 * @date      : 25. Jul. 2010
 * @author    : CooCox
*******************************************************************************/
#include "lpc_lowlayer.h"

extern uint32_t SystemCoreClock;

/***************************************************************************//**
 * @brief     Configure power supply for each peripheral according to NewState
 * @param[in] pclk : Type of peripheral used to enable power,
 *                   should be one of the following:
 *                    - LPC_CLK_PCTIM0  : Timer 0
 *                    - LPC_CLK_PCTIM1  : Timer 1
 *                    - LPC_CLK_PCUART0 : UART 0
 *                    - LPC_CLK_PCUART1 : UART 1
 *                    - LPC_CLK_PCPWM1  : PWM 1
 *                    - LPC_CLK_PCI2C0  : I2C 0
 *                    - LPC_CLK_PCSPI   : SPI
 *                    - LPC_CLK_PCRTC   : RTC
 *                    - LPC_CLK_PCSSP1  : SSP 1
 *                    - LPC_CLK_PCAD    : ADC
 *                    - LPC_CLK_PCAN1   : CAN 1
 *                    - LPC_CLK_PCAN2   : CAN 2
 *                    - LPC_CLK_PCGPIO  : GPIO
 *                    - LPC_CLK_PCRIT   : RIT
 *                    - LPC_CLK_PCMC    : MC
 *                    - LPC_CLK_PCQEI   : QEI
 *                    - LPC_CLK_PCI2C1  : I2C 1
 *                    - LPC_CLK_PCSSP0  : SSP 0
 *                    - LPC_CLK_PCTIM2  : Timer 2
 *                    - LPC_CLK_PCTIM3  : Timer 3
 *                    - LPC_CLK_PCUART2 : UART 2
 *                    - LPC_CLK_PCUART3 : UART 3
 *                    - LPC_CLK_PCI2C2  : I2C 2
 *                    - LPC_CLK_PCI2S   : I2S
 *                    - LPC_CLK_PCGPDMA : GPDMA
 *                    - LPC_CLK_PCENET  : Ethernet
 *                    - LPC_CLK_PCUSB   : USB
 * @param[in] cmd : New state of Peripheral Power, should be:
 *                - 1 : Enable power for this peripheral
 *                - 0 : Disable power for this peripheral
 *
 * @return none
*******************************************************************************/
void LPC_PCLK_Cmd (uint32_t pclk, uint8_t cmd)
{
  if (cmd == 0) {		
    SC->PCONP &= ~pclk;
  }
  else {
    SC->PCONP |= pclk;
  }
}

/***************************************************************************//**
 * @brief     Get power supply state of each peripheral
 * @param[in]	pclk : Type of peripheral used to enable power,
 *                   should be one of the following:
 *                    - LPC_CLK_PCTIM0  : Timer 0
 *                    - LPC_CLK_PCTIM1  : Timer 1
 *                    - LPC_CLK_PCUART0 : UART 0
 *                    - LPC_CLK_PCUART1 : UART 1
 *                    - LPC_CLK_PCPWM1  : PWM 1
 *                    - LPC_CLK_PCI2C0  : I2C 0
 *                    - LPC_CLK_PCSPI   : SPI
 *                    - LPC_CLK_PCRTC   : RTC
 *                    - LPC_CLK_PCSSP1  : SSP 1
 *                    - LPC_CLK_PCAD    : ADC
 *                    - LPC_CLK_PCAN1   : CAN 1
 *                    - LPC_CLK_PCAN2   : CAN 2
 *                    - LPC_CLK_PCGPIO  : GPIO
 *                    - LPC_CLK_PCRIT   : RIT
 *                    - LPC_CLK_PCMC    : MC
 *                    - LPC_CLK_PCQEI   : QEI
 *                    - LPC_CLK_PCI2C1  : I2C 1
 *                    - LPC_CLK_PCSSP0  : SSP 0
 *                    - LPC_CLK_PCTIM2  : Timer 2
 *                    - LPC_CLK_PCTIM3  : Timer 3
 *                    - LPC_CLK_PCUART2 : UART 2
 *                    - LPC_CLK_PCUART3 : UART 3
 *                    - LPC_CLK_PCI2C2  : I2C 2
 *                    - LPC_CLK_PCI2S   : I2S
 *                    - LPC_CLK_PCGPDMA : GPDMA
 *                    - LPC_CLK_PCENET  : Ethernet
 *                    - LPC_CLK_PCUSB   : USB 
 * @return    state of Peripheral Power, should be :
 *              - 1 : Enable power for this peripheral
 *              - 0 : Disable power for this peripheral
*******************************************************************************/
uint8_t LPC_PCLK_Status(uint32_t pclk)
{
  if((SC->PCONP & pclk) == pclk) return 1;
  else                           return 0;
}

/***************************************************************************//**
 * @brief     Setup the pin selection function
 * @param[in] port : PORT number
 * @param[in] pin  : Pin number
 * @param[in] func : Function number
 * @return    None
*******************************************************************************/
uint8_t LPC_Pin_Func ( uint8_t port, uint8_t pin, uint8_t func)
{
  uint8_t pre_func;
  uint32_t pinnum_t = pin;
  uint32_t pinselreg_idx = 2 * port;
  uint32_t *pPinCon = (uint32_t *)&PINCON->PINSEL0;

  if (pinnum_t >= 16) {
    pinnum_t -= 16;
    pinselreg_idx++;
  }
  pre_func = ((*(uint32_t *)(pPinCon + pinselreg_idx)) >> (pinnum_t * 2)) & 0x3;
  *(uint32_t *)(pPinCon + pinselreg_idx) &= ~(0x03UL << (pinnum_t * 2));
  *(uint32_t *)(pPinCon + pinselreg_idx) |= ((uint32_t)func) << (pinnum_t * 2);

  return pre_func;
}

/***************************************************************************//**
 * @brief     Setup resistor mode for each pin
 * @param[in] port : PORT number
 * @param[in] pin  : Pin number
 * @param[in] mode : Mode number
 * @return    None
*******************************************************************************/
uint8_t LPC_Pin_Mode ( uint8_t port, uint8_t pin, uint8_t mode)
{
  uint8_t  pre_mode;
  uint32_t pinnum_t = pin;
  uint32_t pinmodereg_idx = 2 * port;
  uint32_t *pPinCon = (uint32_t *)&PINCON->PINMODE0;

  if (pinnum_t >= 16) {
    pinnum_t -= 16;
    pinmodereg_idx++ ;
  }
  pre_mode = ((*(uint32_t *)(pPinCon + pinmodereg_idx)) >> (pinnum_t * 2)) & 0x3;
  *(uint32_t *)(pPinCon + pinmodereg_idx) &= ~(0x03UL << (pinnum_t * 2));
  *(uint32_t *)(pPinCon + pinmodereg_idx) |= ((uint32_t)mode) << (pinnum_t * 2);

  return pre_mode;
}

/***************************************************************************//**
 * @brief     Setup Open drain mode for each pin
 * @param[in] port : PORT number
 * @param[in] pin  : Pin number
 * @param[in] mode : Open drain mode number
 * @return    None
*******************************************************************************/
uint8_t LPC_Pin_OD ( uint8_t port, uint8_t pin, uint8_t mode)
{
  uint8_t pre_od;
  uint32_t *pPinCon = (uint32_t *)&PINCON->PINMODE_OD0;

  pre_od = ((*(uint32_t *)(pPinCon + port)) >> pin) & 1;

  if (mode == PIN_CFG_OD_OPENDRAIN){
    *(uint32_t *)(pPinCon + port) |= (0x01UL << pin);
  } else {
    *(uint32_t *)(pPinCon + port) &= ~(0x01UL << pin);
  }

  return pre_od;
}

/***************************************************************************//**
 * @brief     Get current value of each Peripheral Clock Selection
 * @param[in] p : Peripheral Clock Selection of each type,
 *                should be one of the following:
 *                  - LPC_PCLK_WDT    : WDT
 *                  - LPC_PCLK_TIMER0 : Timer 0
 *                  - LPC_PCLK_TIMER1 : Timer 1
 *                  - LPC_PCLK_UART0  : UART 0
 *                  - LPC_PCLK_UART1  : UART 1
 *                  - LPC_PCLK_PWM1   : PWM 1
 *                  - LPC_PCLK_I2C0   : I2C 0
 *                  - LPC_PCLK_SPI    : SPI
 *                  - LPC_PCLK_SSP1   : SSP 1
 *                  - LPC_PCLK_DAC    : DAC
 *                  - LPC_PCLK_ADC    : ADC
 *                  - LPC_PCLK_CAN1   : CAN 1
 *                  - LPC_PCLK_CAN2   : CAN 2
 *                  - LPC_PCLK_ACF    : ACF
 *                  - LPC_PCLK_QEI    : QEI
 *                  - LPC_PCLK_PCB    : PCB
 *                  - LPC_PCLK_I2C1   : I2C 1
 *                  - LPC_PCLK_SSP0   : SSP 0
 *                  - LPC_PCLK_TIMER2 : Timer 2
 *                  - LPC_PCLK_TIMER3 : Timer 3
 *                  - LPC_PCLK_UART2  : UART 2
 *                  - LPC_PCLK_UART3  : UART 3
 *                  - LPC_PCLK_I2C2   : I2C 2
 *                  - LPC_PCLK_I2S    : I2S
 *                  - LPC_PCLK_RIT    : RIT
 *                  - LPC_PCLK_SYSCON : SYSCON
 *                  - LPC_PCLK_MC     : MC
 * @return  Value of Selected Peripheral Clock Selection
 ******************************************************************************/
uint32_t LPC_PCLK_Div (uint32_t p)
{
  uint32_t pos, div;

  if (p < 32) {
    pos = p;
    div = SC->PCLKSEL0;
  } else {
    pos = p - 32;
    div = SC->PCLKSEL1;
  }

  div = (div >> pos) & 0x3;

  switch (div) {
  case 0: div = 4;  break;
  case 1: div = 1;  break;
  case 2: div = 2;  break;
  case 3: div = 8;  break;
  }

  return div;
}

/***************************************************************************//**
 * @brief     Set value of each Peripheral Clock Selection
 * @param[in] p : Peripheral Clock Selection of each type,
 *                should be one of the following:
 *                  - LPC_PCLK_WDT    : WDT
 *                  - LPC_PCLK_TIMER0 : Timer 0
 *                  - LPC_PCLK_TIMER1 : Timer 1
 *                  - LPC_PCLK_UART0  : UART 0
 *                  - LPC_PCLK_UART1  : UART 1
 *                  - LPC_PCLK_PWM1   : PWM 1
 *                  - LPC_PCLK_I2C0   : I2C 0
 *                  - LPC_PCLK_SPI    : SPI
 *                  - LPC_PCLK_SSP1   : SSP 1
 *                  - LPC_PCLK_DAC    : DAC
 *                  - LPC_PCLK_ADC    : ADC
 *                  - LPC_PCLK_CAN1   : CAN 1
 *                  - LPC_PCLK_CAN2   : CAN 2
 *                  - LPC_PCLK_ACF    : ACF
 *                  - LPC_PCLK_QEI    : QEI
 *                  - LPC_PCLK_PCB    : PCB
 *                  - LPC_PCLK_I2C1   : I2C 1
 *                  - LPC_PCLK_SSP0   : SSP 0
 *                  - LPC_PCLK_TIMER2 : Timer 2
 *                  - LPC_PCLK_TIMER3 : Timer 3
 *                  - LPC_PCLK_UART2  : UART 2
 *                  - LPC_PCLK_UART3  : UART 3
 *                  - LPC_PCLK_I2C2   : I2C 2
 *                  - LPC_PCLK_I2S    : I2S
 *                  - LPC_PCLK_RIT    : RIT
 *                  - LPC_PCLK_SYSCON : SYSCON
 *                  - LPC_PCLK_MC     : MC
 * @param[in] DivVal : Value of divider, should be:
 *                  - 1 : PCLK_peripheral = CCLK/1
 *                  - 2 : PCLK_peripheral = CCLK/2
 *                  - 4 : PCLK_peripheral = CCLK/4
 *                  - 8 : PCLK_peripheral = CCLK/8 
 * @return none
 ******************************************************************************/
void LPC_PCLK_Set (uint32_t p, uint32_t div)
{
  uint32_t pos;

  switch(div) {
  case 1 :  div = 1; break;
  case 2 :  div = 2; break;
  case 4 :  div = 0; break;
  case 8 :  div = 3; break;
  default : div = 1; break;
  }

  pos = (p < 32) ? (p) : (p - 32);

  if (p < 32) {   /* PCLKSEL0 selected */
    /* Clear two bit at bit position */
    SC->PCLKSEL0 &= ~((uint32_t)3 << pos);

    /* Set two selected bit */
    SC->PCLKSEL0 |= (div << pos);
  }	
  else {          /* PCLKSEL1 selected */
    /* Clear two bit at bit position */
    SC->PCLKSEL1 &= ~((uint32_t)3 << pos);

    /* Set two selected bit */
    SC->PCLKSEL1 |= (div << pos);
  }
}

/***************************************************************************//**
 * @brief     Get current value of each Peripheral Clock
 * @param[in] p : Peripheral Clock Selection of each type,
 *                should be one of the following:
 *                 - LPC_PCLK_WDT    : WDT
 *                 - LPC_PCLK_TIMER0 : Timer 0
 *                 - LPC_PCLK_TIMER1 : Timer 1
 *                 - LPC_PCLK_UART0  : UART 0
 *                 - LPC_PCLK_UART1  : UART 1
 *                 - LPC_PCLK_PWM1   : PWM 1
 *                 - LPC_PCLK_I2C0   : I2C 0
 *                 - LPC_PCLK_SPI    : SPI
 *                 - LPC_PCLK_SSP1   : SSP 1
 *                 - LPC_PCLK_DAC    : DAC
 *                 - LPC_PCLK_ADC    : ADC
 *                 - LPC_PCLK_CAN1   : CAN 1
 *                 - LPC_PCLK_CAN2   : CAN 2
 *                 - LPC_PCLK_ACF    : ACF
 *                 - LPC_PCLK_QEI    : QEI
 *                 - LPC_PCLK_PCB    : PCB
 *                 - LPC_PCLK_I2C1   : I2C 1
 *                 - LPC_PCLK_SSP0   : SSP 0
 *                 - LPC_PCLK_TIMER2 : Timer 2
 *                 - LPC_PCLK_TIMER3 : Timer 3
 *                 - LPC_PCLK_UART2  : UART 2
 *                 - LPC_PCLK_UART3  : UART 3
 *                 - LPC_PCLK_I2C2   : I2C 2
 *                 - LPC_PCLK_I2S    : I2S
 *                 - LPC_PCLK_RIT    : RIT
 *                 - LPC_PCLK_SYSCON : SYSCON
 *                 - LPC_PCLK_MC     : MC
 * @return Value of Selected Peripheral Clock
*******************************************************************************/
uint32_t LPC_PCLK (uint32_t p)
{	
  return (LPC_CCLK() / LPC_PCLK_Div(p));	
}

/***************************************************************************//**
 * @brief     Get current value of Core Clock
 * @param[in] None
 * @return    Value of Core Clock
*******************************************************************************/
uint32_t LPC_CCLK (void)
{	
  return SystemCoreClock;
}
