#include <Drivers/WiiMotionPlus/WiiMotionPlus.h>
#include <Tasks/Debug/DebugTask.h>
#include <Drivers/Uart/Uart.h>
#include <Tasks/Logging/LoggingTask.h>

/*Global definitions for I2C*/

/** Own Slave address in Slave I2C device */
#define I2CDEV_S_ADDR_1 (0x53)
#define I2CDEV_S_ADDR_2	(0x52)

#define I2CDEV_M LPC_I2C2

/* Global variables */
uint8_t Gyro_TxBuffer[2] = { 0, 0 };

I2C_M_SETUP_Type transferMCfg;

OS_EventID wiiMotionPlusSensorSem;
float gyrodata[3];

float calX;
float calY;
float calZ;
unsigned char wiiLogging = 0;
unsigned char appName[] = { "wmp Sensor" };

Bool initializeWiiMotionPlus() {
	wiiLogging = registerApp(appName, LOG_DEBUG);
	writeLog(wiiLogging, "Starting initialization", LOG_DEBUG);
	wiiMotionPlusSensorSem = CoCreateSem(1, 1, EVENT_SORT_TYPE_FIFO);
	if (wiiMotionPlusSensorSem == E_CREATE_FAIL) {
		writeLog(wiiLogging, "Semafore couldn't be created", LOG_ERR);
		return FALSE;
	}
	CoPostSem(wiiMotionPlusSensorSem);

	Bool partialReturnValue = FALSE;

	Gyro_TxBuffer[0] = 0xFE;
	Gyro_TxBuffer[1] = 0x04;

	/* Transmit -------------------------------------------------------- */
	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = 0b1010011;
	transferMCfg.tx_data = Gyro_TxBuffer;
	transferMCfg.tx_length = 2; //sizeof(I2C_TxBuffer);
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0; //sizeof(I2C_RxBuffer);
	transferMCfg.retransmissions_max = 3;

	if (I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_POLLING)) {
		writeLog(wiiLogging, "Wait for address change", LOG_DEBUG);
		/*Wait for WiiMotionPlus to change address*/
		int i = 0;
		while (i < 9999) {
			i++;
		}
		Gyro_TxBuffer[0] = 0x00;
		/* Start I2C slave device first */
		transferMCfg.sl_addr7bit = 0b1010010;
		transferMCfg.tx_data = Gyro_TxBuffer;
		transferMCfg.tx_length = 1; //sizeof(I2C_TxBuffer);
		transferMCfg.rx_data = NULL;
		transferMCfg.rx_length = 0; //sizeof(I2C_RxBuffer);
		transferMCfg.retransmissions_max = 0;

		partialReturnValue = FALSE;
	} else {
		partialReturnValue = FALSE;
	}

	/*Check if address is already changed*/
	if (partialReturnValue == FALSE) {
		Gyro_TxBuffer[0] = 0x00;
		/* Start I2C slave device first */
		transferMCfg.sl_addr7bit = 0b1010010;
		transferMCfg.tx_data = Gyro_TxBuffer;
		transferMCfg.tx_length = 1; //sizeof(I2C_TxBuffer);
		transferMCfg.rx_data = NULL;
		transferMCfg.rx_length = 0; //sizeof(I2C_RxBuffer);
		transferMCfg.retransmissions_max = 0;
		if (I2C_MasterTransferData(I2CDEV_M, &transferMCfg,
				I2C_TRANSFER_INTERRUPT)) {
			return TRUE;
		} else {
			writeLog(wiiLogging, "Master transfer data failed", LOG_ERR);
			return FALSE;
		}
	} else {
		int i;
		for (i = 0; i < 100; i++) {
			gyroscope_get();
			calX += gyroscope_get_value(0)[0];
			calY += gyroscope_get_value(1)[1];
			calZ += gyroscope_get_value(2)[2];
		}
		return partialReturnValue;
	}
}
uint8_t rxBuffer[6];
char test[5];

void callback() {
	float data[3];
	short tmpGyro;
	tmpGyro = (rxBuffer[3] >> 2);
	tmpGyro <<= 8;
	data[0] = tmpGyro;
	data[0] += rxBuffer[0];
	data[0] -= 8000;
	data[0] /= 13.768;

	tmpGyro = (rxBuffer[4] >> 2);
	tmpGyro <<= 8;
	data[1] = tmpGyro;
	data[1] += rxBuffer[1];
	data[1] -= 8000;
	data[1] /= 13.768;

	tmpGyro = (rxBuffer[5] >> 2);
	tmpGyro <<= 8;
	data[2] = tmpGyro;
	data[2] += rxBuffer[2];
	data[2] -= 8000;
	data[2] /= 13.768;

	//detect if we ever went into fast mode
	Bool fastdiscard = !(rxBuffer[3] & 0x02 && rxBuffer[4] & 0x02
			&& rxBuffer[3] & 0x01);
	if (fastdiscard) {
//		if (CoPendSem(wiiMotionPlusSensorSem, 10) == E_OK) {
			gyrodata[0] = data[0]*2000/440;
			gyrodata[1] = data[1]*2000/440;
			gyrodata[2] = data[2]*2000/440;
//			CoPostSem(wiiMotionPlusSensorSem);
//		}
	} else {
//		if (CoPendSem(wiiMotionPlusSensorSem, 10) == E_OK) {
			gyrodata[0] = data[0];
			gyrodata[1] = data[1];
			gyrodata[2] = data[2];
//			CoPostSem(wiiMotionPlusSensorSem);
//		}
	}
}

float* gyroscope_get_value() {
	float data[3];
	if (CoPendSem(wiiMotionPlusSensorSem, 10) == E_OK) {
		data[0] = gyrodata[0];
		data[1] = gyrodata[1];
		data[2] = gyrodata[2];
//		char c[5];
//		Ftoa(data[0],c,1,'f');
//		WriteDebugInfo(c);
//		WriteDebugInfo("<<");
		CoPostSem(wiiMotionPlusSensorSem);
	}
	return data;
}

void gyroscope_get() {
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
	transferMCfg.tx_length = 1; //sizeof(I2C_TxBuffer);
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0; //sizeof(I2C_RxBuffer);
	transferMCfg.retransmissions_max = 0;

	if (I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_POLLING)
			== SUCCESS) {
		/* Start I2C slave device first */
		transferMCfg.sl_addr7bit = I2CDEV_S_ADDR_2;
		transferMCfg.tx_data = NULL;
		transferMCfg.tx_length = 0; //sizeof(I2C_TxBuffer);
		transferMCfg.rx_data = rxBuffer;
		transferMCfg.rx_length = 6; //sizeof(I2C_RxBuffer);
		transferMCfg.retransmissions_max = 0;
		transferMCfg.callback = callback;

		st = I2C_MasterTransferData(I2CDEV_M, &transferMCfg,
				I2C_TRANSFER_INTERRUPT);
	} else {
		writeLog(wiiLogging, "Failed to get data", LOG_ERR);
	}
}
