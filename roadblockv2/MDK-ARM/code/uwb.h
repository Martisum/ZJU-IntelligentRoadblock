#ifndef __UWB_H__
#define __UWB_H__

#include "main.h"

#define UWB_UART_HANDLE huart3

void uwb_init(UART_HandleTypeDef *huart);
void UWB_UART_handler(void);

#endif /* __UWB_H__ */
