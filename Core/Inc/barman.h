/*
 * barman.h
 *
 *  Created on: Dec 12, 2021
 *      Author: Yo
 */

#ifndef INC_BARMAN_H_
#define INC_BARMAN_H_

#include <stdint.h>
#include <stm32f411xe.h>

#define NOT_POURING 0
#define POURING 1

void beginMeas(void);
void endMeas(void);
void pourIngredient(uint8_t IgNumber);
void checkPouredVolume(void);
void setEmptyGlassWeight(void);
void makeDrink(char* drinkName);
void findDrinkInMenu(char* drinkName);
void leadDrinkMaking(void);
uint8_t matchAltretismLevel(uint16_t PUMP_PIN);
void checkIfRunOutOfIngredient(void);
uint16_t findPumpCurrent(uint16_t PUMP_PIN);

/**
 * @brief flaga zawierająca status nalewania
 * 
 */
extern uint8_t pouringStatus;

#endif /* INC_BARMAN_H_ */
