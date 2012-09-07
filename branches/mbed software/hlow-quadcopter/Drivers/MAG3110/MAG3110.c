#include <Drivers/MAG3110/MAG3110.h>
#include <CoOS.h>

#define I2CDEV_M LPC_I2C2
I2C_M_SETUP_Type transferMCfg;

Bool writeDataMAG3110(uint8_t addres, uint8_t transmitData)
{
	uint8_t MAG3110_TxBuffer[2] = {addres, transmitData};

	/* Transmit -------------------------------------------------------- */
	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = 0b0000111;
	transferMCfg.tx_data = MAG3110_TxBuffer;
	transferMCfg.tx_length = 1;//sizeof(I2C_TxBuffer);
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;//sizeof(I2C_RxBuffer);
	transferMCfg.retransmissions_max = 0;
	transferMCfg.callback = NULL;

	if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_POLLING))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

short getDataMAG3110(uint8_t transmitMessage)
{
	uint8_t BMP085_TxBuffer[1] = { transmitMessage };

	uint8_t rxBuffer[4];
		rxBuffer[0] = 0;
		rxBuffer[1] = 0;
		rxBuffer[2] = 0;
		rxBuffer[3] = 0;


	/* Transmit -------------------------------------------------------- */
	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = 0b0000111;
	transferMCfg.tx_data = BMP085_TxBuffer;
	transferMCfg.tx_length = 1;//sizeof(I2C_TxBuffer);
	transferMCfg.rx_data = rxBuffer;
	transferMCfg.rx_length = 2;//sizeof(I2C_RxBuffer);
	transferMCfg.retransmissions_max = 0;
	transferMCfg.callback = NULL;

	if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT))
	{
		while (transferMCfg.rx_count <2);
		short returnValue = rxBuffer[0] << 8;
		return returnValue += rxBuffer[1];
	}
	else
	{
		return 1;
	}
}


Bool initializeMAG3110 ()
{
	if (writeDataMAG3110(0x11, 0x80) == FALSE)
	{
		CoTimeDelay(0,0,1,0);
		return FALSE;
	}
	CoTimeDelay(0,0,1,0);
	//if (!writeDataMAG3110(0x10, 0x01))
	//{
	//	return FALSE;
	//}
	return TRUE;
}
