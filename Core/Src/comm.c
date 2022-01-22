/*
 * comm.c
 *
 *  Created on: Jan 7, 2022
 *      Author: Yo
 */

#include <stdint.h>
#include <usart.h>
#include <string.h>
#include <commands.h>
#include <barman.h>
#include <availableIngredients.h>
#include <comm.h>
#include <stdio.h>
#include <stdlib.h>
#include <usart.h>

#define RxBuf_SIZE 50
#define Message_SIZE 50
#define MainCircBuf_SIZE 200

uint8_t order, orderParam[50], availableIgPlace, mes[50];
uint16_t mes_size = 0;

uint8_t rxBuf[RxBuf_SIZE];
uint8_t mainCircBuf[MainCircBuf_SIZE];
uint8_t message[Message_SIZE];

uint8_t *readPtr = mainCircBuf, *writePtr = mainCircBuf;

/**
 * @brief Fukcnja dekoduje wiadomość tak aby była gotowa do zinterpretowania
 * 
 * @param message wiadomość
 */
void decodeMessage(uint8_t* message){
	char* LFPtr;

	order = *message;
	availableIgPlace = atoi((message+1));
	strcpy(orderParam, message + 2);
	LFPtr = strchr(orderParam, '\n');
	*LFPtr = '\0';

	interpretOrder();
}

/**
 * @brief Fukncja interpretuje wiadomość i wykonuje odpowiednie podfukcnje
 * 
 */
void interpretOrder(void){

	switch(order){
		case SET_AVAILABLE_IGREDNIENT:
			addIngredient(availableIgPlace, orderParam);
			mes_size = sprintf(mes, "%s%s\t%s%d\n\r", "Added Ingredient: ", orderParam, "On Place: ", availableIgPlace);
			sendMessageByBluetooth(mes, &mes_size);
			break;

		case REMOVE_AVAILABLE_IGREDNIENT:
			removeIngredient(availableIgPlace);
			mes_size = sprintf(mes, "%s%d\n\r", "Ingredient Removed From Place: ", availableIgPlace);
			sendMessageByBluetooth(mes, &mes_size);
			break;

		case MAKE_DRINK:
			makeDrink(orderParam);
			break;

		case STOP_MAKING_DRINK:
			stopMakingDrink();
			break;

		default:
			mes_size = sprintf(mes, "%s", "Unrecognized order. Repeat message.\n\r");
			HAL_UART_Transmit_IT(&huart1, mes, mes_size);
			sendMessageByBluetooth(mes, &mes_size);
//			mes_size = sprintf(mes, "%X\n\r", REPEIT_MESSAGE);
//			HAL_UART_Transmit_IT(&huart1, mes, mes_size);
			break;
	}
}

/**
 * @brief Funkcja służy do inicjacji odbioru UART
 * 
 */
void startCheckingForData(void){
	HAL_UARTEx_ReceiveToIdle_IT(&huart1, rxBuf, RxBuf_SIZE);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	receiveData(Size);
	startCheckingForData();
}

/**
 * @brief Funkcja zapisuje odebrane z UARTA dane i zapisuje je do bufora cyklicznego
 * 
 * @param Size wielkość odebranej paczki danych
 */
void receiveData(uint16_t Size){
	if((writePtr + Size) > (mainCircBuf + MainCircBuf_SIZE)){
		uint16_t remainingSpace = (mainCircBuf + MainCircBuf_SIZE) - writePtr;

		memcpy(writePtr, rxBuf, remainingSpace);
		writePtr = mainCircBuf;

		snprintf(writePtr, (Size - remainingSpace) + 1, "%s", rxBuf + remainingSpace);
		writePtr += (Size-remainingSpace);

	} else if((writePtr + Size) <= (mainCircBuf + MainCircBuf_SIZE)){
		snprintf(writePtr, Size + 1, "%s", rxBuf);
		writePtr += Size + 1;
	}
}

/**
 * @brief Funkcja pobiera pojedynczą wiadomość z bufora cyklicznego. Będzie to powtarzać do momętu zrównania się wskaźników
 * odczytu i zapisu. Daje to rodzaj kolekowania poleceń.
 * 
 */
void getMessageFromMainBuf(void){

	if(readPtr != writePtr){
		uint8_t* endMesPtr = checkIfThereIsFullMessage();

		if(*endMesPtr == '\0'){

			strcpy(message, readPtr);
			readPtr = endMesPtr + 1;


		} else if(*endMesPtr != '\0'){


			memcpy(message, readPtr, (endMesPtr - readPtr));
			readPtr = mainCircBuf;
			endMesPtr = checkIfThereIsFullMessage();

			strcpy(message, readPtr);
			readPtr = endMesPtr + 1;
		}
		decodeMessage(message);
	}
}

/**
 * @brief Funkcja sprawdza czy do końca bufora cyklicznego znajduje się pełna wiadomość czy została zawinięta
 * 
 * @return uint8_t* zwraca wskaźnik na koniec wiadomości jeśli ją znajdzie lub wskaźnik odczytu jeśli nie.
 */
uint8_t* checkIfThereIsFullMessage(void){
	uint8_t* search = readPtr;

	while((search < (mainCircBuf + MainCircBuf_SIZE))){
		search++;
		if(*search == '\0'){
			return search;
		}
	}
	return search;
}

/**
 * @brief Funkcja służy do wysłania wiadomości przez bluetooth
 * 
 * @param message wiadomość
 * @param message_size rozmiar wiadomości
 */
void sendMessageByBluetooth(char* message, uint16_t* message_size){
	HAL_UART_Transmit_IT(&huart1, message, *message_size);
}

/**
 * @brief Funkcja służy do wysłania wiadomości przez złącze serwisowe
 * 
 * @param message wiadomość
 * @param message_size rozmiar wiadomości
 */
void sendMessageByService(char* message, uint16_t* message_size){
	HAL_UART_Transmit_IT(&huart2, message, *message_size);
}
