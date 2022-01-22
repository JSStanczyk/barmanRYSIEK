/*
 * availableIngredients.h
 *
 *  Created on: 6 sty 2022
 *      Author: Yo
 */

#ifndef INC_AVAILABLEINGREDIENTS_H_
#define INC_AVAILABLEINGREDIENTS_H_

#include <stm32f411xe.h>
#include <stdint.h>

#define NUMBER_OF_INGREDIENTS 5

void availableIngredientsInit(void);
void addIngredient(uint8_t place, char* IgName);
void removeIngredient(uint8_t place);

/**
 * @brief Struktura zawierająca dane odnośnie odstępnego składnika
 * 
 */
typedef struct{
	GPIO_TypeDef* PUMP_PORT;
	uint16_t PUMP_PIN;
	char IgName[30];

}availableIngredientStruct;

/**
 * @brief talica dostępnych składników
 * 
 */
extern availableIngredientStruct availableIngredients[NUMBER_OF_INGREDIENTS];

#endif /* INC_AVAILABLEINGREDIENTS_H_ */
