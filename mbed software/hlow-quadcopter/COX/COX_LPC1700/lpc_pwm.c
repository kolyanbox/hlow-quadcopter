/***************************************************************************//**
 * @file      : lpc_pwm.c
 * @brief     : LPC17xx PWM Peripheral Interface
 * @version   : V1.0
 * @date      : 14. Jul. 2010
 * @author    : CooCox 
*******************************************************************************/
#include "lpc_pwm.h"
#include "lpc_lowlayer.h"

/** MCR register mask */
#define PWM_MCR_BITMASK             ((uint32_t)(0x001FFFFF))
/** generate a PWM interrupt when a MATCHn occurs */
#define PWM_MCR_INT_ON_MATCH(n)     ((uint32_t)(1<<(((n&0x7)<<1)+(n&0x07))))
/** reset the PWM when a MATCHn occurs */
#define PWM_MCR_RESET_ON_MATCH(n)   ((uint32_t)(1<<(((n&0x7)<<1)+(n&0x07)+1)))
/** stop the PWM when a MATCHn occurs */
#define PWM_MCR_STOP_ON_MATCH(n)    ((uint32_t)(1<<(((n&0x7)<<1)+(n&0x07)+2)))

#define PWM_IR_BITMASK              ((uint32_t)(0x0000073F))

/*******************************************************************************
* PCR register definitions
*******************************************************************************/
/** PCR register mask */
#define PWM_PCR_BITMASK             (uint32_t)0x00007E7C
/** PWM output n is a single edge controlled output */
#define PWM_PCR_PWMSELn(n)          ((uint32_t)(((n&0x7)<2) ? 0 : (1<<n)))
/** enable PWM output n */
#define PWM_PCR_PWMENAn(n)          ((uint32_t)(((n&0x7)<1) ? 0 : (1<<(n+8))))

/*******************************************************************************
* LER register definitions
*******************************************************************************/
/** LER register mask*/
#define PWM_LER_BITMASK              ((uint32_t)(0x0000007F))
/** PWM MATCHn register update control */
#define PWM_LER_EN_MATCHn_LATCH(n)   ((uint32_t)((n<7) ? (1<<n) : 0))

#define LPC_PWM_CHANNEL_INVALID       0xFF

/***************************************************************************//**
 * @brief     Setup PWM Output Pin
 * @param[in] pwmout : PWM output Pin, should be one of the following
 *                     - LPC_PWM_CHANNEL1_0 : PWM Channel 1 Output, P1.18
 *                     - LPC_PWM_CHANNEL1_1 : PWM Channel 1 Output, P2.0
 *                     - LPC_PWM_CHANNEL2_0 : PWM Channel 2 Output, P1.20
 *                     - LPC_PWM_CHANNEL2_1 : PWM Channel 2 Output, P2.1
 *                     - LPC_PWM_CHANNEL2_2 : PWM Channel 2 Output, P3.25
 *                     - LPC_PWM_CHANNEL3_0 : PWM Channel 3 Output, P1.21
 *                     - LPC_PWM_CHANNEL3_1 : PWM Channel 3 Output, P2.2
 *                     - LPC_PWM_CHANNEL3_2 : PWM Channel 3 Output, P3.26
 *                     - LPC_PWM_CHANNEL4_0 : PWM Channel 4 Output, P1.23
 *                     - LPC_PWM_CHANNEL4_1 : PWM Channel 4 Output, P2.3
 *                     - LPC_PWM_CHANNEL5_0 : PWM Channel 5 Output, P1.24
 *                     - LPC_PWM_CHANNEL5_1 : PWM Channel 5 Output, P2.4
 *                     - LPC_PWM_CHANNEL6_0 : PWM Channel 6 Output, P1.26
 *                     - LPC_PWM_CHANNEL6_1 : PWM Channel 6 Output, P2.5
 * @return 
 *         - COX_SUCCESS : success
 *         - COX_ERROR : The PWM Output Pin was not supported
*******************************************************************************/
static COX_Status LPC_PWM_PinCfg (uint16_t pwmout)
{
  uint8_t func = 0;

  switch(pwmout) {
  case LPC_PWM_CHANNEL1_0 : func = 2; break;   /* P1.18 */
  case LPC_PWM_CHANNEL1_1 : func = 1; break;   /* P2.0  */  
  case LPC_PWM_CHANNEL2_0 : func = 2; break;   /* P1.20 */
  case LPC_PWM_CHANNEL2_1 : func = 1; break;   /* P2.1  */
  case LPC_PWM_CHANNEL2_2 : func = 3; break;   /* P3.25 */  
  case LPC_PWM_CHANNEL3_0 : func = 2; break;   /* P1.21 */
  case LPC_PWM_CHANNEL3_1 : func = 1; break;   /* P2.2  */
  case LPC_PWM_CHANNEL3_2 : func = 3; break;   /* P3.26 */  
  case LPC_PWM_CHANNEL4_0 : func = 2; break;   /* P1.23 */
  case LPC_PWM_CHANNEL4_1 : func = 1; break;   /* P2.3  */  
  case LPC_PWM_CHANNEL5_0 : func = 2; break;   /* P1.24 */
  case LPC_PWM_CHANNEL5_1 : func = 1; break;   /* P2.4  */  
  case LPC_PWM_CHANNEL6_0 : func = 2; break;   /* P1.26 */
  case LPC_PWM_CHANNEL6_1 : func = 1; break;   /* P2.5  */
  default : return COX_ERROR;
  }  

  LPC_Pin_Func((pwmout>>8) & 0xFF, pwmout & 0xFF, func);
  LPC_Pin_Mode((pwmout>>8) & 0xFF, pwmout & 0xFF, PIN_CFG_MODE_PULLUP);
  LPC_Pin_OD  ((pwmout>>8) & 0xFF, pwmout & 0xFF, PIN_CFG_OD_NORMAL);

  return COX_SUCCESS;
}

