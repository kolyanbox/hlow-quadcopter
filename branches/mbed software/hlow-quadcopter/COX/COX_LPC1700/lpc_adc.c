/***************************************************************************//**
 * @file      : lpc_adc.c
 * @brief     : LPC17xx ADC Peripheral Interface based on SSPx
 * @version   : V1.0
 * @date      : 17. Jul. 2010
 * @author    : CooCox
*******************************************************************************/
#include "lpc_lowlayer.h"              
#include "LPC_ADC.H"

#define ADC_PC_CTRL    ((uint32_t)1<<12)     /* ADC power/clock control */
#define ADC_CR_PDN     ((uint32_t)1<<21)

/***************************************************************************//**
 * @brief     Setup conversion clock 
 * @param[in] SSPx : peripheral definition, should be SSP0 or SSP1.
 * @param[in] target_clock : Clock frequency in Hz
 * @return    Previous clock frequency in Hz
*******************************************************************************/
static uint32_t LPC_ADC_SetRate (uint32_t target_clock)
{
  uint32_t prescale, clk_bak, clk_div;

  if(target_clock == 0) return COX_ERROR;
     
  /* Get current clock */
  clk_bak = LPC_PCLK(LPC_PCLK_ADC) / (((ADC->ADCR >> 8) & 0xFF) + 1);    
  prescale = (LPC_CCLK()+(target_clock/2)) / target_clock;
  
  clk_div = 1;      
  if(prescale > 0x100) {
    clk_div = 2;
    prescale = prescale / 2;
    if(prescale > 0x100) {
      clk_div = 4;
      prescale = prescale / 2;      
      if(prescale > 0x100) {
        clk_div = 8;
        prescale = prescale / 2;
      }
    } 
  }  

  /* ADC Peripheral Clock Selection */
  LPC_PCLK_Set(LPC_PCLK_ADC, clk_div);

  if(prescale > 0x100) prescale = 0x100;
  if(prescale != 0) prescale -= 1;
  
  ADC->ADCR &= ~((uint32_t)0xFF << 8);  
  ADC->ADCR |=  ((uint32_t)prescale << 8);

  return clk_bak;
}

/***************************************************************************//**
 * @brief     Initialize the ADC peripheral
 * @param[in] clock : conversion clock 
 * @return    COX_SUCCESS or COX_ERROR
*******************************************************************************/
static COX_Status LPC_ADC_Init(uint32_t clock)
{
  /* Enable ADC power/clock */
  SC->PCONP |= ADC_PC_CTRL; 
  /* Enable ADC */
  ADC->ADCR = ADC_CR_PDN;

  LPC_ADC_SetRate(clock * 65);

  return COX_SUCCESS;
}

