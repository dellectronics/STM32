/*
 * uartDecoder.c
 *
 *  Created on: 27 nov. 2018
 *      Author: dcamacho
 */

// Includes

#include <uartDecoder.h>
//#include "stm32l4xx_hal.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef sensors
#include "stm32l475e_iot01.h"
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_psensor.h"
#endif

#ifdef MOTORS
#include <L6470_Driver.h>
#endif

#include <math.h>


#ifndef COLOR_PRINT
#define PRINT_GREEN(X,Y) { \
		HAL_UART_Transmit(&uartP, (uint8_t *) "\033[32;1m", 7 ,500); \
		HAL_UART_Transmit(&uartP, X, Y, 500); \
		HAL_UART_Transmit(&uartP, (uint8_t*) "\033[0m", 4 ,500); \
}

#define PRINT_RED(X,Y) { \
		HAL_UART_Transmit(&uartP, (uint8_t *) "\033[31;1m", 7 ,500); \
		HAL_UART_Transmit(&uartP, X, Y, 500); \
		HAL_UART_Transmit(&uartP, (uint8_t*) "\033[0m", 4 ,500); \
}

#define PRINT_BLUE(X,Y) { \
		HAL_UART_Transmit(&uartP, (uint8_t *) "\033[34;1m", 7 ,500); \
		HAL_UART_Transmit(&uartP, X, Y, 500); \
		HAL_UART_Transmit(&uartP, (uint8_t*) "\033[0m", 4 ,500); \
}
#endif

/*
		Variables
*/

extern UART_HandleTypeDef uartP;
static char rxChar, rxChar2;
static char handlerUart[200], rxBuffer[200];
static uint16_t rxIndex, handlerUartFlag;


// UART Interrupt
void  HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if (UartHandle->Instance == USART)
	{
		if ((rxChar == ('\r')) || (rxChar == ('\n')))
		{
			rxBuffer[rxIndex] = rxChar;
			HAL_UART_Transmit(&uartP, &rxChar, 1, 500);
			for (int a = 0; a < rxIndex + 1; ++a) {
				handlerUart[a] = rxBuffer[a];
			}
			handlerUartFlag = 1;
			rxIndex = 0;
		}
		else if ((rxChar == '\b') && rxIndex > 0) --rxIndex;

		else

		{
			if ((rxChar > 0x60) && (rxChar < 0x7A))rxChar -= 0x20;
			rxBuffer[rxIndex] = rxChar;
			//HAL_UART_Transmit(&huart1,(uint8_t *) WHITE_TEXT1,6,500);
			HAL_UART_Transmit(&uartP, &rxBuffer[rxIndex], 1, 200);
			//HAL_UART_Transmit(&huart1,(uint8_t *) WHITE_TEXT2,6,500);
#ifdef LED
			HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
#endif
			++rxIndex;
		}
		HAL_UART_Receive_IT(&uartP, &rxChar, 1);

	}

}

// Init Uart Decode module
void initUartDecode(void)
{
	HAL_UART_Receive_IT(&uartP, &rxChar, 1);
	rxIndex = 0;
	handlerUartFlag = 0;
}

