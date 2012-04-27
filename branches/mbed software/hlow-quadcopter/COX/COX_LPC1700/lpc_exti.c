/***************************************************************************//**
 * @file      : lpc_exti.c
 * @brief     : LPC17xx EXTI Interface
 * @version   : V1.0
 * @date      : 12. Jul. 2010
 * @author    : CooCox
*******************************************************************************/
#include "lpc_lowlayer.h"
#include "cox_exti.h"  

static EXTI_Handler lpc_exti_handler = COX_NULL;     /* Interrupt Handler */

/***************************************************************************//**
 * @brief     External Interrupt 3 Handler
 * @param[in] None
 * @return    None
*******************************************************************************/
void EINT3_IRQHandler(void)     
{  
  uint32_t status;
  uint8_t cnt;
   
  /* Port 0 Interrupt */
  status  = (GPIOINT->IO0IntStatR & GPIOINT->IO0IntEnR) | (GPIOINT->IO0IntStatF & GPIOINT->IO0IntEnF);
  GPIOINT->IO0IntClr = status;  

  for(cnt=0; (status != 0) && (cnt<31); cnt++) {
    if(status & ((uint32_t)1<<cnt)) {
      status &= ~((uint32_t)1<<cnt);      
      if(lpc_exti_handler != COX_NULL) {
        lpc_exti_handler(COX_PIN(0, cnt));
      }      
    }
  } 
        
  /* Port 2  Interrupt */
  status  = (GPIOINT->IO2IntStatR & GPIOINT->IO2IntEnR) | (GPIOINT->IO2IntStatF & GPIOINT->IO2IntEnF);  
  GPIOINT->IO2IntClr = status;
  for(cnt=0; (status != 0) && (cnt<31); cnt++) {
    if(status & ((uint32_t)1<<cnt)) {
      status &= ~((uint32_t)1<<cnt);
      if(lpc_exti_handler != COX_NULL) {
        lpc_exti_handler(COX_PIN(2, cnt));
      }
      GPIOINT->IO2IntClr = ((uint32_t)1<<cnt);
    }    
  }
} 

/***************************************************************************//**
 * @brief     Setup EXTI Interrupt Service Routine Call-Back Function
 * @param[in] isr : Pointer to the call-back function
 * @return    None
*******************************************************************************/
static void LPC_EXTI_Handler (EXTI_Handler handler)
{
  lpc_exti_handler = handler;
  if(handler != COX_NULL) {
    NVIC_EnableIRQ(EINT3_IRQn);  
  }
  else {
    NVIC_DisableIRQ(EINT3_IRQn);
  } 
}

/***************************************************************************//**
 * @brief     Select Trigger mode
 * @param[in] pin : Specifie the EXTI line
 * @param[in] trigger : Specifie the trigger signal active mode, should be
 *              - COX_EXTI_NONE : None interrupt
 *              - COX_EXTI_EDGE_RISING : Rising edge interrupt
 *              - COX_EXTI_EDGE_FALLING : Falling edge interrupt
 *              - COX_EXTI_EDGE_RISING_FALLING : Both edge interrupt (Rising and Falling) 
 * @return    COX_SUCCESS : Success
 *            COX_ERROR : Error, parament error
*******************************************************************************/
COX_Status LPC_EXTI_Event (COX_PIN_Dev pin, uint8_t event)
{
  uint8_t port_num;
  uint8_t pin_num;
  COX_Status err = COX_ERROR;

  port_num = COX_PORT_NUM(pin); /* Get Port Number */
  pin_num  = COX_PIN_NUM(pin);  /* Get Pin Number  */

  if(port_num == 0) {        /* Poort 0 */
    /* P0.12 - P0.14, P0.31 are Reserved     
     * P0.0 - P0.11, P0.15-P0,30 are valid */
    if(pin_num <= 11 || ((pin_num>=15) && (pin_num <= 30))) {
      switch(event) {      
      case COX_EXTI_NONE :        
        GPIOINT->IO0IntEnR &= ~((uint32_t)1 << pin_num);   /* Interrupt Dsiable for Rising Edge  */
        GPIOINT->IO0IntEnF &= ~((uint32_t)1 << pin_num);   /* Interrupt Dsiable for Falling Edge */        
        err = COX_SUCCESS; break;

      case COX_EXTI_EDGE_RISING:
        GPIOINT->IO0IntEnR |=  ((uint32_t)1 << pin_num);   /* Interrupt Enable  for Rising  Edge */
        GPIOINT->IO0IntEnF &= ~((uint32_t)1 << pin_num);   /* Interrupt Dsiable for Falling Edge */        
        err = COX_SUCCESS; break;

      case COX_EXTI_EDGE_FALLING:
        GPIOINT->IO0IntEnR &= ~((uint32_t)1 << pin_num);   /* Interrupt Dsiable for Rising  Edge */
        GPIOINT->IO0IntEnF |=  ((uint32_t)1 << pin_num);   /* Interrupt Enable  for Falling Edge */        
        err = COX_SUCCESS; break;
          
      case COX_EXTI_EDGE_RISING_FALLING:
        GPIOINT->IO0IntEnR |=  ((uint32_t)1 << pin_num);   /* Interrupt Enable  for Rising  Edge */
        GPIOINT->IO0IntEnF |=  ((uint32_t)1 << pin_num);   /* Interrupt Enable  for Falling Edge */        
        err = COX_SUCCESS; break;

      case COX_EXTI_LEVEL_HIGH:        
      case COX_EXTI_LEVEL_LOW: 
      default : break;     
      }; 
    }      
  }
  else if (port_num == 2) {  /* Poort 2 */
  /* P2.14 - P2.31 are Reserved     
   * P2.10 - P2.13 are used as external interrupt function */
    if(pin_num <= 13) {       /* GPIO Interrupt */
      switch(event) {      
      case COX_EXTI_NONE :
        GPIOINT->IO2IntEnR &= ~((uint32_t)1 << pin_num);   /* Interrupt Dsiable for Rising Edge  */
        GPIOINT->IO2IntEnF &= ~((uint32_t)1 << pin_num);   /* Interrupt Dsiable for Falling Edge */        
        err = COX_SUCCESS; break;

      case COX_EXTI_EDGE_RISING:
        GPIOINT->IO2IntEnR |=  ((uint32_t)1 << pin_num);   /* Interrupt Enable  for Rising  Edge */
        GPIOINT->IO2IntEnF &= ~((uint32_t)1 << pin_num);   /* Interrupt Dsiable for Falling Edge */        
        err = COX_SUCCESS; break;

      case COX_EXTI_EDGE_FALLING:
        GPIOINT->IO2IntEnR &= ~((uint32_t)1 << pin_num);   /* Interrupt Dsiable for Rising  Edge */
        GPIOINT->IO2IntEnF |=  ((uint32_t)1 << pin_num);   /* Interrupt Enable  for Falling Edge */        
        err = COX_SUCCESS; break;
          
      case COX_EXTI_EDGE_RISING_FALLING:
        GPIOINT->IO2IntEnR |=  ((uint32_t)1 << pin_num);   /* Interrupt Enable  for Rising  Edge */
        GPIOINT->IO2IntEnF |=  ((uint32_t)1 << pin_num);   /* Interrupt Enable  for Falling Edge */        
        err = COX_SUCCESS; break;

      case COX_EXTI_LEVEL_HIGH:        
      case COX_EXTI_LEVEL_LOW: 
      default : break;     
      };      
    }
  }

  return err;
}

