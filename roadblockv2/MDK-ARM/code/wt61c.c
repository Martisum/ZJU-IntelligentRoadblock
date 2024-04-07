#include "main.h"
#include "wt61c.h"
#include "usart.h"
#include "string.h"

uint8_t wt61c_rx_buffer;
uint8_t wt61c_angle_buffer[10]={0};

float roll,pitch,yaw;

void wt61c_init(UART_HandleTypeDef *huart){
    HAL_UART_Receive_IT(huart, (uint8_t *)&wt61c_rx_buffer, 1); // 打开接收中断
}

void wt61c_handler(void){
    static uint8_t recv_stage=0;
    static uint8_t recv_address=0;

    if(wt61c_rx_buffer == 0x55 && recv_stage==0){
        recv_stage=1;
    }else if(wt61c_rx_buffer == 0x53 && recv_stage==1){
        recv_stage=2;
        recv_address=1;
    }else if(recv_stage==2){
        if(recv_address>=1 && recv_address<=6){
            wt61c_angle_buffer[recv_address++]=wt61c_rx_buffer;
        }else{
            roll=(float)((int16_t)((wt61c_angle_buffer[2]<<8)|wt61c_angle_buffer[1]))/32768.0f*180.0f;
            pitch=(float)((int16_t)((wt61c_angle_buffer[4]<<8)|wt61c_angle_buffer[3]))/32768.0f*180.0f;
            yaw=(float)((int16_t)((wt61c_angle_buffer[6]<<8)|wt61c_angle_buffer[5]))/32768.0f*180.0f;

            //printf("roll:%.2f pitch:%.2f yaw:%.2f\n",roll,pitch,yaw);
            recv_stage=0;
            recv_address=0;
        }
    }

    HAL_UART_Receive_IT(&WT61C_UART, (uint8_t *)&wt61c_rx_buffer, 1);
}
