/*
 * USB_File_Handling.h
 *
 *  Created on: Nov 11, 2021
 *      Author: Yo
 */

#ifndef FILE_HANDLING_H_
#define FILE_HANDLING_H_

#include "fatfs.h"
#include "string.h"
#include "stdio.h"

#define SAVING_ERROR 1
#define COULD_NOT_OPEN_THE_FILE 2
#define COULD_NOT_CLOSE_THE_FILE 3
#define DATA_SAVED_PROPERLY 4

uint8_t parseLine(char* line, char* flag, char* name, char* volume);
uint8_t searchTab(char* buf, int8_t* code);
uint8_t readConf(void);
FRESULT closeFile(void);

void Mount_USB (void);
void Unmount_USB (void);
FRESULT Format_USB (void);
FRESULT Write_File (char *name, char *data);
FRESULT openFile (char *name);
FRESULT Create_File (char *name);
FRESULT Remove_File (char *name);

#endif /* FILE_HANDLING_RTOS_H_ */
