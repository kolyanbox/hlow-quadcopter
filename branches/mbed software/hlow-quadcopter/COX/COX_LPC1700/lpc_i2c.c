/**************************************************************************//**
 * @file      : lpc_i2c.c
 * @brief     : LPC17xx I2C Master Peripheral Interface
 * @version   : V1.0
 * @date      : 30. Jun. 2010
 * @author    : CooCox
******************************************************************************/
#include "lpc_lowlayer.h"
#include "lpc_i2c.h"

#define LPC_I2C_I2C0     0      /* I2C0 */
#define LPC_I2C_I2C1     1      /* I2C1 */
#define LPC_I2C_I2C2     2      /* I2C2 */
#define LPC_I2C_NONE     0xFF   /* I2C3 */

#define LPC_I2C0_SDA     COX_PIN(0, 27)
#define LPC_I2C0_SCL     COX_PIN(0, 27)

#define LPC_I2C1_SDA     COX_PIN(0,  0)
#define LPC_I2C1_SCL     COX_PIN(0,  1)

#define LPC_I2C2_SDA     COX_PIN(0, 10)
#define LPC_I2C2_SCL     COX_PIN(0, 11)



/* I2C Power/Clock definition ------------------------------------- */
#define I2C0_PC_CTRL  ((uint32_t)1<<7)     /* I2C0 interface power/clock control */
#define I2C1_PC_CTRL  ((uint32_t)1<<19)    /* I2C1 interface power/clock control */
#define I2C2_PC_CTRL  ((uint32_t)1<<26)    /* I2C2 interface power/clock control */


/* I2C Status Code definition (I2C Status register) */
#define I2C_I2CONSET_AA         ((0x04))   /* Assert acknowledge flag */
#define I2C_I2CONSET_SI         ((0x08))   /* I2C interrupt flag */
#define I2C_I2CONSET_STO        ((0x10))   /* STOP flag */
#define I2C_I2CONSET_STA        ((0x20))   /* START flag */
#define I2C_I2CONSET_I2EN       ((0x40))   /* I2C interface enable */

/* I2C Control Clear register description */
/** Assert acknowledge Clear bit */
#define I2C_I2CONCLR_AAC                ((1<<2))
/** I2C interrupt Clear bit */
#define I2C_I2CONCLR_SIC                ((1<<3))
/** START flag Clear bit */
#define I2C_I2CONCLR_STAC               ((1<<5))
/** I2C interface Disable bit */
#define I2C_I2CONCLR_I2ENC              ((1<<6))

/* I2C Status Code definition (I2C Status register) */
/* Return Code in I2C status register */
#define I2C_STAT_CODE_BITMASK           ((0xF8))

/* I2C return status code definitions ----------------------------- */

/** No relevant information */
#define I2C_I2STAT_NO_INF               ((0xF8))

/* Master transmit mode -------------------------------------------- */
/** A start condition has been transmitted */
#define I2C_I2STAT_M_TX_START           ((0x08))
/** A repeat start condition has been transmitted */
#define I2C_I2STAT_M_TX_RESTART         ((0x10))
/** SLA+W has been transmitted, ACK has been received */
#define I2C_I2STAT_M_TX_SLAW_ACK        ((0x18))
/** SLA+W has been transmitted, NACK has been received */
#define I2C_I2STAT_M_TX_SLAW_NACK       ((0x20))
/** Data has been transmitted, ACK has been received */
#define I2C_I2STAT_M_TX_DAT_ACK         ((0x28))
/** Data has been transmitted, NACK has been received */
#define I2C_I2STAT_M_TX_DAT_NACK        ((0x30))
/** Arbitration lost in SLA+R/W or Data bytes */
#define I2C_I2STAT_M_TX_ARB_LOST        ((0x38))

