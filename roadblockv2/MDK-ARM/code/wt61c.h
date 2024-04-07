#ifndef __WT61C_H__
#define __WT61C_H__

#include "main.h"

#define WT61C_UART huart6

extern uint8_t wt61c_rx_buffer;
extern float roll,pitch,yaw;

void wt61c_init(UART_HandleTypeDef *huart);
void wt61c_handler(void);

#endif /* __WT61C_H__ */