/***************************************************************************//**
 * @brief     Get PWM channel number via PWM Output Pin
 * @param[in] pwmout : PWM output Pin, should be one of the following
 *                     - LPC_PWM_CHANNEL1_0 : PWM Channel 1 Output, P1.18
 *                     - LPC_PWM_CHANNEL1_1 : PWM Channel 1 Output, P2.0
 *                     - LPC_PWM_CHANNEL2_0 : PWM Channel 2 Output, P1.20
 *                     - LPC_PWM_CHANNEL2_1 : PWM Channel 2 Output, P2.1
 *                     - LPC_PWM_CHANNEL2_2 : PWM Channel 2 Output, P3.25
 *                     - LPC_PWM_CHANNEL3_0 : PWM Channel 3 Output, P1.21
 *                     - LPC_PWM_CHANNEL3_1 : PWM Channel 3 Output, P2.2
 *                     - LPC_PWM_CHANNEL3_2 : PWM Channel 3 Output, P3.26
 *                     - LPC_PWM_CHANNEL4_0 : PWM Channel 4 Output, P1.23
 *                     - LPC_PWM_CHANNEL4_1 : PWM Channel 4 Output, P2.3
 *                     - LPC_PWM_CHANNEL5_0 : PWM Channel 5 Output, P1.24
 *                     - LPC_PWM_CHANNEL5_1 : PWM Channel 5 Output, P2.4
 *                     - LPC_PWM_CHANNEL6_0 : PWM Channel 6 Output, P1.26
 *                     - LPC_PWM_CHANNEL6_1 : PWM Channel 6 Output, P2.5
 * @return    PWM channel number
*******************************************************************************/
uint8_t LPC_PWM_Channel (uint16_t pwmout)
{  
  uint8_t chn;

  switch(pwmout) {
  case LPC_PWM_CHANNEL1_0 : chn = 0x1; break;   /* P1.18 */
  case LPC_PWM_CHANNEL1_1 : chn = 0x1; break;   /* P2.0  */  
  case LPC_PWM_CHANNEL2_0 : chn = 0x2; break;   /* P1.20 */
  case LPC_PWM_CHANNEL2_1 : chn = 0x2; break;   /* P2.1  */
  case LPC_PWM_CHANNEL2_2 : chn = 0x2; break;   /* P3.25 */  
  case LPC_PWM_CHANNEL3_0 : chn = 0x3; break;   /* P1.21 */
  case LPC_PWM_CHANNEL3_1 : chn = 0x3; break;   /* P2.2  */
  case LPC_PWM_CHANNEL3_2 : chn = 0x3; break;   /* P3.26 */  
  case LPC_PWM_CHANNEL4_0 : chn = 0x4; break;   /* P1.23 */
  case LPC_PWM_CHANNEL4_1 : chn = 0x4; break;   /* P2.3  */  
  case LPC_PWM_CHANNEL5_0 : chn = 0x5; break;   /* P1.24 */
  case LPC_PWM_CHANNEL5_1 : chn = 0x5; break;   /* P2.4  */  
  case LPC_PWM_CHANNEL6_0 : chn = 0x6; break;   /* P1.26 */
  case LPC_PWM_CHANNEL6_1 : chn = 0x6; break;   /* P2.5  */
  default : return 0xFF;
  }

  return chn;     
}

