#include <System/Init.h>

/*Initialize the peripherals*/
int init_Peripherals(void)
{
	/*Setup UART3 for testing gps purposes (pinout: p9, p10)*/
	UARTInit(LPC_UART3, 9600);

	/*Setup uart 2 for wifi module*/
	//UART1Init(LPC_UART1, 9600);
	/*LED initialization (The 4 LEDs at the MBED)*/
	/*Ultrasonic initialization*/
	//initUltrasonicSensor();
	//LED_init();
	/*Initialize I2C*/
	I2C_Initialize();

	/*Initialize ADC*/
	///if (SensorInit() == FALSE)
	//{
	//	return 1;
	//}
	/*Barometric sensor initialization*/
	//BaroMetricInit();

	return 0;
}
