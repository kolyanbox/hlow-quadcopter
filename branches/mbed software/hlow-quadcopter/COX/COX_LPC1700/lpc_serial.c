/***************************************************************************//**
 * @file      : lpc_serial.c
 * @brief     : LPC17xx Serial Peripheral Interface
 * @version   : V1.0
 * @date      : 8. Jul. 2010
 * @author    : CooCox
*******************************************************************************/
#include "lpc_lowlayer.h"
#include "lpc_serial.h"

#define LPC_UART0_SERIAL    0x00

#define LPC_UART1_SERIAL0   0x10
#define LPC_UART1_SERIAL1   0x11

#define LPC_UART2_SERIAL0   0x20
#define LPC_UART2_SERIAL1   0x21

#define LPC_UART3_SERIAL0   0x30
#define LPC_UART3_SERIAL1   0x31

#define LPC_UART_NONE     0xFF

#define LPC_UART0_SERIAL_TXD    COX_PIN(0,  2)
#define LPC_UART0_SERIAL_RXD    COX_PIN(0,  3)

#define LPC_UART1_SERIAL0_TXD   COX_PIN(0, 15)
#define LPC_UART1_SERIAL0_RXD   COX_PIN(0, 16)

#define LPC_UART1_SERIAL1_TXD   COX_PIN(2,  0)
#define LPC_UART1_SERIAL1_RXD   COX_PIN(2,  1)

#define LPC_UART2_SERIAL0_TXD   COX_PIN(0, 10)
#define LPC_UART2_SERIAL0_RXD   COX_PIN(0, 11)

#define LPC_UART2_SERIAL1_TXD   COX_PIN(2,  8)
#define LPC_UART2_SERIAL1_RXD   COX_PIN(2,  9)

#define LPC_UART3_SERIAL0_TXD   COX_PIN(0,  0)
#define LPC_UART3_SERIAL0_RXD   COX_PIN(0,  1)

#define LPC_UART3_SERIAL1_TXD   COX_PIN(4, 28)
#define LPC_UART3_SERIAL1_RXD   COX_PIN(4, 29)

/* Accepted Error baud rate value (in percent unit) */
#define UART_ACCEPTED_BAUDRATE_ERROR      (3)  /*!< Acceptable UART baudrate error */

/* Macro defines for UARTn Receiver Buffer Register */
#define UART_RBR_MASKBIT    ((uint8_t)0xFF)  /*!< UART Received Buffer mask bit (8 bits) */
                          
/* Macro defines for UARTn Transmit Holding Register */
#define UART_THR_MASKBIT    ((uint8_t)0xFF)  /*!< UART Transmit Holding mask bit (8 bits) */

/* Macro defines for UARTn Divisor Latch LSB register */
#define UART_LOAD_DLL(div)  ((div) & 0xFF)   /**< Macro for loading least significant halfs of divisors */
#define UART_DLL_MASKBIT    ((uint8_t)0xFF)  /*!< Divisor latch LSB bit mask */

/* Macro defines for UARTn Divisor Latch MSB register */
#define UART_DLM_MASKBIT    ((uint8_t)0xFF)  /*!< Divisor latch MSB bit mask */
#define UART_LOAD_DLM(div)  (((div) >> 8) & 0xFF)  /**< Macro for loading most significant halfs of divisors */


/* Macro defines for UART interrupt enable register */
#define UART_IER_RBRINT_EN     ((uint32_t)(1<<0))  /*!< RBR Interrupt enable*/
#define UART_IER_THREINT_EN    ((uint32_t)(1<<1))  /*!< THR Interrupt enable*/
#define UART_IER_RLSINT_EN     ((uint32_t)(1<<2))  /*!< RX line status interrupt enable*/
#define UART1_IER_MSINT_EN     ((uint32_t)(1<<3))  /*!< Modem status interrupt enable */
#define UART1_IER_CTSINT_EN    ((uint32_t)(1<<7))  /*!< CTS1 signal transition interrupt enable */
#define UART_IER_ABEOINT_EN    ((uint32_t)(1<<8))  /*!< Enables the end of auto-baud interrupt */
#define UART_IER_ABTOINT_EN    ((uint32_t)(1<<9))  /*!< Enables the auto-baud time-out interrupt */
#define UART_IER_BITMASK       ((uint32_t)(0x307)) /*!< UART interrupt enable register bit mask */
#define UART1_IER_BITMASK      ((uint32_t)(0x38F)) /*!< UART1 interrupt enable register bit mask */


/* UART interrupt identification register defines */
#define UART_IIR_INTSTAT_PEND  ((uint32_t)(1<<0))   /*!<Interrupt Status - Active low */
#define UART_IIR_INTID_RLS     ((uint32_t)(3<<1))   /*!<Interrupt identification: Receive line status*/
#define UART_IIR_INTID_RDA     ((uint32_t)(2<<1))   /*!<Interrupt identification: Receive data available*/
#define UART_IIR_INTID_CTI     ((uint32_t)(6<<1))   /*!<Interrupt identification: Character time-out indicator*/
#define UART_IIR_INTID_THRE    ((uint32_t)(1<<1))   /*!<Interrupt identification: THRE interrupt*/
#define UART1_IIR_INTID_MODEM  ((uint32_t)(0<<1))   /*!<Interrupt identification: Modem interrupt*/
#define UART_IIR_INTID_MASK    ((uint32_t)(7<<1))   /*!<Interrupt identification: Interrupt ID mask */
#define UART_IIR_FIFO_EN       ((uint32_t)(3<<6))   /*!<These bits are equivalent to UnFCR[0] */
#define UART_IIR_ABEO_INT      ((uint32_t)(1<<8))   /*!< End of auto-baud interrupt */
#define UART_IIR_ABTO_INT      ((uint32_t)(1<<9))   /*!< Auto-baud time-out interrupt */
#define UART_IIR_BITMASK       ((uint32_t)(0x3CF))  /*!< UART interrupt identification register bit mask */


