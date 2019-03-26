/*
 * L6470_Driver.c
 *
 *  Created on: 24 mar. 2019
 *      Author: dellectronics
 */

#include <L6470_Driver.h>

extern SPI_HandleTypeDef hspi1;

void L6470_nCS_High(void)
{
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

void L6470_nCS_Low(void)
{
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
}

uint8_t L6470_SetParam(uint8_t param, uint8_t *pData, uint8_t length, uint8_t driverPos)
{
	uint8_t txData[numberOfDrivers * (length + 1)];
	uint8_t offset;

	for (int var = 0; var < sizeof(txData); ++var)
	{
		txData[var] = 0;
	}

	if (length > 3)
	{
		return L6470_ERROR;
	}
	else
	{
		offset = numberOfDrivers - driverPos;
		txData[offset] = param;
		for (int i = offset + numberOfDrivers; i <= (length * numberOfDrivers) + 1; i += numberOfDrivers)
		{
			txData[i] = *pData;
			++pData;
		}

		// if (driverPos == 2)
		// {
		// 	txData[0] = param;
		// 	for (int i = 0; i < length; i += 2)
		// 	{
		// 		txData[i+2] = *pData;
		// 		++pData;
		// 	}
		// }

		// else if (driverPos == 1)
		// {
		// 	txData[1] = param;
		// 	for (int i = 1; i < length; i += 2)
		// 	{
		// 		txData[i+2] = *pData;
		// 		++pData;
		// 	}
		// }

		for (int i = 0; i <= sizeof(txData) / numberOfDrivers; i += numberOfDrivers)
		{

			L6470_nCS_Low();
			HAL_SPI_Transmit(SPI_PORT, &txData[i], numberOfDrivers, 100);
			L6470_nCS_High();

		}

		return L6470_OK;
	}

}	// L6470_SetParam

void L6470_GetParam(uint8_t param, uint8_t *pData, uint8_t length, uint8_t driverPos)
{
	uint8_t rxData[4];

}

void L6470_Run(uint32_t speed, uint8_t dir, uint8_t driverPos)
{
	uint8_t txData[4 * numberOfDrivers];
	uint8_t speedArray[3];
	uint8_t offset, a = 0;

	for (int var = 0; var < sizeof(txData); ++var)
	{
		txData[var] = 0;
	}

	offset = numberOfDrivers - driverPos;

	for (int i = 2; i >= 0; --i)
	{
		speedArray[i] = (uint8_t) (speed & 0x000000FF);
		speed = speed >> 8;
	}

	txData[offset] = 0x50 | dir;

	for (int i = offset + numberOfDrivers; i <= (4 * numberOfDrivers) + 1; i += numberOfDrivers)
	{
		txData[i] = speedArray[a];
		++a;
	}

	for (int i = 0; i <= sizeof(txData) / numberOfDrivers; i += numberOfDrivers)
	{

		L6470_nCS_Low();
		HAL_SPI_Transmit(SPI_PORT, &txData[i], numberOfDrivers, 100);
		L6470_nCS_High();

	}

}