/* Master receive mode -------------------------------------------- */
/** A start condition has been transmitted */
#define I2C_I2STAT_M_RX_START           ((0x08))
/** A repeat start condition has been transmitted */
#define I2C_I2STAT_M_RX_RESTART         ((0x10))
/** Arbitration lost */
#define I2C_I2STAT_M_RX_ARB_LOST        ((0x38))
/** SLA+R has been transmitted, ACK has been received */
#define I2C_I2STAT_M_RX_SLAR_ACK        ((0x40))
/** SLA+R has been transmitted, NACK has been received */
#define I2C_I2STAT_M_RX_SLAR_NACK       ((0x48))
/** Data has been received, ACK has been returned */
#define I2C_I2STAT_M_RX_DAT_ACK         ((0x50))
/** Data has been received, NACK has been return */
#define I2C_I2STAT_M_RX_DAT_NACK        ((0x58))

/***************************************************************************//**
 * @brief     Generate a start condition on I2C bus     
 * @param[in] I2Cx : I2C peripheral selected 
 * @return    value of I2C status register after generate a start condition
*******************************************************************************/
static uint32_t LPC_I2C_Start (I2C_TypeDef *I2Cx)
{
  I2Cx->I2CONCLR = I2C_I2CONCLR_SIC;
  I2Cx->I2CONSET = I2C_I2CONSET_STA;

  /* Wait for complete */
  while (!(I2Cx->I2CONSET & I2C_I2CONSET_SI));
  I2Cx->I2CONCLR = I2C_I2CONCLR_STAC;
  return (I2Cx->I2STAT & I2C_STAT_CODE_BITMASK);
}

/***************************************************************************//**
 * @brief     Generate a stop condition on I2C bus    
 * @param[in] I2Cx : I2C peripheral selected 
 * @return    None
*******************************************************************************/
static void LPC_I2C_Stop (I2C_TypeDef *I2Cx)
{
  /* Make sure start bit is not active */
  if (I2Cx->I2CONSET & I2C_I2CONSET_STA) {
    I2Cx->I2CONCLR = I2C_I2CONCLR_STAC;
  }

  I2Cx->I2CONSET = I2C_I2CONSET_STO;
  I2Cx->I2CONCLR = I2C_I2CONCLR_SIC;
}

/***************************************************************************//**
 * @brief     Send a byte    
 * @param[in] I2Cx : I2C peripheral selected 
 * @param[in] dat  : Data to send
 * @return    value of I2C status register after sending
*******************************************************************************/
static uint32_t LPC_I2C_SendByte (I2C_TypeDef *I2Cx, uint8_t dat)
{
  /* Make sure start bit is not active */
  if (I2Cx->I2CONSET & I2C_I2CONSET_STA) {
    I2Cx->I2CONCLR = I2C_I2CONCLR_STAC;
  }

  I2Cx->I2DAT = dat;
  I2Cx->I2CONCLR = I2C_I2CONCLR_SIC;
  
  while (!(I2Cx->I2CONSET & I2C_I2CONSET_SI));
  return (I2Cx->I2STAT & I2C_STAT_CODE_BITMASK);
}

/***************************************************************************//**
 * @brief     Get a byte    
 * @param[in] I2Cx : I2C peripheral selected 
 * @param[in] retdat : Pointer to read data
 * @param[in] ack : Response ACK or NACK
 * @return    value of I2C status register after receiving
*******************************************************************************/
static uint32_t LPC_I2C_GetByte (I2C_TypeDef *I2Cx, uint8_t *dat, uint8_t ack)
{
  if (ack == 1) {
    I2Cx->I2CONSET = I2C_I2CONSET_AA;
  }
  else {
    I2Cx->I2CONCLR = I2C_I2CONCLR_AAC;
  } 

  I2Cx->I2CONCLR = I2C_I2CONCLR_SIC;
  
  while (!(I2Cx->I2CONSET & I2C_I2CONSET_SI));
  *dat = (uint8_t) (I2Cx->I2DAT & 0xFF);
  return (I2Cx->I2STAT & I2C_STAT_CODE_BITMASK);
}