/* Macro defines for UART FIFO control register */
#define UART_FCR_FIFO_EN       ((uint8_t)(1<<0))  /*!< UART FIFO enable */
#define UART_FCR_RX_RS         ((uint8_t)(1<<1))  /*!< UART FIFO RX reset */
#define UART_FCR_TX_RS         ((uint8_t)(1<<2))  /*!< UART FIFO TX reset */
#define UART_FCR_DMAMODE_SEL   ((uint8_t)(1<<3))  /*!< UART DMA mode selection */
#define UART_FCR_TRG_LEV0      ((uint8_t)(0))     /*!< UART FIFO trigger level 0: 1 character */
#define UART_FCR_TRG_LEV1      ((uint8_t)(1<<6))  /*!< UART FIFO trigger level 1: 4 character */
#define UART_FCR_TRG_LEV2      ((uint8_t)(2<<6))  /*!< UART FIFO trigger level 2: 8 character */
#define UART_FCR_TRG_LEV3      ((uint8_t)(3<<6))  /*!< UART FIFO trigger level 3: 14 character */
#define UART_FCR_BITMASK       ((uint8_t)(0xCF))  /*!< UART FIFO control bit mask */
#define UART_TX_FIFO_SIZE      (16)

/* Macro defines for UART line control register */
#define UART_LCR_WLEN5        ((uint8_t)(0))     /*!< UART 5 bit data mode */
#define UART_LCR_WLEN6        ((uint8_t)(1<<0))  /*!< UART 6 bit data mode */
#define UART_LCR_WLEN7        ((uint8_t)(2<<0))  /*!< UART 7 bit data mode */
#define UART_LCR_WLEN8        ((uint8_t)(3<<0))  /*!< UART 8 bit data mode */
#define UART_LCR_STOPBIT_SEL  ((uint8_t)(1<<2))  /*!< UART Two Stop Bits Select */
#define UART_LCR_PARITY_EN    ((uint8_t)(1<<3))  /*!< UART Parity Enable */
#define UART_LCR_PARITY_ODD   ((uint8_t)(0))     /*!< UART Odd Parity Select */
#define UART_LCR_PARITY_EVEN  ((uint8_t)(1<<4))  /*!< UART Even Parity Select */
#define UART_LCR_PARITY_F_1   ((uint8_t)(2<<4))  /*!< UART force 1 stick parity */
#define UART_LCR_PARITY_F_0   ((uint8_t)(3<<4))  /*!< UART force 0 stick parity */
#define UART_LCR_BREAK_EN     ((uint8_t)(1<<6))  /*!< UART Transmission Break enable */
#define UART_LCR_DLAB_EN      ((uint8_t)(1<<7))  /*!< UART Divisor Latches Access bit enable */
#define UART_LCR_BITMASK      ((uint8_t)(0xFF))  /*!< UART line control bit mask */


/* Macro defines for UART1 Modem Control Register */
#define UART1_MCR_DTR_CTRL     ((uint8_t)(1<<0))   /*!< Source for modem output pin DTR */
#define UART1_MCR_RTS_CTRL     ((uint8_t)(1<<1))   /*!< Source for modem output pin RTS */
#define UART1_MCR_LOOPB_EN     ((uint8_t)(1<<4))   /*!< Loop back mode select */
#define UART1_MCR_AUTO_RTS_EN  ((uint8_t)(1<<6))   /*!< Enable Auto RTS flow-control */
#define UART1_MCR_AUTO_CTS_EN  ((uint8_t)(1<<7))   /*!< Enable Auto CTS flow-control */
#define UART1_MCR_BITMASK      ((uint8_t)(0x0F3))  /*!< UART1 bit mask value */


/* Macro defines for UART line status register */
#define UART_LSR_RDR      ((uint8_t)(1<<0))  /*!<Line status register: Receive data ready*/
#define UART_LSR_OE       ((uint8_t)(1<<1))  /*!<Line status register: Overrun error*/
#define UART_LSR_PE       ((uint8_t)(1<<2))  /*!<Line status register: Parity error*/
#define UART_LSR_FE       ((uint8_t)(1<<3))  /*!<Line status register: Framing error*/
#define UART_LSR_BI       ((uint8_t)(1<<4))  /*!<Line status register: Break interrupt*/
#define UART_LSR_THRE     ((uint8_t)(1<<5))  /*!<Line status register: Transmit holding register empty*/
#define UART_LSR_TEMT     ((uint8_t)(1<<6))  /*!<Line status register: Transmitter empty*/
#define UART_LSR_RXFE     ((uint8_t)(1<<7))  /*!<Error in RX FIFO*/
#define UART_LSR_BITMASK  ((uint8_t)(0xFF))  /*!<UART Line status bit mask */


