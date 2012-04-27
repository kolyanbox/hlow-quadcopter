#include <driver/Accelero.h>
#include <driver/util.h>

/*Global definitions for I2C*/
/** Used I2C device as slave definition */
#define USEDI2CDEV_M		2
/** Own Slave address in Slave I2C device */
#define I2CDEV_S_ADDR	(0x52)

#define I2CDEV_M LPC_I2C2

#define RESET_I2C 0
#define SET_I2C 0

/* Global variables */
uint8_t Accelero_TxBuffer[2] = { 0 };
uint16_t AX = 0;
uint16_t AZ = 0;
uint16_t AY = 0;


I2C_M_SETUP_Type transferMCfg;

int Accelero_init()
{
	NVIC_EnableIRQ(I2C2_IRQn);

	DEBUG_Send("Start i2c\n\r");
	Accelero_TxBuffer[0] = 0x40;
	Accelero_TxBuffer[1] = 0x00;

	/* Transmit -------------------------------------------------------- */
	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
	transferMCfg.tx_data = Accelero_TxBuffer;
	transferMCfg.tx_length = 2;//sizeof(I2C_TxBuffer);
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;//sizeof(I2C_RxBuffer);
	transferMCfg.retransmissions_max = 3;

	if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT))
	{
		while(!MessageReceived);
		MessageReceived = 0;
		DEBUG_Send("Good #1!\n\r");
	}
	else
	{
		DEBUG_Send("Fail #1!\n\r");
		return 1;
	}
	return 1;
}

void Accelero_get(unsigned char* rxBuffer)
{
	Status st;

	DEBUG_Send("get");

	Accelero_TxBuffer[0] = 0x00;
	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
	transferMCfg.tx_data = Accelero_TxBuffer;
	transferMCfg.tx_length = 2;
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;//sizeof(I2C_RxBuffer);
	transferMCfg.retransmissions_max = 3;

	if (CoPendSem(i2c_sem,0) == E_OK)
	{
		if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT))
		{
			while(!MessageReceived);
			MessageReceived = 0;
			DEBUG_Send("Good #2\n\r");


			/* Start I2C slave device first */
			transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
			transferMCfg.tx_data = NULL;
			transferMCfg.tx_length = 0;//sizeof(I2C_TxBuffer);
			transferMCfg.rx_data = rxBuffer;
			transferMCfg.rx_length = 6;//sizeof(I2C_RxBuffer);
			transferMCfg.retransmissions_max = 3;
			DEBUG_Send("Start Receiving \n\r");

			char test[5];
			Memset(test, '\0', sizeof(test));

			st = I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT);

			CoPostSem(i2c_sem);

			if(st)
			{
				while(!MessageReceived);
				MessageReceived = 0;
				DEBUG_Send(test);
				int i =0;
				for(i = 2; i < 6; i++)
				{
					if(i == 2)
					{
						//bit shift the outcome 2 places to the left
						AX = rxBuffer[i] *2*2;
					}
					else if(i == 3)
					{
						//bit shift the outcome 2 places to the left
						AY = rxBuffer[i] *2*2;
					}
					else if(i == 4)
					{
						//bit shift the outcome 2 places to the left
						AZ = rxBuffer[i] *2*2;
					}
					//put the two lsb bits at the end
					else if (i == 5)
					{
						if ((rxBuffer[i] >> 2) & 1)
						{
						  AX += 2;
						}
						if ((rxBuffer[i] >> 3) & 1)
						{
							AX += 1;
						}
						if ((rxBuffer[i] >> 4) & 1)
						{
							AY += 2;
						}
						if ((rxBuffer[i] >> 5) & 1)
						{
							AY += 1;
						}
						if ((rxBuffer[i] >> 6) & 1)
						{
							AZ += 2;
						}
						if ((rxBuffer[i] >> 7) & 1)
						{
							AZ += 1;
						}
					}
				}
			}
			else
			{
				DEBUG_Send("Fail #3!\n\r");
			}
		}
		else
		{
			DEBUG_Send("Fail #2!\n\r");
		}
	}
}

uint16_t getAccelero(int ValueNumber)
{
	switch(ValueNumber)
	{
	case 0:
		return AX;
		break;
	case 1:
		return AY;
		break;
	case 2:
		return AZ;
		break;
	default:
		return 0;
	}
}
