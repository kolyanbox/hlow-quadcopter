#ifndef UART_H
#define UART_H

/* Include LPC Libraries */
#include <lpc17xx_uart.h>
#include <lpc17xx_pinsel.h>
#include <cmsis/core_cm3.h>
#include <Global/Util/util.h>

void UARTInit(LPC_UART_TypeDef *UARTx, uint32_t uiBaudrate);
void UARTSetMisc(LPC_UART_TypeDef *UARTx);
void UARTSend(LPC_UART_TypeDef *UARTx , const char * sendBuffer);
void DEBUG_Init(uint32_t uiBaudrate);
void DEBUG_Send(const char * sendBuffer);

#endif
