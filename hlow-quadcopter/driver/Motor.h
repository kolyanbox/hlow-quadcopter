#ifndef MOTOR_H_
#define MOTOR_H_

#include <lpc_types.h>
#include <cox_types.h>

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

/*Initialize the motors if succeeded return true otherwise return false*/
Bool MotorInit(void);

/*set motor speed. motor*/
Bool SetMotor(enum MOTOR Motor, int Percent);

#endif
