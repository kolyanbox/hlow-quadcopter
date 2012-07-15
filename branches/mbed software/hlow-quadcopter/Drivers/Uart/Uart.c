#include <Drivers/Uart/UART.h>

unsigned char lastReceivedCommandString[COMMANDLENGTH];
int lastReceivedCommandPosition = 0;
Bool receivedValidCommand = FALSE;
Bool isUart0Initialized = FALSE;

Bool UARTInit(LPC_UART_TypeDef *UARTx, uint32_t uiBaudrate)
{
	/* UART Controller */
	UART_CFG_Type UART_ConfigStruct;
	UART_ConfigStruct.Baud_rate = uiBaudrate;
	UART_ConfigStruct.Databits = UART_DATABIT_8;
	UART_ConfigStruct.Parity = UART_PARITY_NONE;
	UART_ConfigStruct.Stopbits = UART_STOPBIT_1;
	UART_Init(UARTx, &UART_ConfigStruct);

	/* UART FIFO */
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	UARTFIFOConfigStruct.FIFO_DMAMode = DISABLE;
	UARTFIFOConfigStruct.FIFO_Level = UART_FIFO_TRGLEV0;
	UARTFIFOConfigStruct.FIFO_ResetRxBuf = ENABLE;
	UARTFIFOConfigStruct.FIFO_ResetTxBuf = ENABLE;
	UART_FIFOConfig(UARTx, &UARTFIFOConfigStruct);

	/* Set PINSEL and Set Interrupts */
	UARTSetMisc(UARTx);

	if (UARTx ==  LPC_UART0)
	{
		isUart0Initialized = TRUE;
	}
	return TRUE;
}

