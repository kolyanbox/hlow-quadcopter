/***************************************************************************//**
 * @file      : Filter.c
 * @brief     : Filtering, Used to apply filters on data
 * @version   : V0.1
 * @date      : 16. November. 2013
 * @knownbugs : -
 *******************************************************************************/
#include <General/Filter/Filter.h>

float lowPassFilter(float sampleData[], int amountOfSamples) {
	float total = 0;
	int i;
	for (i = 0; i < amountOfSamples; i++) {
		total += sampleData[i];
	}
	return total/=amountOfSamples;
}