/***************************************************************************//**
 * @brief     Initializes PWM Peripheral
 * @param[in] None
 * @return    Always SUCCESS
*******************************************************************************/
COX_Status LPC_PWM_Init (void)
{  
  /* Enable PWM peripheral clock */
  LPC_PCLK_Cmd(LPC_CLK_PWM1, 1);
  LPC_PCLK_Set(LPC_PCLK_PWM1, 1);

  /* Clear all Registers */
  PWM1->IR   = 0xFF & PWM_IR_BITMASK;
  PWM1->TCR  = 0x00;
  PWM1->CTCR = 0x00;
  PWM1->MCR  = 0x00;
  PWM1->CCR  = 0x00;
  PWM1->PCR  = 0x00;
  PWM1->LER  = 0x00;
  PWM1->PR   = 0x00;    /* Clock Prescale : 1 */

  PWM1->TCR = (1<<3);   /* PWM Enable */

  /* Enable Reset on MR0 */
  PWM1->MCR = PWM_MCR_RESET_ON_MATCH(0);
	  
  return COX_SUCCESS;     
}

/***************************************************************************//**
 * @brief     Setup PWM Output Period in second
 * @param[in] pwmout : PWM output Pin, should be one of the following
 *                     - LPC_PWM_CHANNEL1_0 : PWM Channel 1 Output, P1.18
 *                     - LPC_PWM_CHANNEL1_1 : PWM Channel 1 Output, P2.0
 *                     - LPC_PWM_CHANNEL2_0 : PWM Channel 2 Output, P1.20
 *                     - LPC_PWM_CHANNEL2_1 : PWM Channel 2 Output, P2.1
 *                     - LPC_PWM_CHANNEL2_2 : PWM Channel 2 Output, P3.25
 *                     - LPC_PWM_CHANNEL3_0 : PWM Channel 3 Output, P1.21
 *                     - LPC_PWM_CHANNEL3_1 : PWM Channel 3 Output, P2.2
 *                     - LPC_PWM_CHANNEL3_2 : PWM Channel 3 Output, P3.26
 *                     - LPC_PWM_CHANNEL4_0 : PWM Channel 4 Output, P1.23
 *                     - LPC_PWM_CHANNEL4_1 : PWM Channel 4 Output, P2.3
 *                     - LPC_PWM_CHANNEL5_0 : PWM Channel 5 Output, P1.24
 *                     - LPC_PWM_CHANNEL5_1 : PWM Channel 5 Output, P2.4
 *                     - LPC_PWM_CHANNEL6_0 : PWM Channel 6 Output, P1.26
 *                     - LPC_PWM_CHANNEL6_1 : PWM Channel 6 Output, P2.5
 * @param[in] period : PWM Output Period in second
 * @return    - COX_SUCCESS : Success
 *            - COX_ERROR   : The PWM Output or With value were not supported
 *
 * @NOTE : The LPC17xx PWM hardware is limited to share the period value between 
 *         all outputs. Therefore, if you change the period of one output, you 
 *         change them all. The pulsewidth can be set independently for each 
 *         output.
*******************************************************************************/
COX_Status LPC_PWM_Period (uint16_t pwmout, float period)
{
  uint32_t uClk;                            
  uint64_t value;

  /* Get PWM Peripheral Clock */ 
  uClk = LPC_PCLK(LPC_PCLK_PWM1);

  value = uClk * period;
  if((value == 0) || (value > 0xFFFFFFFF)) return COX_ERROR;
  
  PWM1->MR0 = value & 0xFFFFFFFF;

  /* Write Latch register */
  PWM1->LER |= PWM_LER_EN_MATCHn_LATCH(0);
 
  return COX_SUCCESS;
}

