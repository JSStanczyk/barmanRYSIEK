/*
 * measurementStruct.h
 *
 *  Created on: Dec 12, 2021
 *      Author: Yo
 */

#ifndef INC_MEASUREMENTSTRUCT_H_
#define INC_MEASUREMENTSTRUCT_H_

#include <stdint.h>

void measStructInit(void);

/**
 * @brief Struktura zawierające dane pomiarowe z ADC oraz z belki tensometrycznej
 * 
 */
typedef struct{
	uint16_t pump1Current;
	uint16_t pump2Current;
	uint16_t pump3Current;
	uint16_t pump4Current;
	uint16_t pump5Current;
	int32_t glassScaleValue;
} measurement;

extern measurement meas;

#endif /* INC_MEASUREMENTSTRUCT_H_ */
