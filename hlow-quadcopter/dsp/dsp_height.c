/** system includes  --------------------------------------------------------- **/
#include "CoOS.h"


/** user includes  ----------------------------------------------------------- **/
#include "dsp_height.h"
#include "coefs_height.h"
#include <System/Sensors.h>
#include "setpoint.h"

/** defines  ----------------------------------------------------------------- **/
#define BIQUAD_COUNT		10
#define SIGNAL_COUNT		1
#define FILTER_COUNT 		SIGNAL_COUNT

/** typedefs  ---------------------------------------------------------------- **/
typedef struct {
	real32_T	afIn[3];
	real32_T	afOut[3];
}	biquad_t;

typedef struct {
	biquad_t	bi_quad[BIQUAD_COUNT];
}	filter_t;

typedef struct {
	real32_T	prev_err;
	real32_T	p,i,d;
	real32_T	integral;
} pid_t;

/** local global variables  -------------------------------------------------- **/
filter_t	filter[FILTER_COUNT];


/** functions  --------------------------------------------------------------- **/

/*********************************************************************//**
 * @brief
 * @param[in]
 * @param[in]
 * @return
 **********************************************************************/
void dsp_height_init(void) {
	int iIndex_filter;
	int iIndex_biquad;
	int iIndex_signal;

	/* clean signal values in each filter */
	for ( iIndex_filter = 0; iIndex_filter < FILTER_COUNT; iIndex_filter++ ) {
		for ( iIndex_biquad = 0; iIndex_biquad < BIQUAD_COUNT; iIndex_biquad++ ) {
			for ( iIndex_signal = 0; iIndex_signal < 3; iIndex_signal++ ) {
				filter[iIndex_filter].bi_quad[iIndex_biquad].afOut[iIndex_signal] = 0;
				filter[iIndex_filter].bi_quad[iIndex_biquad].afIn[iIndex_signal] = 0;
			}
		}
	}
}

/*********************************************************************//**
 * @brief
 * @param[in]
 * @param[in]
 * @return
 **********************************************************************/
void dsp_filter_height(const uint8_t *paiNew_in, int16_t *paiNew_out) {
	int32_t	iIndex_filter, iIndex_biquad, iIndex_signal;
	real32_T	signal;

	/* process each filter */
	for ( iIndex_filter = 0; iIndex_filter < FILTER_COUNT; iIndex_filter++ ) {
		filter_t	*filter_current = &filter[iIndex_filter];

		/* add new sample to biquad number one and signal number one */
		signal = *( paiNew_in + iIndex_filter );

		/* calculate new out value of each biquad */
		for ( iIndex_biquad = 0; iIndex_biquad < BIQUAD_COUNT; iIndex_biquad++ ) {
			biquad_t		*bi_quad_current = &filter_current->bi_quad[iIndex_biquad];
			const real32_T	*coefficient_in = NUM[iIndex_biquad];
			const real32_T	*coefficient_out = DEN[iIndex_biquad];

			/* shift all values of each biquad one back in time */
			for ( iIndex_signal = 2; iIndex_signal > 0; iIndex_signal-- ) {
				/* shift in array */
				bi_quad_current->afIn[iIndex_signal] = bi_quad_current->afIn[iIndex_signal - 1];
				/* shift out array */
				bi_quad_current->afOut[iIndex_signal] = bi_quad_current->afOut[iIndex_signal - 1];
			}

			/* add new in-sample */
			bi_quad_current->afIn[0] = signal;

			/* reset biquad out signal */
			bi_quad_current->afOut[0] = 0;

			/* calculate new out of biquad */
			for ( iIndex_signal = 0; iIndex_signal < 3; iIndex_signal++ ) {
				/* add calculated in value */
				bi_quad_current->afOut[0] += bi_quad_current->afIn[iIndex_signal] * coefficient_in[iIndex_signal];

				/* add calculated out value */
				if ( iIndex_signal != 0 ) {
					bi_quad_current->afOut[0] -= bi_quad_current->afOut[iIndex_signal] * coefficient_out[iIndex_signal];
				}
			}

			/* store founded value to signal */
			signal = bi_quad_current->afOut[0];
		}

		/* add calculated output to out array */
		*( paiNew_out + iIndex_filter ) = (int)((signal*100)+0.5);
	}
}

static float pid ( pid_t *in, const float err ) {
	float derivative;
	int dt = 1; //todo

	in->integral = in->integral + (err*dt);
	derivative = (err - in->prev_err)/dt;
	in->prev_err = err;

	return (in->p*err) + (in->i*in->integral) + (in->d*derivative);
}

/************************************************************************/
/*   OS TASKS                                                           */
/************************************************************************/


/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
void dsp_height_task (void *pvParameters) {
	pid_t		pid_height;
	uint8_t		raw_value;
	int16_t		value;
	int16_t		error;

	/* set pid parameters */
	pid_height.p = 2;
	pid_height.i = 2;
	pid_height.d = 2;

    /* perform an action every 1/100 seconds */
	for ( ;; ) {
		/* wait for the next cycle */
		CoTickDelay (125);

		/* sample signals */
		getValue(distanceToGround, 0, &raw_value);

		/* filter signals */
		dsp_filter_height(&raw_value, &value);

		/* get desired bias (setpoint) */
		error = getBias( height ) - value;

		/* pid */
		value = pid(&pid_height, error);
	}
}