// Decode Message
uint8_t decoderMsg(uint8_t *string)
{
	char *pos, check = 0;

	pos = strstr((char *) string, (const char *)"MENU1");
	if (pos != NULL)
	{
		HAL_UART_Transmit(&uartP, (uint8_t *) "Menu 1 Activado\n", 16, 500);
		PRINT_GREEN((uint8_t *) "Opcion 1\n", 9);

		check = 1;
	} // if(Menu1)

	pos = strstr((char *) string, (const char *)"MENU2");
	if (pos != NULL)
	{
		uint8_t array[50], index = 0;
		int num1;
		//(*pos) += 6;
		//strcpy((char *)array, pos);

		do {
			array[index] = (uint8_t)pos[index + 6];
			++index;
		} while ((index < 50) && (array[index - 1] > 0x2F) && (array[index - 1] < 0x3A));
		num1 = atoi((const char *)array);
		num1 = sprintf((char *) array, "El número es %i\n", num1);
		PRINT_RED((uint8_t *)array, num1 + 1);

		check = 1;
	} // if(Menu2)

	pos = strstr((char *) string, (const char *) "REBOOT");
	if (pos != NULL)
	{
		HAL_UART_Transmit(&uartP, (uint8_t *)"Esperando WDT\n", 14, 300);
		for (int i = 0; i < 20; ++i) {
			int num;
			char array[20];

			HAL_Delay(1000);
			num = sprintf(array, "%i.. ", i);
			HAL_UART_Transmit(&uartP, (uint8_t *) array, num, 500);
		}
	}

	pos = strstr((char *) string, (const char *) "RESET");
	if (pos != NULL)
	{
		HAL_UART_Transmit(&uartP, (uint8_t *)"Reset del sistema\n", 18, 300);
		HAL_Delay(1000);
		HAL_NVIC_SystemReset();
	}

#ifdef SENSORS
	pos = strstr((char *) string, (const char *) "TEMP");
	if (pos != NULL)
	{
		uint8_t tempBuffer[50];
		float temperature;

		temperature = BSP_TSENSOR_ReadTemp();
		int tmpInt1 = temperature;
		float tmpFrac = temperature - tmpInt1;
		int tmpInt2 = trunc(tmpFrac * 100);
		snprintf((char *) tempBuffer, 50, "TEMPERATURE = %d.%02d\n", tmpInt1, tmpInt2);
		HAL_UART_Transmit(&uartP, (uint8_t *) tempBuffer, 20, 100);
		HAL_Delay(1000);

		check = 1;
	}

	pos = strstr((char *) string, (const char *) "PRESSURE");
	if (pos != NULL)
	{
		uint8_t tempBuffer[50], pressure;

		pressure = BSP_PSENSOR_ReadPressure();
		snprintf((char *) tempBuffer, 50, "PRESSURE = %d\n", pressure);
		HAL_UART_Transmit(&uartP, (uint8_t *) tempBuffer, (uint8_t) strlen(tempBuffer), 100);

		check = 1;
	}
#endif

#ifdef MOTORS

	/*
	RUN command - RUN [MOTOR] [DIR] [SPEED]
	[MOTOR] 1 / 2
	[DIR] FOR / REV
	[SPEED] 0 - 1023

	*/

	pos = strstr((char *) string, (const char *) "RUN");
	if (pos != NULL)
	{
		volatile uint8_t driverPos, dir = 5, data, index = 0;
		volatile float speed;
		volatile uint8_t array[3], array1[3], array2[4];

		//driverPos = pos[4];
		driverPos = (uint8_t) (pos[4] - '0');

		for (int i = 0; i < 3; ++i) array1[i] = pos[6 + i];

		if (array1[0] == 'F')
		{
			if (array1[1] == 'O')
			{
				if (array1[2] == 'R') dir = 0;
				else check = 0;
			}
			else check = 0;

		}

		else if (array1[0] == 'R')
		{
			if (array1[1] == 'E')
			{
				if (array1[2] == 'V') dir = 1;
				else check = 0;

			}
			else check = 0;
		}

		else check = 0;


		if ( (dir == 0) || (dir == 1))
		{
			do {
				array2[index] = (uint8_t)pos[index + 10];
				++index;
			} while ((index < 50) && (array2[index - 1] > 0x2F) && (array2[index - 1] < 0x3A));

			speed = atoi(array2);

			L6470_Run(speed, dir, driverPos);

			check = 1;
		}
	}

	pos = strstr((char *) string, (const char *) "SETPARAM");
	if (pos != NULL)
	{
		volatile uint8_t driverPos, data, param, size, correct = 0;
		volatile uint8_t array1[2], array2[7];

		// Se extrae el motor al que se atacará
		if((pos[9] < '2') && (pos[9] >= '0')) driverPos = (uint8_t) (pos[9] - '0');
		else correct = 1

		// Se extrae el parámetro
		if ((pos[11] == '0') && ((pos[12] == 'x') || (pos[12] == 'X')) && correct = 0)
		{
			param = (pos[13] - '0')* 16 + (pos[14] - '0');
		}
		else correct = 1;

		size = pos[16];
		if((size >= '0') && (size < '3') && (correct = 0)) size -= '0';
		else correct = 1;
		
		if((pos[18] == '0') && ((pos[19] == 'x') || (pos[19] == 'X')) && (correct = 0))
		{
			uint8_t index = 0;
			while((pos[20 + index] >= '0') && (pos[20 + index] < 'E')
			{
				array2[index] = pos[20 + index];
				++index;
			}
			
		}
		else if((pos [18] == 'd') && (correct = 0))
		{

		}
		else correct = 1;

		if ( (dir == 0) || (dir == 1))
		{
			do {
				array2[index] = (uint8_t)pos[index + 10];
				++index;
			} while ((index < 50) && (array2[index - 1] > 0x2F) && (array2[index - 1] < 0x3A));

			speed = atoi(array2);

			L6470_Run(speed, dir, driverPos);

			check = 1;
		}
	}

#endif

	if (check == 0) PRINT_RED((uint8_t *)"Unrecognized command\n", 21);

	PRINT_BLUE((uint8_t *) "#", 1);

	return 0;
}