/***************************************************************************//**
 * @brief     Set the PWM Pulsewidth in second
 * @param[in] pwmout : PWM output Pin, should be one of the following
 *                     - LPC_PWM_CHANNEL1_0 : PWM Channel 1 Output, P1.18
 *                     - LPC_PWM_CHANNEL1_1 : PWM Channel 1 Output, P2.0
 *                     - LPC_PWM_CHANNEL2_0 : PWM Channel 2 Output, P1.20
 *                     - LPC_PWM_CHANNEL2_1 : PWM Channel 2 Output, P2.1
 *                     - LPC_PWM_CHANNEL2_2 : PWM Channel 2 Output, P3.25
 *                     - LPC_PWM_CHANNEL3_0 : PWM Channel 3 Output, P1.21
 *                     - LPC_PWM_CHANNEL3_1 : PWM Channel 3 Output, P2.2
 *                     - LPC_PWM_CHANNEL3_2 : PWM Channel 3 Output, P3.26
 *                     - LPC_PWM_CHANNEL4_0 : PWM Channel 4 Output, P1.23
 *                     - LPC_PWM_CHANNEL4_1 : PWM Channel 4 Output, P2.3
 *                     - LPC_PWM_CHANNEL5_0 : PWM Channel 5 Output, P1.24
 *                     - LPC_PWM_CHANNEL5_1 : PWM Channel 5 Output, P2.4
 *                     - LPC_PWM_CHANNEL6_0 : PWM Channel 6 Output, P1.26
 *                     - LPC_PWM_CHANNEL6_1 : PWM Channel 6 Output, P2.5
 * @param[in] period : PWM Pulsewidth in second
 * @return    - COX_SUCCESS : Success
 *            - COX_ERROR   : The PWM Output or With value were not supported
*******************************************************************************/
COX_Status LPC_PWM_Width (uint16_t pwmout, float width)
{
  uint8_t chn;
  uint32_t uClk;                            
  uint32_t value;

  LPC_PWM_PinCfg(pwmout);

  chn = LPC_PWM_Channel (pwmout);
  if(chn == LPC_PWM_CHANNEL_INVALID) {
    return COX_ERROR;
  }
   
  /* Get PWM Peripheral Clock */ 
  uClk = LPC_PCLK(LPC_PCLK_PWM1);

  value = uClk * width;  
  if(value == 0) return COX_ERROR;
  
  switch(chn) {
  case 1: PWM1->MR1 = value & 0xFFFFFFFF; break;
  case 2: PWM1->MR2 = value & 0xFFFFFFFF; break;
  case 3: PWM1->MR3 = value & 0xFFFFFFFF; break;
  case 4: PWM1->MR4 = value & 0xFFFFFFFF; break;
  case 5: PWM1->MR5 = value & 0xFFFFFFFF; break;
  case 6: PWM1->MR6 = value & 0xFFFFFFFF; break;
  }; 

  PWM1->LER |= PWM_LER_EN_MATCHn_LATCH(chn);

  return COX_SUCCESS;
}

