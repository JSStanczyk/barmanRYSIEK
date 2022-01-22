/*
 * barman.c
 *
 *  Created on: Dec 12, 2021
 *      Author: Yo
 */

#include <barman.h>
#include <drinkDataStruct.h>
#include <menuCard.h>
#include <measurementStruct.h>
#include <availableIngredients.h>
#include <currentMeasurements.h>
#include <string.h>
#include <timerMenager.h>

#define GLASS_SCALE_LIMIT 1000

/**
 * @brief Struktura przechowująca parametry potrzebne podczas nalewania składnika
 * 
 */
typedef struct{
	GPIO_TypeDef* PUMP_PORT;
	uint16_t PUMP_PIN;
	uint8_t altretismLevel;
	uint8_t IgVolume;
}barmanHandParam;

uint8_t altretismLevelArray[5] = {9, 9, 10, 0, 0}; /** Tablia współczynników wyrównujących ilość nalewanego składnika*/

barmanHandParam barmanHandParameters;
uint8_t pouringStatus = 0;
drink* drinkToMake; /** wskaźnika na aktualnie komponowany drink*/
uint16_t glassWeight = 0;

/**
 * @brief Funkcja rozpoczynająca pomiary prądu oraz wagi szklanki
 * 
 */
void beginMeas(void){
	startMeasurementTimer();
}

/**
 * @brief Funkcja kończąca pomiary prądu oraz wagi szklanki
 * 
 */
void endMeas(void){
	stopMeasurementTimer();
}

/**
 * @brief Funkcja odpowiedzialna na poprwne rozpoczęcie nalewania składnika.
 * Jeśli numer aktualnie nalewanego składnika jest 1 czyli robienie drinka dopiero się rozpoczęło, to
 * flaga typeOfMeas wskazuje pomiar wagi pustej szklanki. Jeśli nie to poprostu rozpoczyna się nalewanie.
 * 
 * @param IgNumber numer składnika w przepisie
 */
void pourIngredient(uint8_t IgNumber){
	if(IgNumber == 1) typeOfMeas = EMPTY_GLASS_WEIGHT_MEAS;
	else HAL_GPIO_WritePin(barmanHandParameters.PUMP_PORT, barmanHandParameters.PUMP_PIN, GPIO_PIN_RESET);
	pouringStatus = POURING;
	beginMeas();
}

/**
 * @brief Fukncja monitorująca stan nlewania składnika
 * 
 */
void checkPouredVolume(void){
	if(meas.glassScaleValue >= glassWeight+barmanHandParameters.IgVolume-barmanHandParameters.altretismLevel || meas.glassScaleValue >= GLASS_SCALE_LIMIT){
		HAL_GPIO_WritePin(barmanHandParameters.PUMP_PORT, barmanHandParameters.PUMP_PIN, GPIO_PIN_SET);
		endMeas();
		glassWeight += barmanHandParameters.IgVolume;
		pouringStatus = NOT_POURING;
	}
}

/**
 * @brief Funkcja sprawdzająca na podstawie prądu pobieranego przez pompkę czy w zasobniku jest jeszcz co nalewać
 * 
 */
void checkIfRunOutOfIngredient(void){
	if(findPumpCurrent(barmanHandParameters.PUMP_PIN) < 20){
		HAL_GPIO_WritePin(barmanHandParameters.PUMP_PORT, barmanHandParameters.PUMP_PIN, GPIO_PIN_SET);
		stopMakingDrink();
	}
}

/**
 * @brief Funkcja odpowiedzialna ustawienie wagi pustej szklanki. Robi to na zasadzie średniej z 10 odczytów z beli tensometrycznej,
 * tak aby dać czas użytkownikowi na swobodne jej odstawienie oraz wyeliminować problem związany z którkotrwałym i mocnym naciskiem na
 * wagę. Przepuści tylko wagę równą lub większą 200 gram poniważ takie szklanki powinny pomieścić każdego drinka.
 * 
 */
