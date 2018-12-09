/*
 * uartDecoder.c
 *
 *  Created on: 27 nov. 2018
 *      Author: dcamacho
 */


#include <uartDecoder.h>
#include "stm32l4xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stm32l475e_iot01.h"
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include <math.h>


extern UART_HandleTypeDef huart1;


#ifndef COLOR_PRINT
#define PRINT_GREEN(X,Y) { \
		HAL_UART_Transmit(&huart1, (uint8_t *) "\033[32;1m", 7 ,500); \
		HAL_UART_Transmit(&huart1, X, Y, 500); \
		HAL_UART_Transmit(&huart1, (uint8_t*) "\033[0m", 4 ,500); \
}

#define PRINT_RED(X,Y) { \
		HAL_UART_Transmit(&huart1, (uint8_t *) "\033[31;1m", 7 ,500); \
		HAL_UART_Transmit(&huart1, X, Y, 500); \
		HAL_UART_Transmit(&huart1, (uint8_t*) "\033[0m", 4 ,500); \
}

#define PRINT_BLUE(X,Y) { \
		HAL_UART_Transmit(&huart1, (uint8_t *) "\033[34;1m", 7 ,500); \
		HAL_UART_Transmit(&huart1, X, Y, 500); \
		HAL_UART_Transmit(&huart1, (uint8_t*) "\033[0m", 4 ,500); \
}
#endif

void decoderMsg(uint8_t *string)
{
	char *pos, check = 0;

	pos = strstr((char *) string, (const char *)"MENU1");
	if(pos != NULL)
	{
		HAL_UART_Transmit(&huart1, (uint8_t *) "Menu 1 Activado\n",16,500);
		PRINT_GREEN((uint8_t *) "Opcion 1\n", 9);

		check = 1;
	} // if(Menu1)

	pos = strstr((char *) string, (const char *)"MENU2");
	if(pos != NULL)
	{
		uint8_t array[50], index = 0;
		int num1;
		//(*pos) += 6;
		//strcpy((char *)array, pos);

		do {
			array[index] = (uint8_t)pos[index + 6];
			++index;
		} while ((index < 50) && (array[index-1] > 0x2F) && (array[index-1] < 0x3A));
		num1 = atoi((const char *)array);
		num1 = sprintf((char *) array, "El número es %i\n", num1);
		PRINT_RED((uint8_t *)array, num1+1);

		check = 1;
	} // if(Menu2)

	pos = strstr((char *) string, (const char *) "REBOOT");
	if(pos != NULL)
	{
		HAL_UART_Transmit(&huart1, (uint8_t *)"Esperando WDT\n", 14,300);
		for (int i = 0; i < 20; ++i) {
			int num;
			char array[20];

			HAL_Delay(1000);
			num = sprintf(array, "%i.. ",i);
			HAL_UART_Transmit(&huart1,(uint8_t *) array, num, 500);
		}
	}

	pos = strstr((char *) string, (const char *) "RESET");
	if(pos != NULL)
	{
		HAL_UART_Transmit(&huart1, (uint8_t *)"Reset del sistema\n", 18,300);
		HAL_Delay(1000);
		HAL_NVIC_SystemReset();
	}

	pos = strstr((char *) string, (const char *) "TEMP");
	if(pos != NULL)
	{
		uint8_t tempBuffer[50];
		float temperature;

		temperature = BSP_TSENSOR_ReadTemp();
		int tmpInt1 = temperature;
		float tmpFrac = temperature - tmpInt1;
		int tmpInt2 = trunc(tmpFrac * 100);
		snprintf((char *) tempBuffer,50,"TEMPERATURE = %d.%02d\n", tmpInt1, tmpInt2);
		HAL_UART_Transmit(&huart1, (uint8_t *) tempBuffer, 20, 100);
		HAL_Delay(1000);

		check = 1;
	}

	pos = strstr((char *) string, (const char *) "PRESSURE");
	if(pos != NULL)
	{
		uint8_t tempBuffer[50], pressure;

		pressure = BSP_PSENSOR_ReadPressure();
		snprintf((char *) tempBuffer,50,"PRESSURE = %d\n", pressure);
		HAL_UART_Transmit(&huart1, (uint8_t *) tempBuffer, (uint8_t) strlen(tempBuffer), 100);

		check = 1;
	}


	if(check == 0) PRINT_RED((uint8_t *)"Unrecognized command\n",21);

	PRINT_BLUE((uint8_t *) "#",1);
}




