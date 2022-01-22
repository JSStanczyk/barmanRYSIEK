/*
 * drinkDataStruct.h
 *
 *  Created on: 16 lis 2021
 *      Author: Yo
 */

#ifndef INC_DRINKDATASTRUCT_H_
#define INC_DRINKDATASTRUCT_H_

#include "stdint.h"

#define LIST_SIZE 50

void initListOfDrinks(void);

/**
 * @brief Struktura zawierająca przepis na jednego drinka
 * 
 */
typedef struct drink{
	char name[50];
	char firstIgName[30];
	uint8_t firstIgVolume;
	char secondIgName[30];
	uint8_t secondIgVolume;
	char thirdIgName[30];
	uint8_t thirdIgVolume;
	char fourthIgName[30];
	uint8_t fourthIgVolume;
	char fivethIgName[30];
	uint8_t fivethIgVolume;

}drink;

/**
 * @brief Lista wszytskich przepisów
 * 
 */
extern drink listOfDrinks[LIST_SIZE];

#endif /* INC_DRINKDATASTRUCT_H_ */