void setEmptyGlassWeight(void){
	static uint16_t mean = 0;
	static uint8_t counter = 0;

	if(meas.glassScaleValue < 0 || meas.glassScaleValue > 500) return;
	mean += meas.glassScaleValue;
	counter++;

	if(mean/counter >= 200 && counter == 10){
		mean = 0;
		counter = 0;
		glassWeight = meas.glassScaleValue;
		typeOfMeas = POURED_LIQUID_WEIGHT_MEAS;
		HAL_GPIO_WritePin(barmanHandParameters.PUMP_PORT, barmanHandParameters.PUMP_PIN, GPIO_PIN_RESET);

	} else if(counter > 10){
		mean = 0;
		counter = 0;
	}
}

/**
 * @brief Fukncja inicjuej robienie drinka
 * 
 * @param drinkName Nazwa drinka do zrobienia
 */
void makeDrink(char* drinkName){
	findDrinkInMenu(drinkName);
	startDrinkMakeTimer();
	leadDrinkMaking();
}

/**
 * @brief Funkcja zatrzymuje robienie drinka. Bez zapisania stopnia zaawansowania.
 * 
 */
void stopMakingDrink(void){
	endMeas();
	stopDrinkMakeTimer();
}

/**
 * @brief Funkcja zdnajduje drink w karcie menu przypisuje wskaźnik
 * 
 * @param drinkName Nazwa drinka do znalezienia
 */
void findDrinkInMenu(char* drinkName){

	for(uint8_t i = 0; i < LIST_SIZE; i++){
		if(activMenuCard[i] == NULL) return;
		if(strcmp(activMenuCard[i]->name, drinkName) == 0){
			drinkToMake = activMenuCard[i];
			break;
		}
	}
}

/**
 * @brief Fukncja odpowiedzialna za przeprowadzenie procedury przygotowania drinka. Sprawdza czy akurat nie jest nawelany składnik.
 * Następnie sprawdza czy akutlnie nstepny do nalania składnik istnieje. Szuka do której pompki jest przypisany potrzebny składnik
 * i rozpoczyna nalewanie.
 * 
 */
