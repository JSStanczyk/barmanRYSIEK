/*
 * measurementStruct.c
 *
 *  Created on: Dec 12, 2021
 *      Author: Yo
 */

#include <measurementStruct.h>
#include <gpio.h>

measurement meas;

/**
 * @brief Funkcja inicjalizuje strukturę pomiarów
 * 
 */
void measStructInit(void){
	meas.pump1Current = 0;
	meas.pump2Current = 0;
	meas.pump3Current = 0;
	meas.pump4Current = 0;
	meas.pump5Current = 0;
	meas.glassScaleValue = 0;
}
