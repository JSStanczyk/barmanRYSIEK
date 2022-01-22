/*
 * glassScale.c
 *
 *  Created on: Dec 10, 2021
 *      Author: Yo
 */

#include <glassScale.h>
#include <usart.h>
#include <stdio.h>
#include <math.h>

uint8_t bit = 0/** pojedyncz bit odczytany przetwornika od belki tensometrycznej*/, com[20];
uint16_t com_size = 0;
int32_t emptyScaleValue = 0;/** wartość odczytu dla pustej wagi*/

/**
 * @brief Funkcja odczytuje i przelicza na gramy dane z belki tensometrycznej
 * 
 */
void readGlassScaleValue(void){

	 if(HAL_GPIO_ReadPin(GLASS_SCALE_DATA_GPIO_Port, GLASS_SCALE_DATA_Pin) == GPIO_PIN_RESET){

		meas.glassScaleValue = 0;

		for(uint8_t i = 1; i<=25; i++){

			pulse();

			bit = (uint8_t)HAL_GPIO_ReadPin(GLASS_SCALE_DATA_GPIO_Port, GLASS_SCALE_DATA_Pin);

			meas.glassScaleValue = (meas.glassScaleValue << 1) | bit;
		}
		meas.glassScaleValue /= 1000;
		meas.glassScaleValue = emptyScaleValue - meas.glassScaleValue;
		meas.glassScaleValue = round(meas.glassScaleValue/3.375);
		com_size = sprintf(com, "%ld\n\r", meas.glassScaleValue);
		HAL_UART_Transmit_IT(&huart2, com, com_size);
	}
}

/**
 * @brief Funkcja inicjalizuje wagę. Odczytuje i zapisuje wartość pustej wagi używaną potem do przeskalowywanie odczytów wagi.
 * 
 */
void glassScaleInit(void){
	uint8_t i = 1;
	while(i){
		if(HAL_GPIO_ReadPin(GLASS_SCALE_DATA_GPIO_Port, GLASS_SCALE_DATA_Pin) == GPIO_PIN_RESET){
			emptyScaleValue = 0;

			for(uint8_t i = 1; i<=25; i++){

				pulse();

				bit = (uint8_t)HAL_GPIO_ReadPin(GLASS_SCALE_DATA_GPIO_Port, GLASS_SCALE_DATA_Pin);

				emptyScaleValue = (emptyScaleValue << 1) | bit;
			}
			emptyScaleValue /= 1000;
			i--;
		}
	}
}

/**
 * @brief Funkcja wysyła impuls zegarowy dla przetwornika obsługującego belkę tensometryczną. Jest on znakiem że ma
 * wysłać jeden bit.
 * 
 */
void pulse(void){

	HAL_GPIO_WritePin(GLASS_SCALE_SCK_GPIO_Port, GLASS_SCALE_SCK_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GLASS_SCALE_SCK_GPIO_Port, GLASS_SCALE_SCK_Pin, GPIO_PIN_RESET);
}

