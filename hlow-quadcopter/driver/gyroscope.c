#include <driver/gyroscope.h>

/*Global definitions for I2C*/
/** Used I2C device as slave definition */
#define USEDI2CDEV_M		2
/** Own Slave address in Slave I2C device */
#define I2CDEV_S_ADDR_1 (0x53)
#define I2CDEV_S_ADDR_2	(0x52)

#define I2CDEV_M LPC_I2C2

#define RESET_I2C 0
#define SET_I2C 0

#define RPSPUNIT -0.00126766

/* Global variables */
uint8_t Gyro_TxBuffer[2] = { 0 };

I2C_M_SETUP_Type transferMCfg;

int gyroscope_init()
{
	NVIC_EnableIRQ(I2C2_IRQn);

	WriteDebugInfo("Start Gyro init\n\r");
		Gyro_TxBuffer[0] = 0xF0;
		Gyro_TxBuffer[1] = 0x55;

		/* Transmit -------------------------------------------------------- */
		/* Start I2C slave device first */
		transferMCfg.sl_addr7bit = I2CDEV_S_ADDR_2;
		transferMCfg.tx_data = Gyro_TxBuffer;
		transferMCfg.tx_length = 2;//sizeof(I2C_TxBuffer);
		transferMCfg.rx_data = NULL;
		transferMCfg.rx_length = 0;//sizeof(I2C_RxBuffer);
		transferMCfg.retransmissions_max = 3;

		if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT))
		{
			while(!MessageReceived);
			MessageReceived = 0;
			WriteDebugInfo("Good #1!\n\r");
		}
		else
		{
			WriteDebugInfo("Fail #1!\n\r");
			return 1;
		}
		int c;
		for(c = 0 ; c < 0xFFF; c++);
		Gyro_TxBuffer[0] = 0xfe;
		Gyro_TxBuffer[1] = 0x04;
		/* Start I2C slave device first */
		transferMCfg.sl_addr7bit = I2CDEV_S_ADDR_1;
		transferMCfg.tx_data = Gyro_TxBuffer;
		transferMCfg.tx_length = 2;//sizeof(I2C_TxBuffer);
		transferMCfg.rx_data = NULL;
		transferMCfg.rx_length = 0;//sizeof(I2C_RxBuffer);
		transferMCfg.retransmissions_max = 3;
		if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT))
		{
			while(!MessageReceived);
			MessageReceived = 0;
			WriteDebugInfo("Good #2\n\r");
		}
		else
		{
			WriteDebugInfo("Fail #2!\n\r");
		}
		return 1;
}

void gyroscope_get()
{
	uint8_t rxBuffer[6];
		rxBuffer[0] = 0;
		rxBuffer[1] = 0;
		rxBuffer[2] = 0;
		rxBuffer[3] = 0;
		rxBuffer[4] = 0;
		rxBuffer[5] = 0;

		Status st;

			Gyro_TxBuffer[0] = 0x00;
			/* Start I2C slave device first */
			transferMCfg.sl_addr7bit = I2CDEV_S_ADDR_2;
			transferMCfg.tx_data = Gyro_TxBuffer;
			transferMCfg.tx_length = 1;//sizeof(I2C_TxBuffer);
			transferMCfg.rx_data = NULL;
			transferMCfg.rx_length = 0;//sizeof(I2C_RxBuffer);
			transferMCfg.retransmissions_max = 3;

			if (CoPendSem(i2c_sem,0) == E_OK)
			{
				if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT))
				{
					while(!MessageReceived);
					MessageReceived = 0;
					WriteDebugInfo("Good #2\n\r");

					/* Start I2C slave device first */
					transferMCfg.sl_addr7bit = I2CDEV_S_ADDR_2;
					transferMCfg.tx_data = NULL;
					transferMCfg.tx_length = 0;//sizeof(I2C_TxBuffer);
					transferMCfg.rx_data = rxBuffer;
					transferMCfg.rx_length = 6;//sizeof(I2C_RxBuffer);
					transferMCfg.retransmissions_max = 3;
					WriteDebugInfo("Start Receiving \n\r");




					char test[5];
					Memset(test, '\0', sizeof(test));

					st = I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT);

					if(st)
					{
						while(!MessageReceived);
						MessageReceived = 0;

						//detect if we ever went into fast mode
						if (rxBuffer[3] & 0x02)
						{
							WriteDebugInfo("fast 1\n");
						}
						if (rxBuffer[4] & 0x02)
						{
							WriteDebugInfo("fast 2\n");
						}
						if (rxBuffer[3] & 0x01)
						{
							WriteDebugInfo("fast 3\n");
						}

						while (transferMCfg.rx_count <6);

						Bool fastdiscard = !(rxBuffer[3] & 0x02 && rxBuffer[4] & 0x02 && rxBuffer[3] & 0x01);
						if (fastdiscard)
						{
							WriteDebugInfo("fastdiscard");
						}

						float gyrodata[3];
						short tmpGyro;
						tmpGyro = (rxBuffer[3] >> 2);
						tmpGyro <<=8;
						gyrodata[0] = tmpGyro;
						gyrodata[0] += rxBuffer[0];
						gyrodata[0] -= 8000;
						gyrodata[0] *= RPSPUNIT;

						tmpGyro = (rxBuffer[4] >> 2);
						tmpGyro <<= 8;
						gyrodata[1] = tmpGyro;
						gyrodata[1] += rxBuffer[1];
						gyrodata[1] -= 8000;
						gyrodata[1] *= RPSPUNIT;

						tmpGyro = (rxBuffer[5] >> 2);
						tmpGyro <<= 8;
						gyrodata[2] = tmpGyro;
						gyrodata[2] += rxBuffer[2];
						gyrodata[2] -= 8000;
						gyrodata[2] *= RPSPUNIT;



						WriteDebugInfo("Gyrodata0: ");
						Ftoa(gyrodata[0],test,10,'f');
						WriteDebugInfo(test);
						WriteDebugInfo("\r");

						WriteDebugInfo("Gyrodata1: ");
						Ftoa(gyrodata[1],test,10,'f');
						WriteDebugInfo(test);
						WriteDebugInfo("\r");

						WriteDebugInfo("Gyrodata2: ");
						Ftoa(gyrodata[2],test,10,'f');
						WriteDebugInfo(test);
						WriteDebugInfo("\r");
					}
					else
					{
						WriteDebugInfo("Fail #3!\n\r");
					}
				}
				else
				{
					WriteDebugInfo("Fail #2!\n\r");
				}
			}
			CoPostSem(i2c_sem);
}
