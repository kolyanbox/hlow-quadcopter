#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Drivers/Uart/Uart.h>
#include <Drivers/PWM/PWM.h>
#include <lpc_types.h>
#include <lpc_types.h>
#include <lpc_pwm.h>

/*#define SPEED_NEUTRAL	0.05
#define SPEED_LOW		0.05
#define SPEED_HIGH		0.08
#define SPEED_DIFF		(SPEED_HIGH-SPEED_LOW)
#define SPEED_PROCENT	(SPEED_DIFF/100)

enum PWMPORTS {
	pwm1 = COX_PIN(2, 2), //p24
	pwm2 = COX_PIN(2, 3), //p23
	pwm3 = COX_PIN(2, 4), //p22
	pwm4 = COX_PIN(2, 5)  //p21
};
*/
Bool InitializeActuators(void);
//Bool InitializePWM();
Bool WriteDebugInfo(const char * sendBuffer);
//Bool setSpeedFront(int Percent);

#endif
