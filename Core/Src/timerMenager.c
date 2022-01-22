/*
 * timerMenager.c
 *
 *  Created on: 17 sty 2022
 *      Author: Yo
 */

#include <tim.h>
#include <timerMenager.h>
#include <glassScale.h>
#include <adc.h>
#include <usart.h>
#include <barman.h>
#include <currentMeasurements.h>

uint8_t typeOfMeas = 1;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM9){
		getMessageFromMainBuf();

	} else if(htim->Instance == TIM10){
		if(typeOfMeas == EMPTY_GLASS_WEIGHT_MEAS){
			readGlassScaleValue();
			setEmptyGlassWeight();

		} else if(typeOfMeas == POURED_LIQUID_WEIGHT_MEAS){
			readGlassScaleValue();
			checkPouredVolume();
		}
	} else if(htim->Instance == TIM11){
		leadDrinkMaking();
	}
}

/**
 * @brief Funkcja startuje timer od pomiarów
 * 
 */
void startMeasurementTimer(void){
	HAL_TIM_Base_Start_IT(&htim10);
}

/**
 * @brief Funkcja zatrzymuje timer od pomiarów
 * 
 */
void stopMeasurementTimer(void){
	HAL_TIM_Base_Stop_IT(&htim10);
}

/**
 * @brief Funkcja startuje timer sterujący robieniem drinków
 * 
 */
void startDrinkMakeTimer(void){
	HAL_TIM_Base_Start_IT(&htim11);
}

/**
 * @brief Funkcja zatrzymuje timer sterujący robieniem drinków
 * 
 */
void stopDrinkMakeTimer(void){
	HAL_TIM_Base_Stop_IT(&htim11);
}

/**
 * @brief Funkcja startuje timer sprawdzający stan bufora cyrkulacyjnego
 * 
 */
void startCommMainBufCheckTimer(void){
	HAL_TIM_Base_Start_IT(&htim9);
}

/**
 * @brief Funkcja zatrzymuje timer sprawdzający stan bufora cyrkulacyjnego
 * 
 */
void stopCommMainBufCheckTimer(void){
	HAL_TIM_Base_Stop_IT(&htim9);
}