/***************************************************************************//**
 * @brief     Setup PWM Output Period in microsecond
 * @param[in] pwmout : PWM output Pin, should be one of the following
 *                     - LPC_PWM_CHANNEL1_0 : PWM Channel 1 Output, P1.18
 *                     - LPC_PWM_CHANNEL1_1 : PWM Channel 1 Output, P2.0
 *                     - LPC_PWM_CHANNEL2_0 : PWM Channel 2 Output, P1.20
 *                     - LPC_PWM_CHANNEL2_1 : PWM Channel 2 Output, P2.1
 *                     - LPC_PWM_CHANNEL2_2 : PWM Channel 2 Output, P3.25
 *                     - LPC_PWM_CHANNEL3_0 : PWM Channel 3 Output, P1.21
 *                     - LPC_PWM_CHANNEL3_1 : PWM Channel 3 Output, P2.2
 *                     - LPC_PWM_CHANNEL3_2 : PWM Channel 3 Output, P3.26
 *                     - LPC_PWM_CHANNEL4_0 : PWM Channel 4 Output, P1.23
 *                     - LPC_PWM_CHANNEL4_1 : PWM Channel 4 Output, P2.3
 *                     - LPC_PWM_CHANNEL5_0 : PWM Channel 5 Output, P1.24
 *                     - LPC_PWM_CHANNEL5_1 : PWM Channel 5 Output, P2.4
 *                     - LPC_PWM_CHANNEL6_0 : PWM Channel 6 Output, P1.26
 *                     - LPC_PWM_CHANNEL6_1 : PWM Channel 6 Output, P2.5
 * @param[in] period : PWM Output Period in microsecond
 * @return    - COX_SUCCESS : Success
 *            - COX_ERROR   : The PWM Output was not supported
 *
 * @NOTE : The LPC17xx PWM hardware is limited to share the period value between 
 *         all outputs. Therefore, if you change the period of one output, you 
 *         change them all. The pulsewidth can be set independently for each 
 *         output.
*******************************************************************************/
COX_Status LPC_PWM_usPeriod (uint16_t pwmout, uint32_t period)
{
  uint32_t uClk;                            
  uint64_t value;
   
  /* Get PWM Peripheral Clock */ 
  uClk = LPC_PCLK(LPC_PCLK_PWM1);
  value = ((uClk/1000) * period) / 1000;
  if((value == 0) || (value > 0xFFFFFFFF)) return COX_ERROR;

  PWM1->MR0 = value & 0xFFFFFFFF;

  /* Write Latch register */
  PWM1->LER |= PWM_LER_EN_MATCHn_LATCH(0);
 
  return COX_SUCCESS;
}

/***************************************************************************//**
 * @brief     Set the PWM Pulsewidth in microsecond
 * @param[in] pwmout : PWM output Pin, should be one of the following
 *                     - LPC_PWM_CHANNEL1_0 : PWM Channel 1 Output, P1.18
 *                     - LPC_PWM_CHANNEL1_1 : PWM Channel 1 Output, P2.0
 *                     - LPC_PWM_CHANNEL2_0 : PWM Channel 2 Output, P1.20
 *                     - LPC_PWM_CHANNEL2_1 : PWM Channel 2 Output, P2.1
 *                     - LPC_PWM_CHANNEL2_2 : PWM Channel 2 Output, P3.25
 *                     - LPC_PWM_CHANNEL3_0 : PWM Channel 3 Output, P1.21
 *                     - LPC_PWM_CHANNEL3_1 : PWM Channel 3 Output, P2.2
 *                     - LPC_PWM_CHANNEL3_2 : PWM Channel 3 Output, P3.26
 *                     - LPC_PWM_CHANNEL4_0 : PWM Channel 4 Output, P1.23
 *                     - LPC_PWM_CHANNEL4_1 : PWM Channel 4 Output, P2.3
 *                     - LPC_PWM_CHANNEL5_0 : PWM Channel 5 Output, P1.24
 *                     - LPC_PWM_CHANNEL5_1 : PWM Channel 5 Output, P2.4
 *                     - LPC_PWM_CHANNEL6_0 : PWM Channel 6 Output, P1.26
 *                     - LPC_PWM_CHANNEL6_1 : PWM Channel 6 Output, P2.5
 * @param[in] period : PWM Pulsewidth in microsecond
 * @return    - COX_SUCCESS : Success
 *            - COX_ERROR   : The PWM Output or With value were not supported
*******************************************************************************/
COX_Status LPC_PWM_usWidth (uint16_t pwmout, uint32_t width)
{
  uint8_t chn;
  uint32_t uClk;                            
  uint64_t value;

  LPC_PWM_PinCfg(pwmout);

  chn = LPC_PWM_Channel (pwmout);
  if(chn == LPC_PWM_CHANNEL_INVALID) {
    return COX_ERROR;
  }
   
  /* Get PWM Peripheral Clock */ 
  uClk = LPC_PCLK(LPC_PCLK_PWM1);
  value = ((uClk/1000) * width)/1000;
  if(value > 0xFFFFFFFF) return COX_ERROR;
  
  switch(chn) {
  case 1: PWM1->MR1 = value & 0xFFFFFFFF; break;
  case 2: PWM1->MR2 = value & 0xFFFFFFFF; break;
  case 3: PWM1->MR3 = value & 0xFFFFFFFF; break;
  case 4: PWM1->MR4 = value & 0xFFFFFFFF; break;
  case 5: PWM1->MR5 = value & 0xFFFFFFFF; break;
  case 6: PWM1->MR6 = value & 0xFFFFFFFF; break;
  };  

  /* Write Latch register */
  PWM1->LER |= PWM_LER_EN_MATCHn_LATCH(chn);

  return COX_SUCCESS;
}