/* Macro defines for UART Modem (UART1 only) status register */
#define UART1_MSR_DELTA_CTS  ((uint8_t)(1<<0))  /*!< Set upon state change of input CTS */
#define UART1_MSR_DELTA_DSR  ((uint8_t)(1<<1))  /*!< Set upon state change of input DSR */
#define UART1_MSR_LO2HI_RI   ((uint8_t)(1<<2))  /*!< Set upon low to high transition of input RI */
#define UART1_MSR_DELTA_DCD  ((uint8_t)(1<<3))  /*!< Set upon state change of input DCD */
#define UART1_MSR_CTS        ((uint8_t)(1<<4))  /*!< Clear To Send State */
#define UART1_MSR_DSR        ((uint8_t)(1<<5))  /*!< Data Set Ready State */
#define UART1_MSR_RI         ((uint8_t)(1<<6))  /*!< Ring Indicator State */
#define UART1_MSR_DCD        ((uint8_t)(1<<7))  /*!< Data Carrier Detect State */
#define UART1_MSR_BITMASK    ((uint8_t)(0xFF))  /*!< MSR register bit-mask value */


/* Macro defines for UART Scratch Pad Register */
#define UART_SCR_BIMASK      ((uint8_t)(0xFF))  /*!< UART Scratch Pad bit mask */

/* Macro defines for UART Auto baudrate control register */
#define UART_ACR_START         ((uint32_t)(1<<0))   /**< UART Auto-baud start */
#define UART_ACR_MODE          ((uint32_t)(1<<1))   /**< UART Auto baudrate Mode 1 */
#define UART_ACR_AUTO_RESTART  ((uint32_t)(1<<2))   /**< UART Auto baudrate restart */
#define UART_ACR_ABEOINT_CLR   ((uint32_t)(1<<8))   /**< UART End of auto-baud interrupt clear */
#define UART_ACR_ABTOINT_CLR   ((uint32_t)(1<<9))   /**< UART Auto-baud time-out interrupt clear */
#define UART_ACR_BITMASK       ((uint32_t)(0x307))  /**< UART Auto Baudrate register bit mask */

/* UART IrDA control register defines */
#define UART_ICR_IRDAEN       ((uint32_t)(1<<0))         /**< IrDA mode enable */
#define UART_ICR_IRDAINV      ((uint32_t)(1<<1))         /**< IrDA serial input inverted */
#define UART_ICR_FIXPULSE_EN  ((uint32_t)(1<<2))         /**< IrDA fixed pulse width mode */
#define UART_ICR_PULSEDIV(n)  ((uint32_t)((n&0x07)<<3))  /**< PulseDiv - Configures the pulse when FixPulseEn = 1 */
#define UART_ICR_BITMASK      ((uint32_t)(0x3F))         /*!< UART IRDA bit mask */

/* Macro defines for UART Fractional divider register */
#define UART_FDR_DIVADDVAL(n)  ((uint32_t)(n&0x0F))       /**< Baud-rate generation pre-scaler divisor */
#define UART_FDR_MULVAL(n)     ((uint32_t)((n<<4)&0xF0))  /**< Baud-rate pre-scaler multiplier value */
#define UART_FDR_BITMASK       ((uint32_t)(0xFF))         /**< UART Fractional Divider register bit mask */

/* Macro defines for UART Tx Enable register */
#define UART_TER_TXEN      ((uint8_t)(1<<7))  /*!< Transmit enable bit */
#define UART_TER_BITMASK   ((uint8_t)(0x80))  /**< UART Transmit Enable Register bit mask */


/* Macro defines for UART1 RS485 Control register */
#define UART1_RS485CTRL_NMM_EN    ((uint32_t)(1<<0))  /*!< RS-485/EIA-485 Normal Multi-drop Mode (NMM)
                                                           is disabled */
#define UART1_RS485CTRL_RX_DIS    ((uint32_t)(1<<1))  /*!< The receiver is disabled */
#define UART1_RS485CTRL_AADEN     ((uint32_t)(1<<2))  /*!< Auto Address Detect (AAD) is enabled */
#define UART1_RS485CTRL_SEL_DTR   ((uint32_t)(1<<3))  /*!< If direction control is enabled
                                                           (bit DCTRL = 1), pin DTR is used for direction control */
#define UART1_RS485CTRL_DCTRL_EN  ((uint32_t)(1<<4))  /*!< Enable Auto Direction Control */
#define UART1_RS485CTRL_OINV_1	  ((uint32_t)(1<<5))  /*!< This bit reverses the polarity of the direction
                                                           control signal on the RTS (or DTR) pin. The direction control pin
                                                           will be driven to logic "1" when the transmitter has data to be sent */
#define UART1_RS485CTRL_BITMASK   ((uint32_t)(0x3F)) /*!< RS485 control bit-mask value */


/* Macro defines for UART1 RS-485 Address Match register */
#define UART1_RS485ADRMATCH_BITMASK  ((uint8_t)(0xFF))  /*!<Bit mask value */

/* Macro defines for UART1 RS-485 Delay value register */
#define UART1_RS485DLY_BITMASK       ((uint8_t)(0xFF))  /*!< Bit mask value */


/* Macro defines for UART FIFO Level register */
#define UART_FIFOLVL_RXFIFOLVL(n)  ((uint32_t)(n&0x0F))       /*!< Reflects the current level of the UART receiver FIFO */
#define UART_FIFOLVL_TXFIFOLVL(n)  ((uint32_t)((n>>8)&0x0F))  /*!< Reflects the current level of the UART transmitter FIFO */
#define UART_FIFOLVL_BITMASK       ((uint32_t)(0x0F0F))       /*!< UART FIFO Level Register bit mask */

