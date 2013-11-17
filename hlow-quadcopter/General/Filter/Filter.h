/***************************************************************************//**
 * @file      : Filter.h
 * @brief     : Filtering, Used to apply filters on data
 * @version   : V0.1
 * @date      : 16. November. 2013
 * @knownbugs : -
*******************************************************************************/

#ifndef FILTER_H_
#define FILTER_H_

/**
 * Method used to apply a low pass filter
 * @param sampleData the current sample
 * @param amountOfSamples amount of samples
 */
float lowPassFilter(float sampleData[], int amountOfSamples);


#endif
