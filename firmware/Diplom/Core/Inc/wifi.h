#ifndef INC_WIFI_H_
#define INC_WIFI_H_

#include "stdio.h"
#include "retarget.h"
#include "string.h"
#include "stdlib.h"
#include "usart.h"
#include "stm32f0xx_hal.h"

#define MAX_MSG_LEN 128

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void UART1_RxCpltCallback(void);
void ESP_RxMessageHandler(void);

#endif /* INC_WIFI_H_ */
