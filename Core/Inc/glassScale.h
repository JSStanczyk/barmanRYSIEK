/*
 * glassScale.h
 *
 *  Created on: Dec 10, 2021
 *      Author: Yo
 */

#ifndef INC_GLASSSCALE_H_
#define INC_GLASSSCALE_H_

#include <gpio.h>
#include <stdint.h>
#include <measurementStruct.h>

void pulse(void);
void readGlassScaleValue(void);
void glassScaleInit(void);

#endif /* INC_GLASSSCALE_H_ */
