#ifndef _SETPOINT_H_
#define _SETPOINT_H_

/** system includes  --------------------------------------------------------- **/
#include "lpc_types.h"

typedef enum {height, accel_x, accel_y, accel_z, gyros_x, gyros_y, gyros_z} bias_t;

float getBias(const bias_t);

/* ! _SETPOINT_H_ */
#endif
