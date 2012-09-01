#include <Drivers/BMP085/BMP085.h>
#include <Interfaces/Actuators/Actuators.h>
#include <CoOS.h>

OS_EventID sem;
OS_EventID semUt;

/*Global definitions for I2C*/
/** Own Slave address in Slave I2C device */
#define I2CDEV_S_ADDR_1 (0xEF)
#define I2CDEV_S_ADDR_2	(0xEE)
/* Global variables */
#define I2CDEV_M LPC_I2C2
I2C_M_SETUP_Type transferMCfg;

short ac1 = 0;
short ac2 = 0;
short ac3 = 0;
unsigned short ac4 = 0;
unsigned short ac5 = 0;
unsigned short ac6 = 0;
short b1 = 0;
short b2 = 0;
short mb = 0;
short mc = 0;
short md = 0;
long ut = 0;
long up = 0;
long b5 = 0;
long x1 = 0;
long x2 = 0;
short oss1 = 0;

Bool initializeBMP085()
{
	sem = CoCreateSem(0,1,EVENT_SORT_TYPE_FIFO);
	semUt = CoCreateSem(0,1,EVENT_SORT_TYPE_FIFO);
	ac1 = getDataBMP085(0xAA);
	ac2 = getDataBMP085(0xAC);
	ac3 = getDataBMP085(0xAE);
	ac4 = getDataBMP085(0xB0);
	ac5 = getDataBMP085(0xB2);
	ac6 = getDataBMP085(0xB4);
	b1 = getDataBMP085(0xB6);
	b2 = getDataBMP085(0xB8);
	mb = getDataBMP085(0xBA);
	mc = getDataBMP085(0xBC);
	md = getDataBMP085(0xBE);
	if (ac1 == 0x0000 || ac1 == 0xFFFF ||
		ac2 == 0x0000 || ac2 == 0xFFFF ||
		ac3 == 0x0000 || ac3 == 0xFFFF ||
		ac4 == 0x0000 || ac4 == 0xFFFF ||
		ac5 == 0x0000 || ac5 == 0xFFFF ||
		ac6 == 0x0000 || ac6 == 0xFFFF ||
		b1 == 0x0000 || b1 == 0xFFFF ||
		b2 == 0x0000 || b2 == 0xFFFF ||
		mb == 0x0000 || mb == 0xFFFF ||
		mc == 0x0000 || mc == 0xFFFF ||
		md == 0x0000 || md == 0xFFFF)
	{
		return FALSE;
	}
	return TRUE;
}

Bool writeDataBmp085(uint8_t addres, uint8_t transmitData)
{
	uint8_t BMP085_TxBuffer[2] = {addres, transmitData};

	uint8_t rxBuffer[4];
		rxBuffer[0] = 0;
		rxBuffer[1] = 0;
		rxBuffer[2] = 0;
		rxBuffer[3] = 0;


	/* Transmit -------------------------------------------------------- */
	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = 0b1110111;
	transferMCfg.tx_data = BMP085_TxBuffer;
	transferMCfg.tx_length = 2;//sizeof(I2C_TxBuffer);
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;//sizeof(I2C_RxBuffer);
	transferMCfg.retransmissions_max = 0;

	if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_POLLING))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void callbackGetUt()
{
	CoPostSem(semUt);
}

long getUtBMP085(uint8_t transmitMessage)
{
	long i = 0;
	while(i<100000)
	{
		i++;
	}
	writeDataBmp085(0xF4, 0x2E);
	//wait 4.5ms
	i = 0;
	while(i<1000000)
	{
		i++;
	}

	uint8_t BMP085_TxBuffer[1] = { transmitMessage };

	uint8_t rxBuffer[4];
		rxBuffer[0] = 0;
		rxBuffer[1] = 0;
		rxBuffer[2] = 0;
		rxBuffer[3] = 0;


	/* Transmit -------------------------------------------------------- */
	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = 0b1110111;
	transferMCfg.tx_data = BMP085_TxBuffer;
	transferMCfg.tx_length = 1;//sizeof(I2C_TxBuffer);
	transferMCfg.rx_data = rxBuffer;
	transferMCfg.rx_length = 2;//sizeof(I2C_RxBuffer);
	transferMCfg.retransmissions_max = 0;
	transferMCfg.callback = callbackGetUt;

	if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT))
	{
		StatusType s1 = CoPendSem(semUt,0xff);
		if (s1 ==  E_OK){
			long returnValue = rxBuffer[0] << 8;
			returnValue += rxBuffer[1];
			ut = returnValue;
			transferMCfg.callback = 0;
			return returnValue;
		}
		else {
			transferMCfg.callback = 0;
			return -1;
		}
	}
	else
	{
		transferMCfg.callback = 0;
		return -1;
	}
}