/***************************************************************************//**
 * @brief     Setup clock rate for I2C peripheral	      
 * @param[in] I2Cx : I2C peripheral selected, should be LPC_I2C
 * @param[in] target_clock : clock of SSP (Hz)
 * @return    The read data
*******************************************************************************/
static uint32_t LPC_I2C_SetClock (I2C_TypeDef *I2Cx, uint32_t rate)
{
  uint32_t prescale, clk, clk_bak;
  uint32_t clk_div;  
  
  /* The SSP clock is derived from the (main system oscillator / 2),
     so compute the best divider from that clock */    
  if (I2Cx == I2C0){
    clk = LPC_PCLK(LPC_PCLK_I2C0);
  } else if (I2Cx == I2C1) {
    clk = LPC_PCLK(LPC_PCLK_I2C1);
  } else if (I2Cx == I2C2){
    clk = LPC_PCLK(LPC_PCLK_I2C2);
  } else {
    return 0;
  }

  /* Get current clock rate */
  clk_bak = I2Cx->I2SCLH +  I2Cx->I2SCLL;  
  if(clk_bak != 0) {
    clk_bak = clk / clk_bak;  
  }  

  prescale = (LPC_CCLK() + rate/2)/ rate;
  
  clk_div = 1;      
  if(prescale > 0xFFFF) {
    clk_div = 2;
    prescale = prescale / 2;
    if(prescale > 0xFFFF) {
      clk_div = 4;
      prescale = prescale / 2;      
      if(prescale > 0xFFFF) {
        clk_div = 8;
        prescale = prescale / 2;
      }
    } 
  }

  if(prescale > 0xFFFF) prescale = 0xFFFF;

  if (I2Cx == I2C0){
    LPC_PCLK_Set(LPC_PCLK_I2C0, clk_div);
  } else if (I2Cx == I2C1) {
    LPC_PCLK_Set(LPC_PCLK_I2C1, clk_div);
  } else if (I2Cx == I2C2){
    LPC_PCLK_Set(LPC_PCLK_I2C2, clk_div);
  } else {
    return 0;
  }

  /* Set the I2C clock value to register */
  I2Cx->I2SCLH = (uint32_t)(prescale / 2);
  I2Cx->I2SCLL = (uint32_t)(prescale - I2Cx->I2SCLH);

  return clk_bak;
}

/***************************************************************************//**
 * @brief     Setup clock rate for I2C peripheral	      
 * @param[in] I2Cx : I2C peripheral selected, should be LPC_I2C
 * @param[in] target_clock : clock of SSP (Hz)
 * @return    Result, should be
 *              -COX_SUCCESS : No error occurred
 *              -COX_ERROR : Error occurred, parameter is not supported 
*******************************************************************************/
static COX_Status LPC_I2C_Init (I2C_TypeDef *I2Cx, uint32_t rate) 
{  	
  if(I2Cx == I2C0) {
    /* Enable I2C power/clock */
    SC->PCONP |= I2C0_PC_CTRL; 
    
    /* P0.27->SDA0, P0.28->SCL0 */
    PINCON->PINSEL1 &= ~((uint32_t)3<<22 | (uint32_t)3<<24); 
    PINCON->PINSEL1 |=  ((uint32_t)1<<22 | (uint32_t)1<<24);    
  }
  else if (I2Cx == I2C1) {
    /* Enable I2C power/clock */
    SC->PCONP |= I2C1_PC_CTRL; 
    
    /* P0.0->SDA1, P0.1->SCL1 */
    PINCON->PINSEL0 &= ~((uint32_t)3<<0 | (uint32_t)3<<2); 
    PINCON->PINSEL0 |=  ((uint32_t)3<<0 | (uint32_t)3<<2); 
    
    PINCON->PINMODE_OD0 |= ((uint32_t)1<<0 | (uint32_t)1<<1);    
  }
  else if (I2Cx == I2C2) {
    /* Enable I2C power/clock */
    SC->PCONP |= I2C2_PC_CTRL;   

    /* P0.10->SDA2, P0.11->SCL2 */
    PINCON->PINSEL0 &= ~((uint32_t)3<<20 | (uint32_t)3<<22); 
    PINCON->PINSEL0 |=  ((uint32_t)2<<20 | (uint32_t)2<<22); 
    PINCON->PINMODE_OD0 |= ((uint32_t)1<<10 | (uint32_t)1<<11);       
  } else {
    return COX_ERROR;
  }
	
  /* Set clock rate */
  LPC_I2C_SetClock(I2Cx, rate);

  /* Set I2C operation to default */
  I2Cx->I2CONCLR = (I2C_I2CONCLR_AAC | I2C_I2CONCLR_STAC | I2C_I2CONCLR_I2ENC);

  /* Enable I2C Peripheral */  
  I2Cx->I2CONSET = I2C_I2CONSET_I2EN;

  return COX_SUCCESS;
}

