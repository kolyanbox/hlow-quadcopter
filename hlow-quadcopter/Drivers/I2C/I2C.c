#include <Drivers/I2C/I2C.h>

/*Global definitions for I2C*/
/*I2C bus to use*/
#define I2CDEV_M LPC_I2C2

Bool I2C_Initialize()
{
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
	return TRUE;
}

/*Interrupt request handler for I2C*/
void I2C2_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(I2C2_IRQn );
	I2C_MasterHandler(I2CDEV_M);
}
