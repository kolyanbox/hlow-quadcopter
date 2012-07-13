#ifndef UART_H
#define UART_H

/* Include LPC Libraries */
#include <lpc17xx_uart.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_uart.h>
#include <General/util.h>
#include <cmsis/core_cm3.h>
#include <Interfaces/Actuators/Actuators.h>

extern uint8_t bStarted;

volatile unsigned char lastReceivedChar;

Bool UARTInit(LPC_UART_TypeDef *UARTx, uint32_t uiBaudrate);
void UARTSetMisc(LPC_UART_TypeDef *UARTx);
void UARTSend(LPC_UART_TypeDef *UARTx , const char * sendBuffer);
unsigned char* lastReceivedCommand();
void clearLastCommand();

#endif
