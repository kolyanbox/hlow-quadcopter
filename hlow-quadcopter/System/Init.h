#ifndef INIT_H_
#define INIT_H_

/*Basic includes*/
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

/*UART driver*/
#include <Interfaces/Actuators/Actuators.h>

/*I2C driver*/
#include <Drivers/I2C/I2C.h>

/*Ultrasonic driver*/
//#include <driver/UltrasonicSensor.h>

/*Initialize peripherals*/
int init_Peripherals(void);

//#include <System/Sensors.h>
#endif