/***************************************************************************//**
 * @brief     Transmit and Receive data      
 * @param[in] I2Cx : I2C peripheral selected, should be LPC_I2C
 * @param[in] cfg : Pointer to a I2C_Transfer_Cfg structure that contains 
 *                  specified information about the configuration for master 
 *                  transfer.
 * @return    Result, should be
 *              -COX_SUCCESS : No error occurred
 *              -COX_ERROR : Error occurred, parameter is not supported 
 * Note:
 * - In case of using I2C to transmit data only, either transmit length set to 0
 *   or transmit data pointer set to NULL.
 * - In case of using I2C to receive data only, either receive length set to 0
 *   or receive data pointer set to NULL.
 * - In case of using I2C to transmit followed by receive data, transmit length,
 *   transmit data pointer, receive length and receive data pointer should be set
 *   corresponding.
*******************************************************************************/
static COX_Status LPC_I2C_Transfer (I2C_TypeDef *I2Cx, I2C_Transfer_Cfg *cfg) 
{
  uint8_t *txdat;
  uint8_t *rxdat;
  uint32_t CodeStatus;
  uint8_t tmp;

  // reset all default state
  txdat = (uint8_t *) cfg->wbuf;
  rxdat = (uint8_t *) cfg->rbuf;
  // Reset I2C setup value to default state
  cfg->wcnt = 0;
  cfg->rcnt = 0;

  CodeStatus = 0;

  // Start command
  CodeStatus = LPC_I2C_Start(I2Cx);
  if ((CodeStatus != I2C_I2STAT_M_TX_START) && \
      (CodeStatus != I2C_I2STAT_M_TX_RESTART)) {
    goto I2C_ERR_Process;
  }

  /* In case of sending data first */
  if ((cfg->wlen != 0) && (cfg->wbuf != COX_NULL)) {

    /* Send slave address + WR direction bit = 0 */
    CodeStatus = LPC_I2C_SendByte(I2Cx, (cfg->slave << 1));
    if (CodeStatus != I2C_I2STAT_M_TX_SLAW_ACK) {
      goto I2C_ERR_Process;
    }

    /* Send a number of data bytes */
    while (cfg->wcnt < cfg->wlen) {
      CodeStatus = LPC_I2C_SendByte(I2Cx, *txdat);
      if (CodeStatus != I2C_I2STAT_M_TX_DAT_ACK) {
        goto I2C_ERR_Process;
      }

      txdat++;
      cfg->wcnt++;
    }
  }

  /* Second Start condition (Repeat Start) */
  if ((cfg->wlen != 0) && (cfg->wbuf != COX_NULL) && \
      (cfg->rlen != 0) && (cfg->rbuf != COX_NULL)) {

    CodeStatus = LPC_I2C_Start(I2Cx);
    if ((CodeStatus != I2C_I2STAT_M_RX_START) && \
        (CodeStatus != I2C_I2STAT_M_RX_RESTART)) {
      goto I2C_ERR_Process;
    }
  }

  /* Then, start reading after sending data */
  if ((cfg->rlen != 0) && (cfg->rbuf != COX_NULL)) {
    /* Send slave address + RD direction bit = 1 */

    CodeStatus = LPC_I2C_SendByte(I2Cx, ((cfg->slave << 1) | 0x01));
    if (CodeStatus != I2C_I2STAT_M_RX_SLAR_ACK) {
      goto I2C_ERR_Process;
    }

    /* Receive a number of data bytes */
    while (cfg->rcnt < cfg->rlen) {

    /*
     * Note that: if data length is only one, the master should not
     * issue an ACK signal on bus after reading to avoid of next data frame
     * on slave side
     */
    if (cfg->rcnt < (cfg->rlen - 1)) {
      // Issue an ACK signal for next data frame
      CodeStatus = LPC_I2C_GetByte(I2Cx, &tmp, 1);
      if (CodeStatus != I2C_I2STAT_M_RX_DAT_ACK) {
        goto I2C_ERR_Process;
      }
    } else {
      // Do not issue an ACK signal
      CodeStatus = LPC_I2C_GetByte(I2Cx, &tmp, 0);
      if (CodeStatus != I2C_I2STAT_M_RX_DAT_NACK) {
        goto I2C_ERR_Process;
      }
    }
    *rxdat++ = tmp;
    cfg->rcnt++;
    }
  }

  /* Send STOP condition */
  LPC_I2C_Stop(I2Cx);
  return COX_SUCCESS;

I2C_ERR_Process:
  LPC_I2C_Stop(I2Cx);
  return COX_ERROR;  
}

