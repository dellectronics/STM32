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
	uint8_t rxData[length * numberOfDrivers];
	uint8_t offset;
	uint8_t txData[numberOfDrivers] = { 0, 0 };

	offset = numberOfDrivers - driverPos;
	txData[offset] = 0x20 | param;

	L6470_nCS_Low();
	HAL_SPI_Transmit(SPI_PORT, &txData[0], numberOfDrivers, 100);
	L6470_nCS_High();

	L6470_nCS_Low();
	HAL_SPI_Receive(SPI_PORT, &rxData[0], (length * numberOfDrivers), 100);
	L6470_nCS_Low();

	for (int i = 0; i < length; ++i)
	{
		*pData = rxData[(i * length) + offset];
		++pData;
	}

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

}	// L6470_Run

void L6470_Move(uint32_t steps, uint8_t dir, uint8_t driverPos)
{
	uint8_t txData[4 * numberOfDrivers];
	uint8_t stepsArray[3];
	uint8_t offset, a = 0;

	for (int var = 0; var < sizeof(txData); ++var)
	{
		txData[var] = 0;
	}

	offset = numberOfDrivers - driverPos;

	for (int i = 2; i >= 0; --i)
	{
		stepsArray[i] = (uint8_t) (steps & 0x000000FF);
		steps = steps >> 8;
	}

	txData[offset] = 0x40 | dir;

	for (int i = offset + numberOfDrivers; i <= (4 * numberOfDrivers) + 1; i += numberOfDrivers)
	{
		txData[i] = stepsArray[a];
		++a;
	}

	for (int i = 0; i <= sizeof(txData) / numberOfDrivers; i += numberOfDrivers)
	{

		L6470_nCS_Low();
		HAL_SPI_Transmit(SPI_PORT, &txData[i], numberOfDrivers, 100);
		L6470_nCS_High();

	}

}	// L6470_Move


void L6470_GoTo(uint32_t pos, uint8_t driverPos)
{
	uint8_t txData[4 * numberOfDrivers];
	uint8_t posArray[3];
	uint8_t offset, a = 0;

	for (int var = 0; var < sizeof(txData); ++var)
	{
		txData[var] = 0;
	}

	offset = numberOfDrivers - driverPos;

	for (int i = 2; i >= 0; --i)
	{
		posArray[i] = (uint8_t) (pos & 0x000000FF);
		pos = pos >> 8;
	}

	txData[offset] = 0x60;

	for (int i = offset + numberOfDrivers; i <= (4 * numberOfDrivers) + 1; i += numberOfDrivers)
	{
		txData[i] = posArray[a];
		++a;
	}

	for (int i = 0; i <= sizeof(txData) / numberOfDrivers; i += numberOfDrivers)
	{

		L6470_nCS_Low();
		HAL_SPI_Transmit(SPI_PORT, &txData[i], numberOfDrivers, 100);
		L6470_nCS_High();

	}

}	// L6470_Goto

void L6470_GoToDir(uint32_t pos, uint8_t dir, uint8_t driverPos)
{
	uint8_t txData[4 * numberOfDrivers];
	uint8_t posArray[3];
	uint8_t offset, a = 0;

	for (int var = 0; var < sizeof(txData); ++var)
	{
		txData[var] = 0;
	}

	offset = numberOfDrivers - driverPos;

	for (int i = 2; i >= 0; --i)
	{
		posArray[i] = (uint8_t) (pos & 0x000000FF);
		pos = pos >> 8;
	}

	txData[offset] = 0x60 | dir;

	for (int i = offset + numberOfDrivers; i <= (4 * numberOfDrivers) + 1; i += numberOfDrivers)
	{
		txData[i] = posArray[a];
		++a;
	}

	for (int i = 0; i <= sizeof(txData) / numberOfDrivers; i += numberOfDrivers)
	{
		L6470_nCS_Low();
		HAL_SPI_Transmit(SPI_PORT, &txData[i], numberOfDrivers, 100);
		L6470_nCS_High();
	}

}	// L6470_GoToDir

