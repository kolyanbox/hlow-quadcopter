#ifndef ACTUATORS_H_
#define ACTUATORS_H_

/*common includes*/
#include <lpc_types.h>
/*Uart*/
#include <Drivers/Uart/UART.h>
/*GPIO*/
#include <Drivers/GPIO/GPIO.h>

enum LED {
	led1 =  (1 << 18),
	led2 = (1 << 20),
	led3 = (1 << 21),
	led4 = (1 << 23)
};

/*Public actuator initialization*/
Bool ActuatorsInitialization(void);
Bool WriteDebugInfo(const char * sendBuffer);
Bool setLed(enum LED led ,Bool enable);

#endif
