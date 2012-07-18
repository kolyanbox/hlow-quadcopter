#ifndef ACTUATORS_H_
#define ACTUATORS_H_

/*common includes*/
#include <lpc_types.h>
#include <cox_types.h>

enum LED {
	led1 =  (1 << 18),
	led2 = (1 << 20),
	led3 = (1 << 21),
	led4 = (1 << 23)
};

enum ActuatorType{
	ActuatorTelemetry = 0,
	ActuatorLeds = 1,
	ActuatorMotors = 2
};

#define USEBOTHDEBUGINGMODULES TRUE

#define PERIOD			0.018
#define SPEED_NEUTRAL	0.05
#define SPEED_LOW		0.05
#define SPEED_HIGH		0.08
#define SPEED_DIFF		(SPEED_HIGH-SPEED_LOW)
#define SPEED_PROCENT	(SPEED_DIFF/100)

enum MOTOR {
	motor1 = COX_PIN(2, 2), //p24
	motor2 = COX_PIN(2, 3), //p23
	motor3 = COX_PIN(2, 4), //p22
	motor4 = COX_PIN(2, 5)  //p21
};

/*Public actuator initialization*/
Bool ActuatorsInitialization(enum ActuatorType actuatorType);
Bool WriteDebugInfo(const char * sendBuffer);
Bool setLed(enum LED led ,Bool enable);
Bool setSpeedFront(int speedPercent);
Bool setSpeedBack(int speedPercent);
Bool setSpeedLeft(int speedPercent);
Bool setSpeedRight(int speedPercent);

#endif
