#ifndef UART_H
#define UART_H

/* Include LPC Libraries */
#include <lpc17xx_uart.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_uart.h>
#include <driver/util.h>
#include <driver/Motor.h>
#include <cmsis/core_cm3.h>
#include <Interfaces/Actuators/Actuators.h>

extern uint8_t bStarted;

void UARTInit(LPC_UART_TypeDef *UARTx, uint32_t uiBaudrate);
void UARTSetMisc(LPC_UART_TypeDef *UARTx);
void UARTSend(LPC_UART_TypeDef *UARTx , const char * sendBuffer);

#endif
