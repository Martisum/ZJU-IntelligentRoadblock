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

    //ƥ��modbus������0x01
    if(recv_stage==0 && uwb_rx_data==0x01){
        recv_stage=1;
    }
    //ƥ��modbus������0x03
    else if(recv_stage==1 && uwb_rx_data==0x03){
        recv_stage=2;
    }
    //ƥ��Ĵ�������0x2E��Ĭ�����������ֹ������
    else if(recv_stage==2 && uwb_rx_data==0x2E){
        recv_stage=3;
        recv_length=uwb_rx_data;
        recv_address=0; //��������ַָ�룬׼����������
    }else if(recv_stage==3 && recv_address<recv_length){
        uwb_rx_pack[recv_address++]=uwb_rx_data;
    }else if(recv_stage==3 && recv_address==recv_length){
        // ����һ�������ݽ�����ɣ���ʼ��������
        if(uwb_rx_pack[0]==0xAC && uwb_rx_pack[1]==0xDA){
            printf("recv!!!");
        }

        memset(uwb_rx_pack,0,sizeof(uwb_rx_pack)); //������飬׼����һ�ν���
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