void UARTSetMisc(LPC_UART_TypeDef *UARTx)
{
	/* UART PINSEL */
	PINSEL_CFG_Type PinCfg;

	if(UARTx == LPC_UART0)
	{
		PinCfg.Funcnum = 1;
		PinCfg.OpenDrain = 0;
		PinCfg.Pinmode = 0;
		PinCfg.Pinnum = 2;
		PinCfg.Portnum = 0;
		PINSEL_ConfigPin(&PinCfg);
		PinCfg.Pinnum = 3;
		PINSEL_ConfigPin(&PinCfg);

		/* UART Enable Transmit */
		UART_TxCmd(LPC_UART0, ENABLE);

		/* UART3 Priority */
		NVIC_SetPriority(UART0_IRQn, ((0x01 << 3) | 0x01));

		/* UARTx Receive Interrupt Enable */
	    UART_IntConfig(LPC_UART0, UART_INTCFG_RBR, ENABLE);

	    /* Enable IRQ */
	    NVIC_EnableIRQ(UART0_IRQn);

	}
	else if((LPC_UART1_TypeDef *)UARTx == LPC_UART1)
	{
		PinCfg.Funcnum = 1;
		PinCfg.OpenDrain = 0;
		PinCfg.Pinmode = 0;
		PinCfg.Pinnum = 15;
		PinCfg.Portnum = 0;
		PINSEL_ConfigPin(&PinCfg);
		PinCfg.Pinnum = 16;
		PINSEL_ConfigPin(&PinCfg);

		/* UART Enable Transmit */
		UART_TxCmd((LPC_UART_TypeDef *)LPC_UART1, ENABLE);

		/* UART3 Priority */
		NVIC_SetPriority(UART1_IRQn, ((0x01 << 3) | 0x01));

		/* UARTx Receive Interrupt Enable */
	    UART_IntConfig(( LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RBR, ENABLE);

		/* Enable IRQ */
		NVIC_EnableIRQ(UART1_IRQn);
	}
	else if(UARTx == LPC_UART2)
	{
		PinCfg.Funcnum = 1;
		PinCfg.OpenDrain = 0;
		PinCfg.Pinmode = 0;
		PinCfg.Pinnum = 10;
		PinCfg.Portnum = 0;
		PINSEL_ConfigPin(&PinCfg);
		PinCfg.Pinnum = 11;
		PINSEL_ConfigPin(&PinCfg);

		/* UART Enable Transmit */
		UART_TxCmd(LPC_UART2, ENABLE);

		/* UART3 Priority */
		NVIC_SetPriority(UART2_IRQn, ((0x01 << 3) | 0x01));

		/* UARTx Receive Interrupt Enable */
	    UART_IntConfig(LPC_UART2, UART_INTCFG_RBR, ENABLE);

		/* Enable IRQ */
		NVIC_EnableIRQ(UART2_IRQn);
	}
	else if(UARTx == LPC_UART3)
	{
		PinCfg.Funcnum = 2;
		PinCfg.OpenDrain = 0;
		PinCfg.Pinmode = 0;
		PinCfg.Pinnum = 0;
		PinCfg.Portnum = 0;
		PINSEL_ConfigPin(&PinCfg);
		PinCfg.Pinnum = 1;
		PINSEL_ConfigPin(&PinCfg);

		/* UART Enable Transmit */
		UART_TxCmd(LPC_UART3, ENABLE);

		/* UART3 Priority */
		NVIC_SetPriority(UART3_IRQn, ((0x01 << 5) | 0x01));

		/* UARTx Receive Interrupt Enable */
	    UART_IntConfig(LPC_UART3, UART_INTCFG_RBR, ENABLE);

		/* Enable IRQ */
		NVIC_EnableIRQ(UART3_IRQn);
	}
	else
	{
		return;
	}

}

void UARTSend(LPC_UART_TypeDef *UARTx , const char * sendBuffer)
{
	UART_Send(UARTx, (uint8_t *)sendBuffer, Strlen(sendBuffer), BLOCKING);
}

/* Interrupt Handler */
void UART0_IRQHandler (void)
{
	lastReceivedChar = UART_ReceiveByte(LPC_UART0);
	if (receivedValidCommand == FALSE){
		if (lastReceivedChar != '\n')
		{
			lastReceivedCommandString[lastReceivedCommandPosition++] = lastReceivedChar;
			if (lastReceivedCommandPosition > COMMANDLENGTH - 1)
			{
				lastReceivedCommandPosition = 0;
			}
		}
		else {
			receivedValidCommand = TRUE;
		}
	}
}

/* Interrupt Handler */
void UART1_IRQHandler (void)
{

//	unsigned char ucCharacter;
//	ucCharacter = UART_ReceiveByte(LPC_UART1);
//	UART_SendByte(LPC_UART0,ucCharacter);

	//WriteDebugInfo("boe\n");
	//UART_Send(LPC_UART0, UART_ReceiveByte((LPC_UART_TypeDef *)LPC_UART1), Strlen(UART_ReceiveByte((LPC_UART_TypeDef *)LPC_UART1)), BLOCKING);
//	UART_SendByte(LPC_UART0,UART_ReceiveByte((LPC_UART_TypeDef *)LPC_UART0));
}

/* Interrupt Handler */
void UART2_IRQHandler (void)
{
	WriteDebugInfo("hoi");
	unsigned char ucCharacter;
	ucCharacter = UART_ReceiveByte(LPC_UART2);
	UART_SendByte(LPC_UART0,ucCharacter);
	UART_SendByte(LPC_UART0,'g');
	//LED_set(2, 0);
	UART_SendByte(LPC_UART0,UART_ReceiveByte(LPC_UART2));
}

/* Interrupt Handler */
void UART3_IRQHandler (void)
{
	lastReceivedChar = UART_ReceiveByte(LPC_UART3);
	if (isUart0Initialized == TRUE)
	{
		UART_SendByte(LPC_UART0,lastReceivedChar);
	}
	if (receivedValidCommand == FALSE){
		if (lastReceivedChar != '\n')
		{
			lastReceivedCommandString[lastReceivedCommandPosition++] = lastReceivedChar;
			if (lastReceivedCommandPosition > COMMANDLENGTH-1)
			{
				lastReceivedCommandPosition = 0;
			}
		}
		else {
			receivedValidCommand = TRUE;
		}
	}
}

void clearLastCommand()
{
	int i = COMMANDLENGTH-1;
	while (i >= 0)
	{
		lastReceivedCommandString[i] = '\0';
		i--;
	}
}

unsigned char* lastReceivedCommand()
{
	if (receivedValidCommand == TRUE)
	{
		receivedValidCommand =  FALSE;
		int i = COMMANDLENGTH-1;
		while (i >= lastReceivedCommandPosition)
		{
			lastReceivedCommandString[i] = '\0';
			i--;
		}
		lastReceivedCommandPosition = 0;
		return lastReceivedCommandString;
	}
	clearLastCommand();
	return lastReceivedCommandString;
}