/***************************************************************************//**
 * @brief     Start a conversion and get the result
 * @param[in] channel : AD channel, should be :
 *                      -LPC_ADC_CHANNEL0 : AD0, P0.23 
 *                      -LPC_ADC_CHANNEL1 : AD1, P0.24 
 *                      -LPC_ADC_CHANNEL2 : AD2, P0.25
 *                      -LPC_ADC_CHANNEL3 : AD3, P0.26
 *                      -LPC_ADC_CHANNEL4 : AD4, P1.30
 *                      -LPC_ADC_CHANNEL5 : AD5, P1.31
 *                      -LPC_ADC_CHANNEL6 : AD6, P0.3
 *                      -LPC_ADC_CHANNEL7 : AD7, P0.2 
 * @return    Data conversion
*******************************************************************************/
static uint32_t LPC_ADC_Read(uint16_t ad_in)
{
  uint32_t adc_value; 
  uint8_t channel; 
  
  /* Select AD Function, Disable open drain, pull-up, pull-down */
  switch(ad_in) {
  case LPC_ADC_CHANNEL0 :    /* AD0 : P0.23, Function 1 */
    PINCON->PINSEL1  &= ~((uint32_t)3 << 14);
    PINCON->PINSEL1  |=  ((uint32_t)1 << 14);          
    PINCON->PINMODE1 &= ~((uint32_t)3 << 14);
    PINCON->PINMODE1 |=  ((uint32_t)2 << 14);  
    PINCON->PINMODE_OD0 &=  ~((uint32_t)1 << 23);
    channel = 0;  break;

  case LPC_ADC_CHANNEL1 :    /* AD1 : P0.24, Function 1 */
    PINCON->PINSEL1  &= ~((uint32_t)3 << 16);
    PINCON->PINSEL1  |=  ((uint32_t)1 << 16);          
    PINCON->PINMODE1 &= ~((uint32_t)3 << 16);
    PINCON->PINMODE1 |=  ((uint32_t)2 << 16); 
    PINCON->PINMODE_OD0 &=  ~((uint32_t)1 << 24);         
    channel = 1;  break;

  case LPC_ADC_CHANNEL2 :    /* AD2 : P0.25, Function 1 */     
    PINCON->PINSEL1  &= ~((uint32_t)3 << 18);
    PINCON->PINSEL1  |=  ((uint32_t)1 << 18);
    PINCON->PINMODE1 &= ~((uint32_t)3 << 18);
    PINCON->PINMODE1 |=  ((uint32_t)2 << 18);
    PINCON->PINMODE_OD0 &=  ~((uint32_t)1 << 25);
    channel = 2;  break;

  case LPC_ADC_CHANNEL3 :    /* AD3 : P0.26, Function 1 */
    PINCON->PINSEL1  &= ~((uint32_t)3 << 20);
    PINCON->PINSEL1  |=  ((uint32_t)1 << 20); 
    PINCON->PINMODE1 &= ~((uint32_t)3 << 20);
    PINCON->PINMODE1 |=  ((uint32_t)2 << 20); 
    PINCON->PINMODE_OD0 &=  ~((uint32_t)1 << 26);
    channel = 3;  break;

  case LPC_ADC_CHANNEL4 :    /* AD4 : P1.30, Function 3 */
    PINCON->PINSEL3  |=  ((uint32_t)3 << 28);
    PINCON->PINMODE3 &= ~((uint32_t)3 << 28);
    PINCON->PINMODE3 |=  ((uint32_t)2 << 28); 
    PINCON->PINMODE_OD1 &=  ~((uint32_t)1 << 30);
    channel = 4;  break;

  case LPC_ADC_CHANNEL5 :    /* AD5 : P1.31 */ 
    PINCON->PINSEL3  |=  ((uint32_t)3 << 30);
    PINCON->PINMODE3 &= ~((uint32_t)3 << 30);
    PINCON->PINMODE3 |=  ((uint32_t)2 << 30);
    PINCON->PINMODE_OD1 &=  ~((uint32_t)1 << 31);
    channel = 5;  break;
     
  case LPC_ADC_CHANNEL6 :    /* AD6 : P0.3,  Function 2 */
    PINCON->PINSEL0  &= ~((uint32_t)3 << 6);
    PINCON->PINSEL0  |=  ((uint32_t)2 << 6);    
    PINCON->PINMODE0 &= ~((uint32_t)3 << 6);
    PINCON->PINMODE0 |=  ((uint32_t)2 << 6);    
    PINCON->PINMODE_OD0 &=  ~((uint32_t)1 << 3);
    channel = 6;  break;
              
  case LPC_ADC_CHANNEL7 :    /* AD7 : P0.2,  Function 2 */
    PINCON->PINSEL0 &= ~((uint32_t)3 << 4);
    PINCON->PINSEL0 |=  ((uint32_t)2 << 4);    
    PINCON->PINMODE0 &= ~((uint32_t)3 << 4);
    PINCON->PINMODE0 |=  ((uint32_t)2 << 4); 
    PINCON->PINMODE_OD0 &=  ~((uint32_t)1 << 2);   
    channel = 7;  break;
  default : return 0;     
  }  

  /* Read to clear the channel state */
  adc_value = *(uint32_t *) ((&ADC->ADDR0) + channel);

  /* Selects the AD pins to be sampled and converted. */
  ADC->ADCR |= (uint32_t)1 << channel;
  
  /* Start conversion now */
  ADC->ADCR &= ~((uint32_t)7 << 24);
  ADC->ADCR |=  ((uint32_t)1 << 24);
  
  /* Wait conversion complete */
  while(((*(uint32_t *) ((&ADC->ADDR0) + channel)) & (uint32_t)1<<31) == 0);

  /* Get ADC result */
  adc_value = *(uint32_t *) ((&ADC->ADDR0) + channel);
  adc_value = (adc_value>>4) & 0xFFF;

  ADC->ADCR &= ~(((uint32_t)1 << channel) | ((uint32_t)7 << 24));  

  return adc_value;
}

/***************************************************************************//**
 * @brief     Configure the ADC peripheral
 * @param[in] index : Configuration index, should be :
 *                      -COX_ADC_CFG_CLOCK : Conversion clock
 *                      -COX_ADC_CFG_BITS : Conversion Resolution, only 12
 * @param[in] arg : Configuration argument
 * @param[in] pre_arg : Return previous configuration argument
 * @return    COX_SUCCESS or COX_ERROR
*******************************************************************************/
static COX_Status LPC_ADC_Cfg (uint8_t index, uint32_t arg, uint32_t *pre_arg)
{
  uint32_t arg_bak;
  COX_Status result = COX_ERROR;  

  switch(index) {
  case COX_ADC_CFG_CLOCK :   
    arg_bak = LPC_ADC_SetRate(arg * 65) / 65; 
    result = COX_SUCCESS;  break;
        
  case COX_ADC_CFG_BITS :    
    arg_bak = 12;            /* Only support 12-bit */
    if(arg != 12) break;
    result = COX_SUCCESS;  break;

  default : break;     
  }  

  if((result == COX_SUCCESS) && (pre_arg != COX_NULL)) {
    *pre_arg = arg_bak;
  }

  return result;
}

/***************************************************************************//**
 * Define LPC17xx COX ADC Peripheral Interface
*******************************************************************************/
COX_ADC_PI pi_adc = 
{
  LPC_ADC_Init,
  LPC_ADC_Read,
  LPC_ADC_Cfg,
};
