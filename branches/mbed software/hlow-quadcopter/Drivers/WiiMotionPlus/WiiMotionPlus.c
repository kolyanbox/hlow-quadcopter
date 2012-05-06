#include <Drivers/WiiMotionPlus/WiiMotionPlus.h>

/*Global definitions for I2C*/

/** Own Slave address in Slave I2C device */
#define I2CDEV_S_ADDR_1 (0x53)
#define I2CDEV_S_ADDR_2	(0x52)

#define I2CDEV_M LPC_I2C2

#define RPSPUNIT -0.00126766

/* Global variables */
uint8_t Gyro_TxBuffer[2] = { 0,0 };

I2C_M_SETUP_Type transferMCfg;

int initializeWiiMotionPlus()
{
	//NVIC_EnableIRQ(I2C2_IRQn);

	Gyro_TxBuffer[0] = 0xFE;
	Gyro_TxBuffer[1] = 0x04;

	/* Transmit -------------------------------------------------------- */
	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = 0b1010011;
	transferMCfg.tx_data = Gyro_TxBuffer;
	transferMCfg.tx_length = 2;//sizeof(I2C_TxBuffer);
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;//sizeof(I2C_RxBuffer);
	transferMCfg.retransmissions_max = 3;

	if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_POLLING))
	{
		/*Wait for WiiMotionPlus to change address*/
		int i = 0;
		while (i<9999)
		{
			i++;
		}
		Gyro_TxBuffer[0] = 0x00;
		/* Start I2C slave device first */
		transferMCfg.sl_addr7bit = 0b1010010;
		transferMCfg.tx_data = Gyro_TxBuffer;
		transferMCfg.tx_length = 1;//sizeof(I2C_TxBuffer);
		transferMCfg.rx_data = NULL;
		transferMCfg.rx_length = 0;//sizeof(I2C_RxBuffer);
		transferMCfg.retransmissions_max = 0;
		if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT))
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		return 1;
	}
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
	Gyro_TxBuffer[1] = 0x00;
	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = 0b1010010;
	transferMCfg.tx_data = Gyro_TxBuffer;
	transferMCfg.tx_length = 1;//sizeof(I2C_TxBuffer);
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;//sizeof(I2C_RxBuffer);
	transferMCfg.retransmissions_max = 0;

	if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_POLLING))
	{
		/* Start I2C slave device first */
		transferMCfg.sl_addr7bit = I2CDEV_S_ADDR_2;
		transferMCfg.tx_data = NULL;
		transferMCfg.tx_length = 0;//sizeof(I2C_TxBuffer);
		transferMCfg.rx_data = rxBuffer;
		transferMCfg.rx_length = 6;//sizeof(I2C_RxBuffer);
		transferMCfg.retransmissions_max = 0;

		char test[5];
		Memset(test, '\0', sizeof(test));

		st = I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT);

		if(st)
		{
			while (transferMCfg.rx_count <6);

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