/***************************************************************************//**
 * @brief     Set the ouput duty-cycle, specified as a percentage
 * @param[in] pwmout : PWM output Pin, should be one of the following
 *                     - LPC_PWM_CHANNEL1_0 : PWM Channel 1 Output, P1.18
 *                     - LPC_PWM_CHANNEL1_1 : PWM Channel 1 Output, P2.0
 *                     - LPC_PWM_CHANNEL2_0 : PWM Channel 2 Output, P1.20
 *                     - LPC_PWM_CHANNEL2_1 : PWM Channel 2 Output, P2.1
 *                     - LPC_PWM_CHANNEL2_2 : PWM Channel 2 Output, P3.25
 *                     - LPC_PWM_CHANNEL3_0 : PWM Channel 3 Output, P1.21
 *                     - LPC_PWM_CHANNEL3_1 : PWM Channel 3 Output, P2.2
 *                     - LPC_PWM_CHANNEL3_2 : PWM Channel 3 Output, P3.26
 *                     - LPC_PWM_CHANNEL4_0 : PWM Channel 4 Output, P1.23
 *                     - LPC_PWM_CHANNEL4_1 : PWM Channel 4 Output, P2.3
 *                     - LPC_PWM_CHANNEL5_0 : PWM Channel 5 Output, P1.24
 *                     - LPC_PWM_CHANNEL5_1 : PWM Channel 5 Output, P2.4
 *                     - LPC_PWM_CHANNEL6_0 : PWM Channel 6 Output, P1.26
 *                     - LPC_PWM_CHANNEL6_1 : PWM Channel 6 Output, P2.5
 * @param[in] duty : duty-cycle, specified as a percentage
 * @return    - COX_SUCCESS : success
 *            - COX_ERROR   : The Duty-cycle was not supported
*******************************************************************************/
COX_Status LPC_PWM_Duty (uint16_t pwmout, float duty)
{
  uint32_t value;
  uint8_t  chn;

  if(duty > 1) return COX_ERROR;  

  LPC_PWM_PinCfg(pwmout);

  value = PWM1->MR0;
  value *= duty;
  chn = LPC_PWM_Channel (pwmout);
  if(chn == LPC_PWM_CHANNEL_INVALID) {
    return COX_ERROR;
  }

  switch(chn) {
  case 1: PWM1->MR1 = value; break;
  case 2: PWM1->MR2 = value; break;
  case 3: PWM1->MR3 = value; break;
  case 4: PWM1->MR4 = value; break;
  case 5: PWM1->MR5 = value; break;
  case 6: PWM1->MR6 = value; break;
  };  

  /* Write Latch register */
  PWM1->LER |= PWM_LER_EN_MATCHn_LATCH(chn);

  return COX_SUCCESS;
}