void callbackGetUp()
{
	CoPostSem(sem);
}

long getUpBMP085(uint8_t transmitMessage)
{
	long ut = getUtBMP085(0xf6);

	short oss = 0;
	uint8_t transmitData = 0x34;
	transmitData += oss<<6;
	writeDataBmp085(0xF4, transmitData);
	long i = 0;
	while(i<1000000)
	{
		i++;
	}

	oss1 = oss;
	uint8_t BMP085_TxBuffer[1] = { transmitMessage };

	uint8_t rxBuffer[4];
		rxBuffer[0] = 0;
		rxBuffer[1] = 0;
		rxBuffer[2] = 0;
		rxBuffer[3] = 0;


	/* Transmit -------------------------------------------------------- */
	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = 0b1110111;
	transferMCfg.tx_data = BMP085_TxBuffer;
	transferMCfg.tx_length = 1;//sizeof(I2C_TxBuffer);
	transferMCfg.rx_data = rxBuffer;
	transferMCfg.rx_length = 3;//sizeof(I2C_RxBuffer);
	transferMCfg.retransmissions_max = 0;
	transferMCfg.callback = callbackGetUp;

	if(I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_INTERRUPT))
	{
		StatusType s1 = CoPendSem(sem,0xff);
		if (s1 ==  E_OK){
			up = rxBuffer[0] << 16;
			up += rxBuffer[1] << 8;
			up += rxBuffer[2];
			up = up >> (8-oss);
			transferMCfg.callback = 0;
			return up;
		}
		else {
			transferMCfg.callback = 0;
			return -1;
		}
	}
	else
	{
		transferMCfg.callback = 0;
		return -1;
	}
}

short getDataBMP085(uint8_t transmitMessage)
{
	uint8_t BMP085_TxBuffer[1] = { transmitMessage };

	uint8_t rxBuffer[4];
		rxBuffer[0] = 0;
		rxBuffer[1] = 0;
		rxBuffer[2] = 0;
		rxBuffer[3] = 0;


	/* Transmit -------------------------------------------------------- */
	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = 0b1110111;
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

long getTemperature(void)
{
	if (getUpBMP085(0xf6) == -1)
	{
		return -1;
	}

	x1 = (ut-ac6) * ac5 / Pow(2,15);
	x2 = mc * Pow(2,11) / (x1+md);
	b5 = x1 + x2;
	return (b5+8)/Pow(2,4);
}

long getPressure(void)
{
	if (getTemperature() == -1)
	{
		return -1;
	}

	long b6 = b5 - 4000;
	x1 = (b2*(b6*b6/Pow(2,12)))/Pow(2,11);
	x2 = ac2 * b6 / Pow(2,11);
	long x3 = x1+x2;
	long b3 = ((((unsigned long)ac1 * 4 + x3) << oss1) + 2) / 4;
	x1 = ac3*b6/Pow(2,13);
	x2 = (b1*(b6*b6/Pow(2,12)))/Pow(2,16);
	x3 = ((x1+x2)+2)/Pow(2,2);
	unsigned long b4 = ac4*(unsigned long)(x3+32768)/Pow(2,15);
	unsigned long b7 = ((unsigned long)up-b3)*(50000>>oss1);
	long p = 0;
	if (b7 < 0x80000000)
	{
		p = (b7*2)/b4;
	}
	else
	{
		p = (b7/b4)*2;
	}
	x1 = (p/Pow(2,8))*(p/Pow(2,8));
	x1 = (x1*3038)/Pow(2,16);
	x2 = (-7357*p)/Pow(2,16);
	return p+(x1+x2+3791)/Pow(2,4);
}
