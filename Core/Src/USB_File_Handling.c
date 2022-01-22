/*
 * USB_File_Handling.c
 *
 *  Created on: Nov 11, 2021
 *      Author: Yo
 */

#include <USB_File_Handling.h>
#include "stm32f4xx_hal.h"
#include "../Inc/drinkDataStruct.h"

#define BAD_SYNTAX 0
#define LINE_COMMENTED 1
#define LINE_PARSE_OK 2
#define LINE_EMPTY 3

#define DRINK 1
#define FIRST_IG 2
#define SECOND_IG 3
#define THIRD_IG 4
#define FOURTH_IG 5
#define FIVETH_IG 6

#define RANGE_EXCEEDED 0

#define PROPER_FLAG 1
#define FLAG_NOT_FOUND 0

extern char USBHPath[4];   /* USBH logical drive path */
extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
extern FIL USBHFile;       /* File object for USBH */

FILINFO USBHfno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

FATFS *pUSBHFatFS;
DWORD fre_clust;
uint32_t total, free_space;

void* flagsTab[] = {
		(void*)"[DRINK]", (void*)DRINK,
		(void*)"[FIRST_IG]", (void*)FIRST_IG,
		(void*)"[SECOND_IG]", (void*)SECOND_IG,
		(void*)"[THIRD_IG]", (void*)THIRD_IG,
		(void*)"[FOURTH_IG]", (void*)FOURTH_IG,
		(void*)"[FIVETH_IG]", (void*)FIVETH_IG,
		(void*)"[EOF]", (void*)EOF,
		(void*)""
	};

/**
 * @brief Funkcja montuje dysk logiczny USB
 * 
 */
void Mount_USB (void)
{
	fresult = f_mount(&USBHFatFS, USBHPath, 1);
}

/**
 * @brief Funkcja odmontowuje dysk logiczny USB
 * 
 */
void Unmount_USB (void)
{
	fresult = f_mount(NULL, USBHPath, 1);
}

// /* Only supports removing files from home directory */
// FRESULT Format_USB (void)
// {
//     DIR dir;
//     char *path = "/";//malloc(20*sizeof (char));
// //    sprintf (path, "%s","/");

//     fresult = f_opendir(&dir, path);                       /* Open the directory */
//     if (fresult == FR_OK)
//     {
//         for (;;)
//         {
//             fresult = f_readdir(&dir, &USBHfno);                   /* Read a directory item */
//             if (fresult != FR_OK || USBHfno.fname[0] == 0) break;  /* Break on error or end of dir */
//             if (USBHfno.fattrib & AM_DIR)     /* It is a directory */
//             {
//             	if (!(strcmp ("SYSTEM~1", USBHfno.fname))) continue;
//             	if (!(strcmp("System Volume Information", USBHfno.fname))) continue;
//             	fresult = f_unlink(USBHfno.fname);
//             	if (fresult == FR_DENIED) continue;
//             }
//             else
//             {   /* It is a file. */
//                fresult = f_unlink(USBHfno.fname);
//             }
//         }
//         f_closedir(&dir);
//     }
//     //free(path);
//     return fresult;
// }

/**
 * @brief Fukncja zapisuje dane do pliku
 * 
 * @param name nazwa pliku
 * @param data dane do zapisania
 * @return FRESULT 
 */
FRESULT Write_File (char *name, char *data)
{
	fresult = f_stat (name, &USBHfno);
	if (fresult != FR_OK)
	    return fresult;

	fresult = f_open(&USBHFile, name, FA_OPEN_EXISTING | FA_WRITE);
	if (fresult != FR_OK)
	    return fresult;

	fresult = f_write(&USBHFile, data, strlen(data), &bw);

	fresult = f_close(&USBHFile);

	return fresult;
}

/**
 * @brief Funkcja otwiera plik do odczytu
 * 
 * @param name nazwa pliku
 * @return FRESULT 
 */
FRESULT openFile (char *name)
{
	fresult = f_stat (name, &USBHfno);
	if (fresult != FR_OK)
	    return fresult;

	fresult = f_open(&USBHFile, name, FA_READ);
	return fresult;
}

/**
 * @brief Funkcja zamyka łącze do pliku
 * 
 * @return FRESULT 
 */
FRESULT closeFile(void){
	fresult = f_close(&USBHFile);
	return fresult;
}

/**
 * @brief Fukcja tworzy nowy plik i otwiera go  w trybie zapisu i odczytu
 * 
 * @param name nazwa pliku
 * @return FRESULT 
 */
