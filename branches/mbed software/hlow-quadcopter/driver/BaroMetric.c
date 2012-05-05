#include <driver/BaroMetric.h>
#include <General/util.h>

//uint8_t BaroMetric_TxBuffer[2] = { 0 };
//I2C_M_SETUP_Type transferMCfg;


void BaroMetricInit(void)
{
//	NVIC_EnableIRQ(I2C2_IRQn);

//	DEBUG_Send("Start i2c barometric \n\r");
//	BaroMetric_TxBuffer[0] = 0x40;
//	BaroMetric_TxBuffer[1] = 0x00;

	/* Transmit -------------------------------------------------------- */
	/* Start I2C slave device first */
//	transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
//	transferMCfg.tx_data = BaroMetric_TxBuffer;
//	transferMCfg.tx_length = 2;//sizeof(I2C_TxBuffer);
//	transferMCfg.rx_data = NULL;
//	transferMCfg.rx_length = 0;//sizeof(I2C_RxBuffer);
//	transferMCfg.retransmissions_max = 3;

//	if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT))
//	{
//		while(!MessageReceived);
//		MessageReceived = 0;
//		DEBUG_Send("Good barometric!\n\r");
//	}
//	else
//	{
//		DEBUG_Send("Fail barometric#1!\n\r");
//		return;
//	}
//	return;
}

void Barometric_get(unsigned char* rxBuffer)
{
//	Status st;

//		BaroMetric_TxBuffer[0] = 0x2E;
//		/* Start I2C slave device first */
//		transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
//		transferMCfg.tx_data = BaroMetric_TxBuffer;
//		transferMCfg.tx_length = 1;//sizeof(I2C_TxBuffer);
//		transferMCfg.rx_data = NULL;
//		transferMCfg.rx_length = 0;//sizeof(I2C_RxBuffer);
//		transferMCfg.retransmissions_max = 3;

//		if (CoPendSem(i2c_sem,0) == E_OK)
//		{
//			if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT))
//			{
//				while(!MessageReceived);
//				MessageReceived = 0;
//				DEBUG_Send("Good Barometric #2\n");

//				BaroMetric_TxBuffer[0] = 0x5C;
//				BaroMetric_TxBuffer[1] = 0x96;
//				/* Start I2C slave device first */
//				transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
//				transferMCfg.tx_data = BaroMetric_TxBuffer;
//				transferMCfg.tx_length = 2;//sizeof(I2C_TxBuffer);
//				transferMCfg.rx_data = rxBuffer;
//				transferMCfg.rx_length = 6;//sizeof(I2C_RxBuffer);
//				transferMCfg.retransmissions_max = 3;
//				DEBUG_Send("Start Receiving barometric\n\r");

//				char test[5];
//				Memset(test, '\0', sizeof(test));

//				st = I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT);
//				CoPostSem(i2c_sem);
//				if(st)
//				{
//					while(!MessageReceived);
//					MessageReceived = 0;
//					DEBUG_Send(test);
//					int i =0;
//					for(i = 0; i < 6; i++)
//					{

//						Itoa(rxBuffer[i], test, 10);
//						DEBUG_Send(test);
//						DEBUG_Send("\n\r");
//					}
//				}
//				else
//				{
//					DEBUG_Send("Fail barometric #3!\n\r");
//				}
//			}
//			else
//			{
//				DEBUG_Send("Fail barometric #2!\n\r");
//			}
//		}
}

