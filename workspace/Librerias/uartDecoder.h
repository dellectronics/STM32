/*
 * uartDecoder.h
 *
 *  Created on: 27 nov. 2018
 *      Author: dcamacho
 */

#ifndef UARTDECODER_H_
#define UARTDECODER_H_

//#include "stm32l4xx_hal.h"
#include "stm32f4xx_hal.h"
#include <string.h>

#define uartP huart3
#define USART USART3
#define endChar '\r'

#define MOTORS
//#define SENSORS

#define LED
#define LED_PIN LD3_Pin
#define LED_PORT LD3_GPIO_Port

void  HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle);
void initUartDecode(void);

uint8_t decoderMsg(uint8_t *string);

#endif /* UARTDECODER_H_ */