FRESULT Create_File (char *name)
{
	fresult = f_stat (name, &USBHfno);
	if (fresult == FR_OK)
	    return fresult;

	fresult = f_open(&USBHFile, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
	if (fresult != FR_OK)
		return fresult;

	fresult = f_close(&USBHFile);

    return fresult;
}

/**
 * @brief Fukncja otiwera plik w trybie dopisywania i zapisuje w nim dane
 * 
 * @param name nzawa pliku
 * @param data dane do zapisania
 * @return FRESULT 
 */
FRESULT Update_File (char *name, char *data)
{
	fresult = f_stat (name, &USBHfno);
	if (fresult != FR_OK)
	    return fresult;

	fresult = f_open(&USBHFile, name, FA_OPEN_APPEND | FA_WRITE);
	if (fresult != FR_OK)
	    return fresult;

	fresult = f_write(&USBHFile, data, strlen (data), &bw);

	fresult = f_close(&USBHFile);

    return fresult;
}

/**
 * @brief Funkcja parsuje linię pobraną z pliku konfiguracyjnego
 * 
 * @param line pobrana linia
 * @param flag odczytana z linik laga identyfikacyjna
 * @param name nazwa odczytana z linii
 * @param volume objętość odczytana z linii
 * @return uint8_t  status parsowania linii
 */
uint8_t parseLine(char* line, char* flag, char* name, char* volume){

	#define checkSyntax(ch) if(ch == '\n') return BAD_SYNTAX;

	uint8_t flagReaded = 0;

	if(*line == '/' && *(line+1) == '/')
		return LINE_COMMENTED;

	while(*line != '\n' || *line != '\0'){
		while(*line == ' ' || *line == '\t' || *line == ')' || *line == '}'){line++;}

		if(*line == '\n' || *line == '\0')
			break;

		if(*line == '['){
			while(*(line-1) != ']'){
				checkSyntax(*line);
				*flag = *line;
				flag++;line++;
			}
			*(flag) = '\0';
			flagReaded++;

		} else if(*line == '{'){
			line++;
			while(*line != '|'){
				checkSyntax(*line);
				*name = *line;
				name++;line++;
			}

			line++;
			*(name) = '\0';

			while(*line != '}'){
				checkSyntax(*line);
				*volume = *line;
				volume++;line++;
			}

			*(volume) = '\0';
		} else if(*line == '('){
			line++;
			while(*line != ')'){
				checkSyntax(*line);
				*name = *line;
				name++;line++;
			}

			*(name) = '\0';
		}
	}
	if(flagReaded) return LINE_PARSE_OK;
	else return LINE_EMPTY;
}

/**
 * @brief Funkcja zarządza wczytywaniem dananych z pliku konfiguracyjnego i wpisywaniem ich do odpowienich struktur danych
 * 
 * @return uint8_t 
 */
uint8_t readConf(void){
	#define LINE_LENGTH 200
	#define FLAG_LENGTH 30
	#define NAME_LENGTH 40
	#define VOLUME_LENGTH 5

	char flag[FLAG_LENGTH], line[LINE_LENGTH], name[NAME_LENGTH], volume[VOLUME_LENGTH];
	int8_t code = 0;
	uint8_t drinksCounter = 0;

	initListOfDrinks();

	if(openFile("/listOfDrinks.txt") != FR_OK) return COULD_NOT_OPEN_THE_FILE;

	while(1){
		f_gets(line, LINE_LENGTH, &USBHFile);

		if(parseLine(line, flag, name, volume) == LINE_PARSE_OK){
			if(searchTab(flag, &code) == PROPER_FLAG){
				switch(code){

					case DRINK:
						if(++drinksCounter > LIST_SIZE) return RANGE_EXCEEDED;
						sprintf(listOfDrinks[drinksCounter-1].name, "%s", name);
						break;

					case FIRST_IG:
						sprintf(listOfDrinks[drinksCounter-1].firstIgName, "%s", name);
						listOfDrinks[drinksCounter-1].firstIgVolume = atoi(volume);
						break;

					case SECOND_IG:
						sprintf(listOfDrinks[drinksCounter-1].secondIgName, "%s", name);
						listOfDrinks[drinksCounter-1].secondIgVolume = atoi(volume);
						break;

					case THIRD_IG:
						sprintf(listOfDrinks[drinksCounter-1].thirdIgName, "%s", name);
						listOfDrinks[drinksCounter-1].thirdIgVolume = atoi(volume);
						break;

					case FOURTH_IG:
						sprintf(listOfDrinks[drinksCounter-1].fourthIgName, "%s", name);
						listOfDrinks[drinksCounter-1].fourthIgVolume = atoi(volume);
						break;

					case FIVETH_IG:
						sprintf(listOfDrinks[drinksCounter-1].fivethIgName, "%s", name);
						listOfDrinks[drinksCounter-1].fivethIgVolume = atoi(volume);
						break;

					case EOF:
						if(closeFile() != FR_OK) return COULD_NOT_OPEN_THE_FILE;
						return DATA_SAVED_PROPERLY;

					default:
						return SAVING_ERROR;
				}
			}
		}
	}
}

/**
 * @brief Funkcja przeszukuje tablicę flag indentyfikacyjnych, zwraca czy dana flaga jest obsługiwana oraz jeśli jest to zwraca jej kod
 * 
 * @param buf flaga szukana
 * @param code wskaźnik do zmiennej na kod flagi
 * @return uint8_t 
 */
uint8_t searchTab(char* buf, int8_t* code){
	uint8_t i = 0;

	while(1){
		if(strcmp(buf, (char*)flagsTab[i]) == 0){
			*code = (int8_t)flagsTab[i+1];
			return PROPER_FLAG;

		} else if(strcmp(flagsTab[i], "") == 0){
			break;
		}
		i+=2;
	}
	return FLAG_NOT_FOUND;
}


//FRESULT Remove_File (char *name)
//{
//	/**** check whether the file exists or not ****/
//	fresult = f_stat (name, &USBHfno);
//	if (fresult != FR_OK)
//	{
//		char *buf = malloc(100*sizeof(char));
//		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
//		Send_Uart (buf);
//		free(buf);
//		return fresult;
//	}
//
//	else
//	{
//		fresult = f_unlink (name);
//		if (fresult == FR_OK)
//		{
//			char *buf = malloc(100*sizeof(char));
//			sprintf (buf, "*%s* has been removed successfully\n\n", name);
//			Send_Uart (buf);
//			free(buf);
//		}
//
//		else
//		{
//			char *buf = malloc(100*sizeof(char));
//			sprintf (buf, "ERROR No. %d in removing *%s*\n\n",fresult, name);
//			Send_Uart (buf);
//			free(buf);
//		}
//	}
//	return fresult;
//}