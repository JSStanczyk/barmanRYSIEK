/*
 * currentMeasurements.c
 *
 *  Created on: 14 gru 2021
 *      Author: Yo
 */

#include <measurementStruct.h>
#include <adc.h>
#include <currentMeasurements.h>

uint32_t ADC_raw_data[5] = {0}; /** tablica z surowymi odczytami z ADC*/

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	meas.pump1Current = ADC_raw_data[0];
	meas.pump2Current = ADC_raw_data[1];
	meas.pump3Current = ADC_raw_data[2];
	meas.pump4Current = ADC_raw_data[3];
	meas.pump5Current = ADC_raw_data[4];
}

/**
 * @brief Funkcja rozpoczynająca pojedunczy pomiar prądu
 * 
 */
void measurePumpCurrent(void){
	HAL_ADC_Start_DMA(&hadc1, ADC_raw_data, 5);
}

