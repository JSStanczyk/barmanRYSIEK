/*
 * timerMenager.h
 *
 *  Created on: 17 sty 2022
 *      Author: Yo
 */

#ifndef INC_TIMERMENAGER_H_
#define INC_TIMERMENAGER_H_

extern uint8_t typeOfMeas;

void startMeasurementTimer(void);
void stopMeasurementTimer(void);
void startDrinkMakeTimer(void);
void stopDrinkMakeTimer(void);
void startCommMainBufCheckTimer(void);
void stopCommMainBufCheckTimer(void);

#endif /* INC_TIMERMENAGER_H_ */
