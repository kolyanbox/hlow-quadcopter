/***************************************************************************//**
 * @file      : lpc_serial.h
 * @brief     : LPC17xx Serial Peripheral Interface
 * @version   : V1.0
 * @date      : 8. Jul. 2010
 * @author    : CooCox
*******************************************************************************/
#ifndef __LPC_SERIAL_H                                                        
#define __LPC_SERIAL_H

#include "cox_serial.h"
/**
 * Pin assignment                                                             \n
 *   +--------------+----------------+---------------+                        \n
 *   |   Interface  |       TXD      |      RXD      |                        \n
 *   +--------------+----------------+---------------+                        \n
 *   |    SERIAL0   |  P0.2          |  P0.3         |                        \n
 *   +--------------+----------------+---------------+                        \n
 *   |    SERIAL1   |  P2.0, P0.15   |  P2.1, P0.16  |                        \n
 *   +--------------+----------------+---------------+                        \n
 *   |    SERIAL2   |  P0.10, P2.8   |  P0.11, P2.9  |                        \n
 *   +--------------+----------------+---------------+                        \n
 *   |    SERIAL3   |  P0.0, P4.28   |  P0.1, P4.29  |                        \n
 *   +--------------+----------------+---------------+                        \n
 *                                                                            \n
 * Configuration Support:                                                     \n
 *   COX_SERIAL_CFG_RATE : Set baudrate                                       \n
 *   COX_SERIAL_CFG_BITS : Number of data bits, should be                     \n
 *                          -5 : 5 bit data mode                              \n
 *                          -6 : 6 bit data mode                              \n
 *                          -7 : 7 bit data mode                              \n
 *                          -8 : 8 bit data mode (default)                    \n
 *   COX_SERIAL_CFG_STOP_BITS : Number of stop bits, should be                \n
 *                          -COX_SERIAL_STOPBIT_1 : 1 Stop Bits (default)     \n
 *                          -COX_SERIAL_STOPBIT_2 : 1 Stop Bits               \n
 *   COX_SERIAL_CFG_PARITY : Parity selection, should be                      \n
 *                          -COX_SERIAL_PARITY_NONE : No parity (default)     \n
 *                          -COX_SERIAL_PARITY_ODD  : Odd parity              \n
 *                          -COX_SERIAL_PARITY_EVEN : Even parity             \n
 *                          -COX_SERIAL_PARITY_SP_1 : Forced "1" stick parity \n
 *                          -COX_SERIAL_PARITY_SP_0 : Forced "0" stick parity \n
 *                                                                            \n
 * Default Configuration :                                                    \n
 *   8 bit data mode, COX_SERIAL_STOPBIT_1, COX_SERIAL_PARITY_NONE            \n
*******************************************************************************/
#define LPC_DEV_SERIAL0     { COX_PIN(0,  2), COX_PIN(0,  3) }

#define LPC_DEV_SERIAL1_0   { COX_PIN(0, 15), COX_PIN(0, 16) }
#define LPC_DEV_SERIAL1_1   { COX_PIN(2,  0), COX_PIN(2,  1) }

#define LPC_DEV_SERIAL2_0   { COX_PIN(0, 10), COX_PIN(0, 11) }
#define LPC_DEV_SERIAL2_1   { COX_PIN(2,  8), COX_PIN(2,  9) }

#define LPC_DEV_SERIAL3_0   { COX_PIN(0,  0), COX_PIN(0,  1) }
#define LPC_DEV_SERIAL3_1   { COX_PIN(4, 28), COX_PIN(4, 29) }


/** Define LPC17xx COX Serial Interface based on UART0
*******************************************************************************/
extern COX_SERIAL_PI pi_serial0;

/** Define LPC17xx COX Serial Interface based on UART1
*******************************************************************************/
#define LPC_SERIAL1_P200_P201      /* Assign TXD1 and RXD1 to P2.0  and P2.1  */
//#define LPC_SERIAL1_P015_P016    /* Assign TXD1 and RXD1 to P0.15 and P0.16 */
extern COX_SERIAL_PI pi_serial1;

/** Define LPC17xx COX Serial Interface based on UART2
*******************************************************************************/
//#define LPC_SERIAL2_P010_P011    /* Assign TXD2 and RXD2 to P0.10 and P0.11 */
#define LPC_SERIAL2_P208_P209      /* Assign TXD2 and RXD2 to P2.8  and P2.9  */ 
extern COX_SERIAL_PI pi_serial2;


/** Define LPC17xx COX Serial Interface based on UART3
*******************************************************************************/
//#define LPC_SERIAL3_P000_P001    /* Assign TXD3 and RXD3 to P0.0  and P0.1  */
#define LPC_SERIAL3_P428_P429      /* Assign TXD3 and RXD3 to P4.28 and P4.29 */
extern COX_SERIAL_PI pi_serial3;


#endif
