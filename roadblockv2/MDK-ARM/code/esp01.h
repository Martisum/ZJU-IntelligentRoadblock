#ifndef __ESP01_H__
#define __ESP01_H__

#include "main.h"

extern uint8_t esp01_rx_data;

uint8_t esp01_init(void);
void ESP01_UART_IRQHandler(void);

#endif /* __ESP01_H__ */
