/*
 * menuCard.h
 *
 *  Created on: 6 sty 2022
 *      Author: Yo
 */

#ifndef INC_MENUCARD_H_
#define INC_MENUCARD_H_

#include <drinkDataStruct.h>
#include <availableIngredients.h>

void updateActiveMenuCard(void);
void activeMenuCardInit(void);
void checkNumberOfIgToMatch(uint8_t* numberOfIgToMatch, drink listOfDrinksElem);
void matchIngredients(uint8_t* numberOfIgToMatch, uint8_t* menuElemCount, drink* listOfDrinksElem);

/**
 * @brief Karta drinków możliwych do zrobienia przy aktualnie dostępnuch składnikach
 * 
 */
extern drink* activMenuCard[LIST_SIZE];

#endif /* INC_MENUCARD_H_ */