/***************************************************************************//**
 * @brief     Check whether the slave device is ok or not	      
 * @param[in] I2Cx : I2C peripheral selected, should be LPC_I2C
 * @param[in] slave_adr : slave device selected
 * @return    Result, should be
 *              -COX_SUCCESS : Device is ok
 *              -COX_ERROR : Error occurred, bus error or device is not found
*******************************************************************************/
static COX_Status LPC_I2C_CheckSlave (I2C_TypeDef *I2Cx, uint8_t slave_adr)
{
  uint32_t CodeStatus = 0;
  /* Start command */
  CodeStatus = LPC_I2C_Start(I2Cx);
  if ((CodeStatus != I2C_I2STAT_M_TX_START) && \
     (CodeStatus != I2C_I2STAT_M_TX_RESTART) &&
     (CodeStatus != I2C_I2STAT_M_RX_START) && 
     (CodeStatus != I2C_I2STAT_M_RX_RESTART)) {
    
    goto I2C_ERR_Process;
  }

  /* Send slave address + WR direction bit = 0 */
  CodeStatus = LPC_I2C_SendByte(I2Cx, (slave_adr << 1));
  if (CodeStatus != I2C_I2STAT_M_TX_SLAW_ACK) {
	  
    goto I2C_ERR_Process;
  }

  LPC_I2C_Stop(I2Cx); 
  return COX_SUCCESS;

I2C_ERR_Process:
  LPC_I2C_Stop(I2Cx); 
  return COX_ERROR;
}

/***************************************************************************//**
 * @brief     Reset I2C bus, generate a start conditon followed by a stop condition
 * @param[in] I2Cx : I2C peripheral selected, should be LPC_I2C
 * @param[in] slave_adr : slave device selected
 * @return    Result, should be
 *              -COX_SUCCESS : Reset OK
 *              -COX_ERROR : Error occurred, bus error
*******************************************************************************/
static COX_Status LPC_I2C_BusReset (I2C_TypeDef *I2Cx)
{
  uint32_t CodeStatus = 0;
  
  /* Start command */
  CodeStatus = LPC_I2C_Start(I2Cx);
  if ((CodeStatus != I2C_I2STAT_M_TX_START) && \
      (CodeStatus != I2C_I2STAT_M_TX_RESTART) &&
      (CodeStatus != I2C_I2STAT_M_RX_START) && 
      (CodeStatus != I2C_I2STAT_M_RX_RESTART)) {
    return COX_ERROR;
  }

  LPC_I2C_Stop(I2Cx);
  return COX_SUCCESS;
}

