/*
 * drinkDataStruct.c
 *
 *  Created on: 16 lis 2021
 *      Author: Yo
 */

#include "../Inc/drinkDataStruct.h"

drink listOfDrinks[LIST_SIZE];

/**
 * @brief Funckja inicjalizuje listę dostępnych przepisów na drinki
 * 
 */
void initListOfDrinks(void){
	for(uint8_t i = 0; i < LIST_SIZE; i++){
		listOfDrinks[i].name[0] = '\0';
		listOfDrinks[i].firstIgName[0] = '\0';
		listOfDrinks[i].firstIgVolume = 0;
		listOfDrinks[i].secondIgName[0] ='\0';
		listOfDrinks[i].secondIgVolume = 0;
		listOfDrinks[i].thirdIgName[0] = '\0';
		listOfDrinks[i].thirdIgVolume = 0;
		listOfDrinks[i].fourthIgName[0] = '\0';
		listOfDrinks[i].fourthIgVolume = 0;
		listOfDrinks[i].fivethIgName[0] = '\0';
		listOfDrinks[i].fivethIgVolume = 0;
	}
}

