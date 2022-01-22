/*
 * menuCard.c
 *
 *  Created on: 6 sty 2022
 *      Author: Yo
 */

#include <menuCard.h>
#include <string.h>

drink* activMenuCard[LIST_SIZE];

/**
 * @brief Funkcja aktualizuje zawartość karty menu
 * 
 */
void updateActiveMenuCard(void){
	uint8_t numberOfIgToMatch = 0, menuElemCount = 0;
	activeMenuCardInit();

	for(uint8_t i = 0; i < LIST_SIZE; i++){
		if(listOfDrinks[i].name[0] == '\0') break;

		checkNumberOfIgToMatch(&numberOfIgToMatch, listOfDrinks[i]);
		matchIngredients(&numberOfIgToMatch, &menuElemCount, &listOfDrinks[i]);
	}
}

/**
 * @brief Funckja szuka dopasowania między dostępnymi składnikami, a każdym z przepisów. Jeśli wszystkie składniki z przepisu
 * są pokryte to wpisuje wskaźnik na ten drink do karty menu.
 * 
 * @param numberOfIgToMatch liczba składników z danego przepisu do pokrycia
 * @param menuElemCount indeks wskaźnik na index karty menu
 * @param listOfDrinksElem wskażnik na aktualnie sprawdzny przepis
 */
void matchIngredients(uint8_t* numberOfIgToMatch, uint8_t* menuElemCount, drink* listOfDrinksElem){
	for(uint8_t i = 0; i < NUMBER_OF_INGREDIENTS; i++){
		if(strcmp(availableIngredients[i].IgName, listOfDrinksElem->firstIgName) == 0 && listOfDrinksElem->firstIgName[0] != '\0'){
			(*numberOfIgToMatch)--;

		} else if(strcmp(availableIngredients[i].IgName, listOfDrinksElem->secondIgName) == 0 && listOfDrinksElem->secondIgName[0] != '\0'){
			(*numberOfIgToMatch)--;

		} else if(strcmp(availableIngredients[i].IgName, listOfDrinksElem->thirdIgName) == 0 && listOfDrinksElem->thirdIgName[0] != '\0'){
			(*numberOfIgToMatch)--;

		} else if(strcmp(availableIngredients[i].IgName, listOfDrinksElem->fourthIgName) == 0 && listOfDrinksElem->fourthIgName[0] != '\0'){
			(*numberOfIgToMatch)--;

		} else if(strcmp(availableIngredients[i].IgName, listOfDrinksElem->fivethIgName) == 0 && listOfDrinksElem->fivethIgName[0] != '\0'){
			(*numberOfIgToMatch)--;
		}
	}

	if(*numberOfIgToMatch == 0){
		activMenuCard[*menuElemCount] = listOfDrinksElem;
	}
}

/**
 * @brief Funkcja sprawdza ile składników jest do pokrycia w danym przepisie
 * 
 * @param numberOfIgToMatch liczba składników z danego przepisu do pokrycia
 * @param listOfDrinksElem aktualnie sprawdzny przepis
 */
void checkNumberOfIgToMatch(uint8_t* numberOfIgToMatch, drink listOfDrinksElem){
	*numberOfIgToMatch = 0;

	if(listOfDrinksElem.firstIgName[0] != '\0'){
		(*numberOfIgToMatch)++;
	}

	if(listOfDrinksElem.secondIgName[0] != '\0'){
		(*numberOfIgToMatch)++;
	}

	if(listOfDrinksElem.thirdIgName[0] != '\0'){
		(*numberOfIgToMatch)++;
	}

	if(listOfDrinksElem.fourthIgName[0] != '\0'){
		(*numberOfIgToMatch)++;
	}

	if(listOfDrinksElem.fivethIgName[0] != '\0'){
		(*numberOfIgToMatch)++;
	}
}

/**
 * @brief Funkcja inicjalizuje kartę menu
 * 
 */
void activeMenuCardInit(void){
	for(uint8_t i = 0; i < LIST_SIZE; i++){
		activMenuCard[i] = NULL;
	}
}
