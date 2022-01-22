/*
 * comm.h
 *
 *  Created on: Jan 7, 2022
 *      Author: Yo
 */

#ifndef INC_COMM_H_
#define INC_COMM_H_

#include <stdint.h>

void decodeMessage(uint8_t* message);
void interpretOrder(void);
void startCheckingForData(void);
void receiveData(uint16_t Size);
void getMessageFromMainBuf(void);
uint8_t* checkIfThereIsFullMessage(void);
void sendMessageByBluetooth(char* message, uint16_t* message_size);
void sendMessageByService(char* message, uint16_t* message_size);

#endif /* INC_COMM_H_ */