/***************************************************************************//**
 * @brief     Configure I2C peripheral
 * @param[in] I2Cx : I2C Peripheral selected
 * @param[in] index : Configuration option, should be
 *                    -COX_I2C_CFG_RATE : clock rate
 * @param[in] arg : Argument
 * @return    Result, should be
 *              -COX_ERROR : The option is not support 
 *              -COX_SUCCESS
*******************************************************************************/
static COX_Status LPC_I2C_Cfg (I2C_TypeDef *I2Cx, uint8_t index, uint32_t arg, uint32_t *pre_arg)
{
  COX_Status result = COX_ERROR;
  uint32_t arg_bak;

  switch(index) {
  case COX_I2C_CFG_RATE : 
    arg_bak = LPC_I2C_SetClock(I2Cx, arg); 
    result = COX_SUCCESS;
    break;
  default : break;
  }

  if((result == COX_SUCCESS) && (pre_arg != COX_NULL)) {
    *pre_arg = arg_bak;  
  }

  return result;
}

static COX_Status LPC_I2C0_Init (uint32_t rate) { return LPC_I2C_Init(I2C0, rate); } 
static COX_Status LPC_I2C1_Init (uint32_t rate) { return LPC_I2C_Init(I2C1, rate); } 
static COX_Status LPC_I2C2_Init (uint32_t rate) { return LPC_I2C_Init(I2C2, rate); } 

static COX_Status LPC_I2C0_Transfer (I2C_Transfer_Cfg *cfg)  { return LPC_I2C_Transfer(I2C0, cfg); }
static COX_Status LPC_I2C1_Transfer (I2C_Transfer_Cfg *cfg)  { return LPC_I2C_Transfer(I2C1, cfg); }
static COX_Status LPC_I2C2_Transfer (I2C_Transfer_Cfg *cfg)  { return LPC_I2C_Transfer(I2C2, cfg); }

static COX_Status LPC_I2C0_CheckSlave (uint8_t adr) { return LPC_I2C_CheckSlave(I2C0, adr); }
static COX_Status LPC_I2C1_CheckSlave (uint8_t adr) { return LPC_I2C_CheckSlave(I2C1, adr); }
static COX_Status LPC_I2C2_CheckSlave (uint8_t adr) { return LPC_I2C_CheckSlave(I2C2, adr); }

static COX_Status LPC_I2C0_BusReset (void) { return LPC_I2C_BusReset(I2C0); }
static COX_Status LPC_I2C1_BusReset (void) { return LPC_I2C_BusReset(I2C1); }
static COX_Status LPC_I2C2_BusReset (void) { return LPC_I2C_BusReset(I2C2); }

static COX_Status LPC_I2C0_Cfg (uint8_t index, uint32_t arg, uint32_t *pre_arg) { return LPC_I2C_Cfg(I2C0, index, arg, pre_arg); }
static COX_Status LPC_I2C1_Cfg (uint8_t index, uint32_t arg, uint32_t *pre_arg) { return LPC_I2C_Cfg(I2C1, index, arg, pre_arg); }
static COX_Status LPC_I2C2_Cfg (uint8_t index, uint32_t arg, uint32_t *pre_arg) { return LPC_I2C_Cfg(I2C2, index, arg, pre_arg); }

/***************************************************************************//**
 * Define LPC17xx COX I2C Peripheral Interface based on I2C0
*******************************************************************************/
COX_I2C_PI pi_i2c0 = 
{
  LPC_I2C0_Init,      
  LPC_I2C0_Transfer,
  LPC_I2C0_CheckSlave,
  LPC_I2C0_BusReset,
  LPC_I2C0_Cfg,  
};

/***************************************************************************//**
 * Define LPC17xx COX I2C Peripheral Interface based on I2C1
*******************************************************************************/
COX_I2C_PI pi_i2c1 = 
{
  LPC_I2C1_Init,      
  LPC_I2C1_Transfer,
  LPC_I2C1_CheckSlave,
  LPC_I2C1_BusReset,
  LPC_I2C1_Cfg,  
};

/***************************************************************************//**
 * Define LPC17xx COX I2C Peripheral Interface based on I2C2
*******************************************************************************/
COX_I2C_PI pi_i2c2 = 
{
  LPC_I2C2_Init,      
  LPC_I2C2_Transfer,
  LPC_I2C2_CheckSlave,
  LPC_I2C2_BusReset,
  LPC_I2C2_Cfg,  
};