/***************************************************************************//**
 * @brief     Initiate PWM Output
 * @param[in] pwmout : PWM output Pin, should be one of the following
 *                     - LPC_PWM_CHANNEL1_0 : PWM Channel 1 Output, P1.18
 *                     - LPC_PWM_CHANNEL1_1 : PWM Channel 1 Output, P2.0
 *                     - LPC_PWM_CHANNEL2_0 : PWM Channel 2 Output, P1.20
 *                     - LPC_PWM_CHANNEL2_1 : PWM Channel 2 Output, P2.1
 *                     - LPC_PWM_CHANNEL2_2 : PWM Channel 2 Output, P3.25
 *                     - LPC_PWM_CHANNEL3_0 : PWM Channel 3 Output, P1.21
 *                     - LPC_PWM_CHANNEL3_1 : PWM Channel 3 Output, P2.2
 *                     - LPC_PWM_CHANNEL3_2 : PWM Channel 3 Output, P3.26
 *                     - LPC_PWM_CHANNEL4_0 : PWM Channel 4 Output, P1.23
 *                     - LPC_PWM_CHANNEL4_1 : PWM Channel 4 Output, P2.3
 *                     - LPC_PWM_CHANNEL5_0 : PWM Channel 5 Output, P1.24
 *                     - LPC_PWM_CHANNEL5_1 : PWM Channel 5 Output, P2.4
 *                     - LPC_PWM_CHANNEL6_0 : PWM Channel 6 Output, P1.26
 *                     - LPC_PWM_CHANNEL6_1 : PWM Channel 6 Output, P2.5
 * @return    - COX_SUCCESS : Success
 *            - COX_ERROR   : The PWM Output was not supported
*******************************************************************************/
COX_Status LPC_PWM_Start (uint16_t pwmout)
{
  uint8_t chn;

  chn = LPC_PWM_Channel (pwmout);
  if(chn == LPC_PWM_CHANNEL_INVALID) {
    return COX_ERROR;
  }

  PWM1->PCR |= PWM_PCR_PWMENAn(chn);
  PWM1->TCR |= 1 << 0; /* Enable Counter */

  return COX_SUCCESS;  
}

/***************************************************************************//**
 * @brief     Stop PWM Output
 * @param[in] pwmout : PWM output Pin, should be one of the following
 *                     - LPC_PWM_CHANNEL1_0 : PWM Channel 1 Output, P1.18
 *                     - LPC_PWM_CHANNEL1_1 : PWM Channel 1 Output, P2.0
 *                     - LPC_PWM_CHANNEL2_0 : PWM Channel 2 Output, P1.20
 *                     - LPC_PWM_CHANNEL2_1 : PWM Channel 2 Output, P2.1
 *                     - LPC_PWM_CHANNEL2_2 : PWM Channel 2 Output, P3.25
 *                     - LPC_PWM_CHANNEL3_0 : PWM Channel 3 Output, P1.21
 *                     - LPC_PWM_CHANNEL3_1 : PWM Channel 3 Output, P2.2
 *                     - LPC_PWM_CHANNEL3_2 : PWM Channel 3 Output, P3.26
 *                     - LPC_PWM_CHANNEL4_0 : PWM Channel 4 Output, P1.23
 *                     - LPC_PWM_CHANNEL4_1 : PWM Channel 4 Output, P2.3
 *                     - LPC_PWM_CHANNEL5_0 : PWM Channel 5 Output, P1.24
 *                     - LPC_PWM_CHANNEL5_1 : PWM Channel 5 Output, P2.4
 *                     - LPC_PWM_CHANNEL6_0 : PWM Channel 6 Output, P1.26
 *                     - LPC_PWM_CHANNEL6_1 : PWM Channel 6 Output, P2.5
 * @return    - COX_SUCCESS : Success
 *            - COX_ERROR   : The PWM Output was not supported
*******************************************************************************/
COX_Status LPC_PWM_Stop (uint16_t pwmout)
{
  uint8_t chn;
  chn = LPC_PWM_Channel (pwmout);
  if(chn == LPC_PWM_CHANNEL_INVALID) {
    return COX_ERROR;
  }

  PWM1->PCR &= ~PWM_PCR_PWMENAn(chn);
  return COX_SUCCESS;  
}

/***************************************************************************//**
 * Define Tick Interface based on SysTick
*******************************************************************************/
COX_PWM_PI pi_pwm = {
  LPC_PWM_Init,
  LPC_PWM_Period,
  LPC_PWM_Width,
  LPC_PWM_usPeriod,
  LPC_PWM_usWidth,
  LPC_PWM_Duty,
  LPC_PWM_Start,
  LPC_PWM_Stop
};
