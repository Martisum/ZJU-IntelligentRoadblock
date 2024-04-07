#include "main.h"
#include "uwb.h"
#include "usart.h"
#include "string.h"

uint8_t uwb_rx_data=0;
int16_t uwb_rx_pack[50]={0};
int16_t x_uwb=0,y_uwb=0,z_uwb=0;

uint8_t isAnsysFinish=0;

void uwb_init(UART_HandleTypeDef *huart){
    HAL_UART_Receive_IT(huart,(uint8_t *)&uwb_rx_data,1);
}

void UWB_UART_handler(void){
    static uint8_t recv_stage=0;
    static uint8_t recv_address=0;
    static uint8_t recv_length=0;

    //匹配modbus功能吗0x01
    if(recv_stage==0 && uwb_rx_data==0x01){
        recv_stage=1;
    }
    //匹配modbus功能码0x03
    else if(recv_stage==1 && uwb_rx_data==0x03){
        recv_stage=2;
    }
    //匹配寄存器长度0x2E，默认是这个，防止出问题
    else if(recv_stage==2 && uwb_rx_data==0x2E){
        recv_stage=3;
        recv_length=uwb_rx_data;
        recv_address=0; //清空数组地址指针，准备接收数据
    }else if(recv_stage==3 && recv_address<recv_length){
        uwb_rx_pack[recv_address++]=uwb_rx_data;
    }else if(recv_stage==3 && recv_address==recv_length){
        // 到这一步，数据接收完成，开始处理数据
        if(uwb_rx_pack[0]==0xAC && uwb_rx_pack[1]==0xDA){
            printf("recv!!!");
        }

        memset(uwb_rx_pack,0,sizeof(uwb_rx_pack)); //清空数组，准备下一次接收
        recv_address=0;
        recv_stage=0;
        recv_length=0;
    }else{
        recv_address=0;
        recv_stage=0;
        recv_length=0;
    }
    

    

    HAL_UART_Receive_IT(&UWB_UART_HANDLE,(uint8_t *)&uwb_rx_data,1);
}