void leadDrinkMaking(void){
	static uint8_t IgNumber = 1;

	if(pouringStatus == NOT_POURING){
		if(IgNumber == 1){
			if(drinkToMake->firstIgName[0] == '\0'){
				IgNumber = 1;
				stopDrinkMakeTimer();
				return;
			}

			for(uint8_t i = 0; i < NUMBER_OF_INGREDIENTS; i++){
				if(strcmp(availableIngredients[i].IgName, drinkToMake->firstIgName) == 0){
					barmanHandParameters.PUMP_PORT = availableIngredients[i].PUMP_PORT;
					barmanHandParameters.PUMP_PIN = availableIngredients[i].PUMP_PIN;
					barmanHandParameters.altretismLevel = matchAltretismLevel(barmanHandParameters.PUMP_PIN);
					barmanHandParameters.IgVolume = drinkToMake->firstIgVolume;
					break;
				}
			}
			pourIngredient(IgNumber);

		} else if(IgNumber == 2){
			if(drinkToMake->secondIgName[0] == '\0'){
				IgNumber = 1;
				stopDrinkMakeTimer();
				return;
			}

			for(uint8_t i = 0; i < NUMBER_OF_INGREDIENTS; i++){
				if(strcmp(availableIngredients[i].IgName, drinkToMake->secondIgName) == 0){
					barmanHandParameters.PUMP_PORT = availableIngredients[i].PUMP_PORT;
					barmanHandParameters.PUMP_PIN = availableIngredients[i].PUMP_PIN;
					barmanHandParameters.altretismLevel = matchAltretismLevel(barmanHandParameters.PUMP_PIN);
					barmanHandParameters.IgVolume = drinkToMake->secondIgVolume;
					break;
				}
			}
			pourIngredient(IgNumber);

		} else if(IgNumber == 3){
			if(drinkToMake->thirdIgName[0] == '\0'){
				IgNumber = 1;
				stopDrinkMakeTimer();
				return;
			}

			for(uint8_t i = 0; i < NUMBER_OF_INGREDIENTS; i++){
				if(strcmp(availableIngredients[i].IgName, drinkToMake->thirdIgName) == 0){
					barmanHandParameters.PUMP_PORT = availableIngredients[i].PUMP_PORT;
					barmanHandParameters.PUMP_PIN = availableIngredients[i].PUMP_PIN;
					barmanHandParameters.altretismLevel = matchAltretismLevel(barmanHandParameters.PUMP_PIN);
					barmanHandParameters.IgVolume = drinkToMake->thirdIgVolume;
					break;
				}
			}
			pourIngredient(IgNumber);

		} else if(IgNumber == 4){
			if(drinkToMake->fourthIgName[0] == '\0'){
				IgNumber = 1;
				stopDrinkMakeTimer();
				return;
			}

			for(uint8_t i = 0; i < NUMBER_OF_INGREDIENTS; i++){
				if(strcmp(availableIngredients[i].IgName, drinkToMake->fourthIgName) == 0){
					barmanHandParameters.PUMP_PORT = availableIngredients[i].PUMP_PORT;
					barmanHandParameters.PUMP_PIN = availableIngredients[i].PUMP_PIN;
					barmanHandParameters.altretismLevel = matchAltretismLevel(barmanHandParameters.PUMP_PIN);
					barmanHandParameters.IgVolume = drinkToMake->fourthIgVolume;
					break;
				}
			}
			pourIngredient(IgNumber);

		} else if(IgNumber == 5){
			if(drinkToMake->fivethIgName[0] == '\0'){
				IgNumber = 1;
				stopDrinkMakeTimer();
				return;
			}

			for(uint8_t i = 0; i < NUMBER_OF_INGREDIENTS; i++){
				if(strcmp(availableIngredients[i].IgName, drinkToMake->fivethIgName) == 0){
					barmanHandParameters.PUMP_PORT = availableIngredients[i].PUMP_PORT;
					barmanHandParameters.PUMP_PIN = availableIngredients[i].PUMP_PIN;
					barmanHandParameters.altretismLevel = matchAltretismLevel(barmanHandParameters.PUMP_PIN);
					barmanHandParameters.IgVolume = drinkToMake->fivethIgVolume;
					break;
				}
			}
			pourIngredient(IgNumber);
		}
		IgNumber++;

		if(IgNumber > 5){
			IgNumber = 1;
			stopDrinkMakeTimer();
		}
	}
}

/**
 * @brief Fukncja dopsowuje współczynnik skalujący do odpowiedniej pompki
 * 
 * @param PUMP_PIN adres pinu pompki
 * @return uint8_t odpowiednia wartość z tablicy współczynników
 */
uint8_t matchAltretismLevel(uint16_t PUMP_PIN){
	if(PUMP_PIN == PUMP1_Pin){
		return altretismLevelArray[0];
	} else if(PUMP_PIN == PUMP2_Pin){
		return altretismLevelArray[1];
	} else if(PUMP_PIN == PUMP3_Pin){
		return altretismLevelArray[2];
	} else if(PUMP_PIN == PUMP4_Pin){
		return altretismLevelArray[3];
	} else if(PUMP_PIN == PUMP5_Pin){
		return altretismLevelArray[4];
	}
	return 0;
}

/**
 * @brief Fukcja znajduje w strukturze pomiarów prąd pąpki która aktualnie pompuje składnik
 * 
 * @param PUMP_PIN adres pinu pompki
 * @return uint16_t znaleziona wartość prądu
 */
uint16_t findPumpCurrent(uint16_t PUMP_PIN){
	if(PUMP_PIN == PUMP1_Pin){
		return meas.pump1Current;
	} else if(PUMP_PIN == PUMP2_Pin){
		return meas.pump2Current;
	} else if(PUMP_PIN == PUMP3_Pin){
		return meas.pump3Current;
	} else if(PUMP_PIN == PUMP4_Pin){
		return meas.pump4Current;
	} else if(PUMP_PIN == PUMP5_Pin){
		return meas.pump5Current;
	}
	return 0;
}