/** UART 0 power/clock control bit */
#define	 CLKPWR_PCONP_PCUART0  	((uint32_t)(1<<3))
/** UART 1 power/clock control bit */
#define	 CLKPWR_PCONP_PCUART1  	((uint32_t)(1<<4))
/** UART 2 power/clock control bit */
#define	 CLKPWR_PCONP_PCUART2  	((uint32_t)(1<<24))
/** UART 3 power/clock control bit */
#define	 CLKPWR_PCONP_PCUART3  	((uint32_t)(1<<25))

/** Peripheral clock divider bit position for UART0 */
#define	CLKPWR_PCLKSEL0_UART0   ((uint32_t)(6))
/** Peripheral clock divider bit position for UART1 */
#define	CLKPWR_PCLKSEL0_UART1   ((uint32_t)(8))
/** Peripheral clock divider bit position for UART2 */
#define	CLKPWR_PCLKSEL1_UART2   ((uint32_t)(16))
/** Peripheral clock divider bit position for UART3 */
#define	CLKPWR_PCLKSEL1_UART3   ((uint32_t)(18))

/***************************************************************************//**
 * @brief     Configurate the baudrate
 * @param[in] UARTx : UART peripheral selected, should be 
 *                    UART0, UART1, UART2, UART3
 * @param[in] rate  : Baudrate 
 * @return    Previous baudrate
*******************************************************************************/
static uint32_t LPC_UART_SetRate(UART_TypeDef *UARTx, uint32_t baudrate)
{
  uint32_t uClk;
  uint32_t calcBaudrate = 0;
  uint32_t temp = 0;
  uint32_t rate_bak;

  uint32_t mulFracDiv, dividerAddFracDiv;
  uint32_t diviser = 0 ;
  uint32_t mulFracDivOptimal = 1;
  uint32_t dividerAddOptimal = 0;
  uint32_t diviserOptimal = 0;

  uint32_t relativeError = 0;
  uint32_t relativeOptimalError = 100000;

  /* get UART block clock */
  if (UARTx == UART0) {
    uClk = LPC_PCLK(LPC_PCLK_UART0);
  }
  else if (UARTx == (UART_TypeDef *)UART1) {
    uClk = LPC_PCLK(LPC_PCLK_UART1);
  }
  else if (UARTx == UART2) {
    uClk = LPC_PCLK(LPC_PCLK_UART2);
  }
  else if (UARTx == UART3) {
    uClk = LPC_PCLK(LPC_PCLK_UART3);
  } else {
    return 0;
  }
  
  uClk = uClk >> 4; /* div by 16 */

  rate_bak = 0;

  UARTx->LCR |= UART_LCR_DLAB_EN;
            
  if(((UARTx->FDR >> 4) & 0xF) !=0 ) {
    rate_bak = 256 * UARTx->DLM + UARTx->DLL;
    rate_bak = (1 + (UARTx->FDR & 0xF)/((UARTx->FDR >> 4) & 0xF));   
    if((((256 * UARTx->DLM + UARTx->DLL) * (1 + (UARTx->FDR & 0xF)/((UARTx->FDR >> 4) & 0xF)))) != 0) {  
      rate_bak = uClk / ((256 * UARTx->DLM + UARTx->DLL) * (1 + ((UARTx->FDR & 0xF) * 1.0)/((UARTx->FDR >> 4) & 0xF)));
    }
  } 

  UARTx->LCR &= (~UART_LCR_DLAB_EN) & UART_LCR_BITMASK;
    
  /* In the Uart IP block, baud rate is calculated using FDR and DLL-DLM registers
  * The formula is :
  * BaudRate= uClk * (mulFracDiv/(mulFracDiv+dividerAddFracDiv) / (16 * (DLL)
  * It involves floating point calculations. That's the reason the formulae are adjusted with
  * Multiply and divide method.*/
  /* The value of mulFracDiv and dividerAddFracDiv should comply to the following expressions:
  * 0 < mulFracDiv <= 15, 0 <= dividerAddFracDiv <= 15 */
  for (mulFracDiv = 1 ; mulFracDiv <= 15 ;mulFracDiv++) {
  for (dividerAddFracDiv = 0 ; dividerAddFracDiv <= 15 ;dividerAddFracDiv++) {
  temp = (mulFracDiv * uClk) / ((mulFracDiv + dividerAddFracDiv));

  diviser = temp / baudrate;
  if ((temp % baudrate) > (baudrate / 2)) {
    diviser++;
  }  		

  if (diviser > 2 && diviser < 65536) {
    calcBaudrate = temp / diviser;
    
    if (calcBaudrate <= baudrate) {
      relativeError = baudrate - calcBaudrate;      
    }  		  
    else {
      relativeError = calcBaudrate - baudrate;
    }
    
    if ((relativeError < relativeOptimalError)) {
      mulFracDivOptimal = mulFracDiv ;
      dividerAddOptimal = dividerAddFracDiv;
      diviserOptimal = diviser;
      relativeOptimalError = relativeError;
      if (relativeError == 0) break;
    }
   } /* End of if */
  } /* end of inner for loop */
  if (relativeError == 0) break;
  } /* end of outer for loop  */

  if (relativeOptimalError < ((baudrate * UART_ACCEPTED_BAUDRATE_ERROR)/100)) {
    if (((UART1_TypeDef *)UARTx) == UART1) {
      ((UART1_TypeDef *)UARTx)->LCR |= UART_LCR_DLAB_EN;
      ((UART1_TypeDef *)UARTx)->DLM = UART_LOAD_DLM(diviserOptimal);
      ((UART1_TypeDef *)UARTx)->DLL = UART_LOAD_DLL(diviserOptimal);
      /* Then reset DLAB bit */
      ((UART1_TypeDef *)UARTx)->LCR &= (~UART_LCR_DLAB_EN) & UART_LCR_BITMASK;
      ((UART1_TypeDef *)UARTx)->FDR = (UART_FDR_MULVAL(mulFracDivOptimal) \
        | UART_FDR_DIVADDVAL(dividerAddOptimal)) & UART_FDR_BITMASK;
    }
    else {
      UARTx->LCR |= UART_LCR_DLAB_EN;
      UARTx->DLM = UART_LOAD_DLM(diviserOptimal);
      UARTx->DLL = UART_LOAD_DLL(diviserOptimal);
      /* Then reset DLAB bit */
      UARTx->LCR &= (~UART_LCR_DLAB_EN) & UART_LCR_BITMASK;
      UARTx->FDR = (UART_FDR_MULVAL(mulFracDivOptimal) \
                   | UART_FDR_DIVADDVAL(dividerAddOptimal)) & UART_FDR_BITMASK;
    }
  }

  return rate_bak;
}

