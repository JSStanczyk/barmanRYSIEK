/*
 * availableIngredients.c
 *
 *  Created on: 6 sty 2022
 *      Author: Yo
 */

#include <availableIngredients.h>
#include <gpio.h>
#include <menuCard.h>
#include <string.h>
#include <math.h>

availableIngredientStruct availableIngredients[NUMBER_OF_INGREDIENTS];

/**
 * @brief Funkcja inicjalizująca tablicę dostępnych składników
 * 
 */
void availableIngredientsInit(void){
	for(uint8_t i = 0; i < NUMBER_OF_INGREDIENTS; i++){
		availableIngredients[i].IgName[0] = '\0';
		availableIngredients[i].PUMP_PORT = PUMP1_GPIO_Port; // Wszystkie pompki mają ten sam port
		availableIngredients[i].PUMP_PIN = PUMP1_Pin/(pow(2,i)); //Kolejene adresy Pinów 
	}
}

/**
 * @brief Funkcja dodaje składnik do listy dostępnych
 * 
 * @param place miejsce w tablicy, na które ma być wstawiony składnik
 * @param IgName nazwa składnika
 */
void addIngredient(uint8_t place, char* IgName){
	//sprintf(availableIngredients[place-1].IgName, "%s", IgName);
	strcpy(availableIngredients[place-1].IgName, IgName);
	updateActiveMenuCard();
}

/**
 * @brief Funkcja usuwa składnik z listy dostępnych
 * 
 * @param place miejsce w tablicy, z którego ma zostać usunięty składnik
 */
void removeIngredient(uint8_t place){
	availableIngredients[place-1].IgName[0] = '\0';
	updateActiveMenuCard();
}