/***************************************************************************//**
 * @brief     Initialize the EXTI line
 * @param[in] pin : Specifie the EXTI line
 * @param[in] trigger : Specifie the trigger signal active mode, should be
 *              - COX_EXTI_NONE : None interrupt
 *              - COX_EXTI_EDGE_RISING : Rising edge interrupt
 *              - COX_EXTI_EDGE_FALLING : Falling edge interrupt
 *              - COX_EXTI_EDGE_RISING_FALLING : Both edge interrupt (Rising and Falling) 
 * @return    COX_SUCCESS : Success
 *            COX_ERROR : Error, parament error
*******************************************************************************/
static COX_Status LPC_EXTI_Init (uint16_t pin)
{    
  COX_Status err = COX_ERROR;
  uint8_t port_num;
  uint8_t pin_num;

  port_num = COX_PORT_NUM(pin); /* Get Port Number */
  pin_num  = COX_PIN_NUM(pin);  /* Get Pin Number  */

  /* Poort 0
   * P0.12 - P0.14, P0.31 are Reserved     
   * P0.0 - P0.11, P0.15-P0,30 are valid */
  if(port_num == 0) {            
    if(pin_num <= 11 || ((pin_num>=15) && (pin_num <= 30))) {      
      
      GPIOINT->IO0IntEnR &= ~((uint32_t)1 << pin_num);   /* Dsiable for Rising Edge Interrupt */
      GPIOINT->IO0IntEnF &= ~((uint32_t)1 << pin_num);   /* Dsiable Interrupt for Rising Edge */      
      GPIO0->FIODIR      &= ~((uint32_t)1 << pin_num);   /* Set Direction as input */      
      GPIOINT->IO0IntClr  =  ((uint32_t)1 << pin_num);   /* Clear Interrupt status */
      err = COX_SUCCESS;      
    }      
  }  
  /* Port 2
   * P2.14 - P2.31 are Reserved     
   * P2.10 - P2.13 are used as external interrupt function */
  else if ((port_num == 2) && (pin_num <= 13)) {          
    GPIOINT->IO2IntEnR &= ~((uint32_t)1 << pin_num);   /* Dsiable for Rising Edge Interrupt */
    GPIOINT->IO2IntEnF &= ~((uint32_t)1 << pin_num);   /* Dsiable Interrupt for Rising Edge */      
    GPIO2->FIODIR      &= ~((uint32_t)1 << pin_num);   /* Set Direction as input */      
    GPIOINT->IO2IntClr  =  ((uint32_t)1 << pin_num);   /* Clear Interrupt status */
    err = COX_SUCCESS;    
  }    

  if(err == COX_SUCCESS) {
    LPC_Pin_Func((pin>>8) & 0xFF, (pin>>0) & 0xFF, PIN_CFG_FUNC_0);
    LPC_Pin_Mode((pin>>8) & 0xFF, (pin>>0) & 0xFF, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD  ((pin>>8) & 0xFF, (pin>>0) & 0xFF, PIN_CFG_OD_NORMAL);    
  }
  
  return err;
}

/***************************************************************************//**
 * Define LPC17xx COX EXTI Interface based on GPIO
*******************************************************************************/
COX_EXTI_PI pi_exti = {
  LPC_EXTI_Handler,
  LPC_EXTI_Init,
  LPC_EXTI_Event,
};