/***************************************************************************//**
 * @brief     Initializes the UARTx peripheral
 * @param[in] UARTx : UART peripheral selected, should be 
 *                    UART0, UART1, UART2, UART3
 * @param[in] rate  : Baudrate 
 * @return    COX_SUCCESS or COX_ERROR
*******************************************************************************/
static COX_Status LPC_UART_Init (UART_TypeDef *UARTx, uint32_t baudrate)
{
  uint32_t tmp;

  if(UARTx == UART0) {
    /* Set up clock and power for UART module */
    SC->PCONP |= CLKPWR_PCONP_PCUART0;

    /* Set P0.2 and P0.3 function as TXD0, RXD0 */        
    LPC_Pin_Func(0, 2, PIN_CFG_FUNC_1);
    LPC_Pin_Mode(0, 2, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(0, 2, PIN_CFG_OD_NORMAL);

    LPC_Pin_Func(0, 3, PIN_CFG_FUNC_1);
    LPC_Pin_Mode(0, 3, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(0, 3, PIN_CFG_OD_NORMAL);       
  }

  else if(((UART1_TypeDef *)UARTx) == UART1) {
    /* Set up clock and power for UART module */
    SC->PCONP |= CLKPWR_PCONP_PCUART1;    

#ifdef LPC_SERIAL1_P015_P016   
    /* Set P0.15. P0.16 function as TXD1 and RXD1 */
    LPC_Pin_Func(0, 15, PIN_CFG_FUNC_1);
    LPC_Pin_Mode(0, 15, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(0, 15, PIN_CFG_OD_NORMAL);

    LPC_Pin_Func(0, 16, PIN_CFG_FUNC_1);
    LPC_Pin_Mode(0, 16, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(0, 16, PIN_CFG_OD_NORMAL); 
#endif    

#ifdef LPC_SERIAL1_P200_P201   
    /* Set P2.0, P2.1 function as TXD1 and RXD1 */ 
    LPC_Pin_Func(2, 0, PIN_CFG_FUNC_2);
    LPC_Pin_Mode(2,0, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(2, 0, PIN_CFG_OD_NORMAL);

    LPC_Pin_Func(2, 1, PIN_CFG_FUNC_2);
    LPC_Pin_Mode(2,1, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(2, 1, PIN_CFG_OD_NORMAL);
#endif
  }  
  else if(UARTx == UART2) {
    /* Set up clock and power for UART module */
    SC->PCONP |= CLKPWR_PCONP_PCUART2;

#ifdef LPC_SERIAL2_P010_P011
    /* Set P0.10 and P0.11 function as TXD2, RXD2 */
    LPC_Pin_Func(0, 10, PIN_CFG_FUNC_1);
    LPC_Pin_Mode(0,10, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(0, 10, PIN_CFG_OD_NORMAL);

    LPC_Pin_Func(0, 11, PIN_CFG_FUNC_1);
    LPC_Pin_Mode(0,11, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(0, 11, PIN_CFG_OD_NORMAL);
#endif

#ifdef LPC_SERIAL2_P208_P209    
    /* Set P2.8 and P2.9 function as TXD2, RXD2 */
    LPC_Pin_Func(2, 8, PIN_CFG_FUNC_2);
    LPC_Pin_Mode(2, 8, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(2, 8, PIN_CFG_OD_NORMAL);

    LPC_Pin_Func(2, 9, PIN_CFG_FUNC_2);
    LPC_Pin_Mode(2, 9, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(2, 9, PIN_CFG_OD_NORMAL);
#endif
  }
  else if(UARTx == UART3) {
    /* Set up clock and power for UART module */
    SC->PCONP |= CLKPWR_PCONP_PCUART3;

#ifdef LPC_SERIAL3_P000_P001
    /* Set P0.0 and P0.1 function as TXD3, RXD3 */
    LPC_Pin_Func(0, 0, PIN_CFG_FUNC_2);
    LPC_Pin_Mode(0, 0, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(0, 0, PIN_CFG_OD_NORMAL);

    LPC_Pin_Func(0, 1, PIN_CFG_FUNC_2);
    LPC_Pin_Mode(0, 1, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(0, 1, PIN_CFG_OD_NORMAL);
#endif

#ifdef LPC_SERIAL3_P428_P429    
    /* Set P4.28 and P4.29 function as TXD3, RXD3 */
    LPC_Pin_Func(4, 28, PIN_CFG_FUNC_3);
    LPC_Pin_Mode(4, 28, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(4, 28, PIN_CFG_OD_NORMAL);

    LPC_Pin_Func(4, 29, PIN_CFG_FUNC_3);
    LPC_Pin_Mode(4, 29, PIN_CFG_MODE_PULLUP);
    LPC_Pin_OD(4, 29, PIN_CFG_OD_NORMAL);
    
#endif    
  } else {
    return COX_ERROR;
  }

  if (((UART1_TypeDef *)UARTx) == UART1) {
    /* FIFOs are empty */
    ((UART1_TypeDef *)UARTx)->FCR = ( UART_FCR_FIFO_EN \
    		| UART_FCR_RX_RS | UART_FCR_TX_RS);
    // Disable FIFO
    ((UART1_TypeDef *)UARTx)->FCR = 0;
    
    // Dummy reading
    while (((UART1_TypeDef *)UARTx)->LSR & UART_LSR_RDR) {
      tmp = ((UART1_TypeDef *)UARTx)->/*RBTHDLR.*/RBR;
    }
    
    ((UART1_TypeDef *)UARTx)->TER = UART_TER_TXEN;
    // Wait for current transmit complete
    while (!(((UART1_TypeDef *)UARTx)->LSR & UART_LSR_THRE));
    // Disable Tx
    ((UART1_TypeDef *)UARTx)->TER = 0;
    
    // Disable interrupt
    ((UART1_TypeDef *)UARTx)->/*DLIER.*/IER = 0;
    // Set LCR to default state
    ((UART1_TypeDef *)UARTx)->LCR = 0;
    // Set ACR to default state
    ((UART1_TypeDef *)UARTx)->ACR = 0;
    // Set Modem Control to default state
    ((UART1_TypeDef *)UARTx)->MCR = 0;
    // Set RS485 control to default state
    ((UART1_TypeDef *)UARTx)->RS485CTRL = 0;
    // Set RS485 delay timer to default state
    ((UART1_TypeDef *)UARTx)->RS485DLY = 0;
    // Set RS485 addr match to default state
    ((UART1_TypeDef *)UARTx)->ADRMATCH = 0;
    //Dummy Reading to Clear Status
    tmp = ((UART1_TypeDef *)UARTx)->MSR;
    tmp = ((UART1_TypeDef *)UARTx)->LSR;
  }
  
  else {
    /* FIFOs are empty */
    UARTx->FCR = ( UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS);
    // Disable FIFO
    UARTx->FCR = 0;
    
    // Dummy reading
    while (UARTx->LSR & UART_LSR_RDR) {
      tmp = UARTx->RBR;
    }
    
    UARTx->TER = UART_TER_TXEN;
    // Wait for current transmit complete
    while (!(UARTx->LSR & UART_LSR_THRE));
    // Disable Tx
    UARTx->TER = 0;    
    // Disable interrupt
    UARTx->IER = 0;
    // Set LCR to default state
    UARTx->LCR = 0;
    // Set ACR to default state
    UARTx->ACR = 0;
    // Dummy reading
    tmp = UARTx->LSR;
  }
  
  if (UARTx == UART3) {
    // Set IrDA to default state
    UARTx->ICR = 0;
  }
  
  // Set Line Control register ----------------------------  
  LPC_UART_SetRate(UARTx, baudrate);
  
  if (((UART1_TypeDef *)UARTx) == UART1) {
    tmp = (((UART1_TypeDef *)UARTx)->LCR & (UART_LCR_DLAB_EN | UART_LCR_BREAK_EN)) \
          & UART_LCR_BITMASK;
  }  
  else {
    tmp = (UARTx->LCR & (UART_LCR_DLAB_EN | UART_LCR_BREAK_EN)) & UART_LCR_BITMASK;
  }
  
  tmp |= UART_LCR_WLEN8; 
  
  // Write back to LCR, configure FIFO and Disable Tx
  if (((UART1_TypeDef *)UARTx) ==  UART1) {
    ((UART1_TypeDef *)UARTx)->LCR = (uint8_t)(tmp & UART_LCR_BITMASK);
  } else {
    UARTx->LCR = (uint8_t)(tmp & UART_LCR_BITMASK);
  }

  // Enable Transmit
  if (((UART1_TypeDef *)UARTx) == UART1) {
    ((UART1_TypeDef *)UARTx)->TER |= UART_TER_TXEN;
  } else {
    UARTx->TER |= UART_TER_TXEN;
  }

  return COX_SUCCESS;
}

/***************************************************************************//**
 * @brief     Send a block of data via UART peripheral
 * @param[in] UARTx : UART peripheral selected, should be 
 *                    UART0, UART1, UART2, UART3
 * @param[in] wbuf  : Pointer to Transmit buffer 
 * @param[in] wlen  : Length of transmit buffer 
 * @return    Number of bytes sent
*******************************************************************************/      
static uint32_t LPC_UART_Write (UART_TypeDef *UARTx, const void* wbuf, uint32_t wlen)
{
  uint32_t i;
  const uint8_t *tx_buf = wbuf;

  for(i=0; i<wlen; i++) {

    /* Wait for THR empty */
    while (!(UARTx->LSR & UART_LSR_THRE));  
    UARTx->THR = tx_buf[i];  
  }
  
  return wlen;  
}

/***************************************************************************//**
 * @brief     Receive a block of data via UART peripheral
 * @param[in] UARTx : UART peripheral selected, should be 
 *                    UART0, UART1, UART2, UART3
 * @param[in] wbuf  : Pointer to receive buffer 
 * @param[in] wlen  : Length of receive buffer 
 * @return    Number of bytes received
*******************************************************************************/
static uint32_t LPC_UART_Read (UART_TypeDef *UARTx, void *rbuf, uint32_t rlen)
{
  uint32_t i;
  uint8_t *rx_buf = rbuf;  
  
  for(i=0; i<rlen; i++) {
    
    while (!(UARTx->LSR & UART_LSR_RDR));
    // Get data from the buffer
    if (((UART1_TypeDef *)UARTx) == UART1) {
      rx_buf[i] = ((UART1_TypeDef *)UARTx)->RBR & 0xFF;
    } else {
      rx_buf[i] = UARTx->RBR & 0xFF;
    }
  }
  
  return rlen;  
}

/***************************************************************************//**
 * @brief     Configure the UARTx peripheral
 * @param[in] UARTx : UART peripheral selected, should be 
 *                    UART0, UART1, UART2, UART3
 * @param[in] index : Configuration index, it should be :
 *                      -COX_SERIAL_CFG_RATE : Set baudrate
 *                      -COX_SERIAL_CFG_BITS : Number of data bits, arg should be
 *                         @5 : 5 bit data mode
 *                         @6 : 5 bit data mode
 *                         @7 : 5 bit data mode
 *                         @8 : 5 bit data mode (default)
 *                      -COX_SERIAL_CFG_STOP_BITS : Number of stop bits, arg should be 
 *                         @COX_SERIAL_STOPBIT_1 : 1 Stop Bits (default)
 *                         @COX_SERIAL_STOPBIT_2 : 1 Stop Bits
 *                      -COX_SERIAL_CFG_PARITY : Parity selection, arg should be
 *                         @COX_SERIAL_PARITY_NONE : No parity (default)
 *                         @COX_SERIAL_PARITY_ODD  : Odd parity
 *                         @COX_SERIAL_PARITY_EVEN : Even parity
 *                         @COX_SERIAL_PARITY_SP_1 : Forced "1" stick parity
 *                         @COX_SERIAL_PARITY_SP_0 : Forced "0" stick parity 
 * @param[in] arg : Configuration argument
 * @param[in] pre_arg : Return previous configuration
 * @return    COX_SUCCESS or COX_ERROR
*******************************************************************************/
static COX_Status LPC_UART_Cfg (UART_TypeDef *UARTx, uint8_t index, uint32_t arg, uint32_t *pre_arg)
{
  COX_Status err = COX_ERROR;
  uint32_t arg_bak;

  switch(index) {
  /*------------------------------------------*/
  /* Baudrate */
  case COX_SERIAL_CFG_RATE:
    arg_bak = LPC_UART_SetRate(UARTx, arg);
    err = COX_SUCCESS; break;

  /*------------------------------------------*/
  /* Data format */
  case COX_SERIAL_CFG_BITS:
    switch(UARTx->LCR & 3) {
    case 0: arg_bak = 5; break;
    case 1: arg_bak = 6; break;
    case 2: arg_bak = 7; break;
    case 3: arg_bak = 8; break;
    }

    switch(arg) {
    case 5:
      UARTx->LCR &= ~((uint32_t)3<<0);
      UARTx->LCR |=  ((uint32_t)0<<0);
      err = COX_SUCCESS; break;
    case 6:
      UARTx->LCR &= ~((uint32_t)3<<0);
      UARTx->LCR |=  ((uint32_t)1<<0);
      err = COX_SUCCESS; break;
    case 7:
      UARTx->LCR &= ~((uint32_t)3<<0);
      UARTx->LCR |=  ((uint32_t)2<<0);
      err = COX_SUCCESS; break;
    case 8:
      UARTx->LCR &= ~((uint32_t)3<<0);
      UARTx->LCR |=  ((uint32_t)3<<0);
      err = COX_SUCCESS; break;
    default : break;
    };
    break;
  /*------------------------------------------*/
  /* Stop bits */
  case COX_SERIAL_CFG_STOP_BITS:
    if((UARTx->LCR >> 2) & 1) {
      arg_bak = COX_SERIAL_STOPBIT_2;
    } else {
      arg_bak = COX_SERIAL_STOPBIT_1;    
    }

    switch(arg) {
    case COX_SERIAL_STOPBIT_1:
      UARTx->LCR &= ~((uint32_t)1 << 2);
      err = COX_SUCCESS; break;
    case COX_SERIAL_STOPBIT_2:     
      UARTx->LCR |=  ((uint32_t)1 << 2);
      err = COX_SUCCESS; break;
    }
    break;
    
  /*------------------------------------------*/
  /* Parity select */
  case COX_SERIAL_CFG_PARITY:
    if((UARTx->LCR >> 3) & 1) { /* Parity Enable Bit */
      switch((UARTx->LCR >> 4) & 3) { /* Parity Select Bits */
      case 0: arg_bak = COX_SERIAL_PARITY_ODD;  break;
      case 1: arg_bak = COX_SERIAL_PARITY_EVEN; break;
      case 2: arg_bak = COX_SERIAL_PARITY_SP_1; break;
      case 3: arg_bak = COX_SERIAL_PARITY_SP_0; break;
      };
    } else {
      arg_bak = COX_SERIAL_PARITY_NONE;
    }

    switch(arg) {
    case COX_SERIAL_PARITY_NONE:
      UARTx->LCR &= ~((uint32_t)1 << 3);
      err = COX_SUCCESS; break;
    case COX_SERIAL_PARITY_ODD :
      UARTx->LCR |=  ((uint32_t)1 << 3);
      UARTx->LCR &= ~((uint32_t)3 << 4);
      UARTx->LCR |=  ((uint32_t)0 << 4);
      err = COX_SUCCESS; break;
    case COX_SERIAL_PARITY_EVEN:
      UARTx->LCR |=  ((uint32_t)1 << 3);
      UARTx->LCR &= ~((uint32_t)3 << 4);
      UARTx->LCR |=  ((uint32_t)1 << 4);
      err = COX_SUCCESS; break;
    case COX_SERIAL_PARITY_SP_1:
      UARTx->LCR |=  ((uint32_t)1 << 3);
      UARTx->LCR &= ~((uint32_t)3 << 4);
      UARTx->LCR |=  ((uint32_t)2 << 4);
      err = COX_SUCCESS; break;
    case COX_SERIAL_PARITY_SP_0:
      UARTx->LCR |=  ((uint32_t)1 << 3);
      UARTx->LCR &= ~((uint32_t)3 << 4);
      UARTx->LCR |=  ((uint32_t)3 << 4);
      err = COX_SUCCESS; break;
    default: break;
    }
    break;
        
    /*------------------------------------------*/       
    default: break;
  };

  /* Return previous configuration argument */
  if((err == COX_SUCCESS) && (pre_arg != COX_NULL)) {
    *pre_arg = arg_bak;    
  }    
  return err;
}

static COX_Status LPC_Serial0_Init (uint32_t baudrate) { return LPC_UART_Init((UART_TypeDef *)UART0, baudrate); }
static COX_Status LPC_Serial1_Init (uint32_t baudrate) { return LPC_UART_Init((UART_TypeDef *)UART1, baudrate); }
static COX_Status LPC_Serial2_Init (uint32_t baudrate) { return LPC_UART_Init((UART_TypeDef *)UART2, baudrate); }
static COX_Status LPC_Serial3_Init (uint32_t baudrate) { return LPC_UART_Init((UART_TypeDef *)UART3, baudrate); }

static uint32_t LPC_Serial0_Write (const void* wbuf, uint32_t wlen) { return LPC_UART_Write ((UART_TypeDef *)UART0, wbuf, wlen); }
static uint32_t LPC_Serial1_Write (const void* wbuf, uint32_t wlen) { return LPC_UART_Write ((UART_TypeDef *)UART1, wbuf, wlen); }
static uint32_t LPC_Serial2_Write (const void* wbuf, uint32_t wlen) { return LPC_UART_Write ((UART_TypeDef *)UART2, wbuf, wlen); }
static uint32_t LPC_Serial3_Write (const void* wbuf, uint32_t wlen) { return LPC_UART_Write ((UART_TypeDef *)UART3, wbuf, wlen); }

static uint32_t LPC_Serial0_Read (void *rbuf, uint32_t rlen) { return LPC_UART_Read ((UART_TypeDef *)UART0, rbuf, rlen); }
static uint32_t LPC_Serial1_Read (void *rbuf, uint32_t rlen) { return LPC_UART_Read ((UART_TypeDef *)UART1, rbuf, rlen); }
static uint32_t LPC_Serial2_Read (void *rbuf, uint32_t rlen) { return LPC_UART_Read ((UART_TypeDef *)UART2, rbuf, rlen); }
static uint32_t LPC_Serial3_Read (void *rbuf, uint32_t rlen) { return LPC_UART_Read ((UART_TypeDef *)UART3, rbuf, rlen); }

static COX_Status LPC_Serial0_Cfg (uint8_t index, uint32_t arg, uint32_t *pre_arg) { return LPC_UART_Cfg ((UART_TypeDef *)UART0, index, arg, pre_arg); }
static COX_Status LPC_Serial1_Cfg (uint8_t index, uint32_t arg, uint32_t *pre_arg) { return LPC_UART_Cfg ((UART_TypeDef *)UART1, index, arg, pre_arg); }
static COX_Status LPC_Serial2_Cfg (uint8_t index, uint32_t arg, uint32_t *pre_arg) { return LPC_UART_Cfg ((UART_TypeDef *)UART2, index, arg, pre_arg); }
static COX_Status LPC_Serial3_Cfg (uint8_t index, uint32_t arg, uint32_t *pre_arg) { return LPC_UART_Cfg ((UART_TypeDef *)UART3, index, arg, pre_arg); }


/***************************************************************************//**
 * Define LPC17xx COX Serial Interface based on UART0
*******************************************************************************/
COX_SERIAL_PI pi_serial0 = {
  LPC_Serial0_Init,
  LPC_Serial0_Write,
  LPC_Serial0_Read,
  LPC_Serial0_Cfg,
};

/***************************************************************************//**
 * Define LPC17xx COX Serial Interface based on UART1
*******************************************************************************/
COX_SERIAL_PI pi_serial1 = {
  LPC_Serial1_Init,
  LPC_Serial1_Write,
  LPC_Serial1_Read,
  LPC_Serial1_Cfg,
};

/*******************************************************************************
 * Define LPC17xx COX Serial Interface based on UART2
*******************************************************************************/
COX_SERIAL_PI pi_serial2 = {
  LPC_Serial2_Init,
  LPC_Serial2_Write,
  LPC_Serial2_Read,
  LPC_Serial2_Cfg,
};

/*******************************************************************************
 * Define LPC17xx COX Serial Interface based on UART3
*******************************************************************************/
COX_SERIAL_PI pi_serial3 = {
  LPC_Serial3_Init,
  LPC_Serial3_Write,
  LPC_Serial3_Read,
  LPC_Serial3_Cfg,
};
