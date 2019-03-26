/*
 * L6470_Driver.h
 *
 *  Created on: 24 mar. 2019
 *      Author: dellectronics
 */

#ifndef L6470_DRIVER_H_
#define L6470_DRIVER_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"



#define CS_PORT	SPI1_CS_GPIO_Port
#define CS_PIN SPI1_CS_Pin
#define numberOfDrivers 2

#define L6470_OK 1;
#define L6470_ERROR 0;

#define SPI_PORT &hspi1

#define	ABS_POS 0x01,			//	R/WS - Current Position
#define	EL_POS 0x02				//	R/WS - Electrical Position
#define	MARK 0x03 				//	R/WR - Mark Position
#define SPEED 0x04				//	R - Current Speed
#define	ACC 0x05				//	R/WS - Acceleration
#define	DEC 0x06 				//	R/WS - Deceleration
#define	MAX_SPEED 0x07			//	R/WR - Maximum speed
#define	MIN_SPEED 0x08			//	R/WS - Minimum speed
#define	KVAL_HOLD 0x09			//	R/WR - Holding Kval
#define	KVAL_RUN 0x0A			//  R/WR - Constant speed Kval
#define	KVAL_ACC 0x0B			//	R/WR - Acceleration starting Kval
#define	KVAL_DEC 0x0C			//	R/WR - Deceleration starting Kval
#define	INT_SPEED 0x0D			//	R/WH - Intersect speed
#define	ST_SLP 0x0E				//	R/WH - Start slope
#define	FN_SLP_ACC 0x0F			//	R/WH - Acceleration final slope
#define	FN_SLP_DEC 0x10			//	R/WH - Deceleration final slope
#define	K_TERM 0x11				//	R/WR - Thermal compensation factor
#define	ADC_OUT 0x12			//	R - ADC out
#define	OCD_TH 0x13				//	R/WR - OCD threshold
#define	STALL_TH 0x14			//	R/WR - Stall threshold
#define	FS_SPD 0x15				//	R/WR - Full-step speed
#define	STEP_MODE 0x16			//	R/WH - Step mode
#define	ALARM_EN 0x17			//	R/WS - Alarm enable
#define CONFIG 0x18				//	R/WH - IC Configurator
#define	STATUS 0x19				//	R - Status

#define FORWARD 1
#define REVERSE 0

void L6470_nCS_High(void);
void L6470_nCS_Low(void);
uint8_t L6470_SetParam(uint8_t param, uint8_t *pData, uint8_t length, uint8_t driverPos);
void L6470_GetParam(uint8_t param, uint8_t *pData, uint8_t length, uint8_t driverPos);
void L6470_Run(uint32_t speed, uint8_t dir, uint8_t driverPos);

#endif /* L6470_DRIVER_H_ */
