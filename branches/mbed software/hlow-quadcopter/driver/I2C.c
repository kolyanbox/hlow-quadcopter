#include <driver/I2c.h>

/*Global definitions for I2C*/
/** Used I2C device as slave definition */
#define USEDI2CDEV_M		2
/** Own Slave address in Slave I2C device */
#define I2CDEV_S_ADDR	(0x52)

#define I2CDEV_M LPC_I2C2

#define RESET_I2C 0
#define SET_I2C 0

/* Global variables */
uint8_t I2C_TxBuffer[2] = { 0 };
volatile uint8_t MessageReceived = 0;
volatile int totalMessages = 0;
void I2C_Initialize(void)
{
	i2c_sem = CoCreateSem(0,1,EVENT_SORT_TYPE_FIFO);
	if (i2c_sem == E_CREATE_FAIL)
    {
		return;
    }

	PINSEL_CFG_Type PinCfg;
	/*
	 * Initialize I2C pin connect
	 */
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Funcnum = 2;
	PinCfg.Pinnum = 10;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 11;
	PINSEL_ConfigPin(&PinCfg);

	/* Initialize Slave I2C peripheral */
	/* 100KHz */
	I2C_Init(I2CDEV_M, 100000);

	/* Enable Slave I2C operation */
	I2C_Cmd(I2CDEV_M, ENABLE);

	CoPostSem(i2c_sem);
}

/*Interrupt request handler for I2C*/
int count = 0;
void I2C2_IRQHandler(void)
{
	MessageReceived = 1;
	NVIC_ClearPendingIRQ(I2C2_IRQn );
	I2C_MasterHandler(I2CDEV_M);
}