void L6470_GoMark(uint8_t driverPos)
{
	uint8_t txData[numberOfDrivers];

	for (int var = 0; var < sizeof(txData); ++var)
	{
		txData[var] = 0;
	}

	txData[numberOfDrivers - driverPos] = 0x78;

	L6470_nCS_Low();
	HAL_SPI_Transmit(SPI_PORT, &txData[numberOfDrivers - driverPos], numberOfDrivers, 100);
	L6470_nCS_High();

}	// L6470_GoMark

void L6470_ResetPos(uint8_t driverPos)
{
	uint8_t txData[numberOfDrivers];

	for (int var = 0; var < sizeof(txData); ++var)
	{
		txData[var] = 0;
	}

	txData[numberOfDrivers - driverPos] = 0xD8;;

	L6470_nCS_Low();
	HAL_SPI_Transmit(SPI_PORT, &txData[numberOfDrivers - driverPos], numberOfDrivers, 100);
	L6470_nCS_High();

}	// L6470_ResetPos

void L6470_ResetDevice(uint8_t driverPos)
{
	uint8_t txData[numberOfDrivers];

	for (int var = 0; var < sizeof(txData); ++var)
	{
		txData[var] = 0;
	}

	txData[numberOfDrivers - driverPos] = 0xC0;;

	L6470_nCS_Low();
	HAL_SPI_Transmit(SPI_PORT, &txData[numberOfDrivers - driverPos], numberOfDrivers, 100);
	L6470_nCS_High();

}	// L6470_ResetDevice

void L6470_SoftStop(uint8_t driverPos)
{
	uint8_t txData[numberOfDrivers];

	for (int var = 0; var < sizeof(txData); ++var)
	{
		txData[var] = 0;
	}

	txData[numberOfDrivers - driverPos] = 0xB0;;

	L6470_nCS_Low();
	HAL_SPI_Transmit(SPI_PORT, &txData[numberOfDrivers - driverPos], numberOfDrivers, 100);
	L6470_nCS_High();

}	// L6470_SoftStop

void L6470_HardStop(uint8_t driverPos)
{
	uint8_t txData[numberOfDrivers];

	for (int var = 0; var < sizeof(txData); ++var)
	{
		txData[var] = 0;
	}

	txData[numberOfDrivers - driverPos] = 0xB8;;

	L6470_nCS_Low();
	HAL_SPI_Transmit(SPI_PORT, &txData[numberOfDrivers - driverPos], numberOfDrivers, 100);
	L6470_nCS_High();

}	// L6470_HardStop

void L6470_SoftHiZ(uint8_t driverPos)
{
	uint8_t txData[numberOfDrivers];

	for (int var = 0; var < sizeof(txData); ++var)
	{
		txData[var] = 0;
	}

	txData[numberOfDrivers - driverPos] = 0xA0;;

	L6470_nCS_Low();
	HAL_SPI_Transmit(SPI_PORT, &txData[numberOfDrivers - driverPos], numberOfDrivers, 100);
	L6470_nCS_High();

}	// L6470_SoftHiZ

void L6470_HardHiZ(uint8_t driverPos)
{
	uint8_t txData[numberOfDrivers];

	for (int var = 0; var < sizeof(txData); ++var)
	{
		txData[var] = 0;
	}

	txData[numberOfDrivers - driverPos] = 0xA8;;

	L6470_nCS_Low();
	HAL_SPI_Transmit(SPI_PORT, &txData[numberOfDrivers - driverPos], numberOfDrivers, 100);
	L6470_nCS_High();

}	// L6470_HardHiZ

void L6470_GetStatus(uint8_t *data, uint8_t driverPos)
{
	uint8_t rxData[2 * numberOfDrivers];
	uint8_t offset;
	uint8_t txData[numberOfDrivers] = { 0, 0 };

	offset = numberOfDrivers - driverPos;
	txData[offset] = 0xD0;

	L6470_nCS_Low();
	HAL_SPI_Transmit(SPI_PORT, &txData[0], numberOfDrivers, 100);
	L6470_nCS_High();

	L6470_nCS_Low();
	HAL_SPI_Receive(SPI_PORT, &rxData[0], (2 * numberOfDrivers), 100);
	L6470_nCS_Low();

	for (int i = 0; i < length; ++i)
	{
		*pData = rxData[(i * 2) + offset];
		++pData;
	}
}	// L6470_GetStatus