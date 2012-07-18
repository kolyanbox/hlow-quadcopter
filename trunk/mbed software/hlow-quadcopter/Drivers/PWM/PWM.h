#ifndef PWM_H_
#define PWM_H_

#include <lpc_types.h>
#include <cox_types.h>
#include <lpc_pwm.h>

#define SPEED_NEUTRAL	0.05
#define SPEED_LOW		0.05
#define SPEED_HIGH		0.08
#define SPEED_DIFF		(SPEED_HIGH-SPEED_LOW)
#define SPEED_PROCENT	(SPEED_DIFF/100)

/*Initialize a pwm port if succeeded return true otherwise return false*/
Bool initializePWM(uint16_t pwmPort, float period);

/*set motor speed. motor*/
Bool setPWM(uint16_t pwmPin, float duty);

#endif
