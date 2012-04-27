/** system includes  --------------------------------------------------------- **/

/** user includes  ----------------------------------------------------------- **/
#include "setpoint.h"


/** defines  ----------------------------------------------------------------- **/
/** typedefs  ---------------------------------------------------------------- **/
/** local global variables  -------------------------------------------------- **/
/** functions  --------------------------------------------------------------- **/

/*********************************************************************//**
 * @brief
 * @param[in]
 * @param[in]
 * @return
 **********************************************************************/
float getBias(const bias_t bias) {
	float out;

	switch ( bias ) {
		case height:
			out = 10;
			break;
		case accel_x:

			break;
		case accel_y:

			break;
		case accel_z:

			break;
		case gyros_x:

			break;
		case gyros_y:

			break;
		case gyros_z:

			break;
		default:
			out = -1;
			break;
	}

	return out;
}

