#include "main.h"
#include "esp01.h"
#include "usart.h"
#include "string.h"

//开启本debug模式后，会使用printf函数在串口打印esp01的回显，以方便调试
//前提：printf确实绑定了一个实际存在的串口
#define ESP01_DEBUG_MODULE 1

//设定esp01所用的串口
#define ESP_UART_HANDLE huart2

#ifdef ESP01_DEBUG_MODULE
#endif // ESP01_DEBUG_MODULE

//初始化指令集合
const uint8_t AT[5]="AT\r\n"; //AT测试指令
const uint8_t ATE0[10]="ATE0\r\n";
const uint8_t AT_CWMODE_1[15]="AT+CWMODE=1\r\n"; //设置芯片为STA模式
const uint8_t AT_CIPMUX_0[15]="AT+CIPMUX=0\r\n"; //设置芯片为单链接模式
const uint8_t AT_CWJAP_ID_PW[40]="AT+CWJAP=\"martinx_wifi\",\"qq209644\"\r\n"; //设置连接wifi热点的账号和密码
const uint8_t AT_CIPSTART_TCP_IP[50]="AT+CIPSTART=\"TCP\",\"192.168.31.35\",8080\r\n"; //建立 TCP 连接到某个IP地址的入口
const uint8_t AT_CIPMODE_1[20]="AT+CIPMODE=1\r\n"; //开启串口透传模式
const uint8_t AT_CIPSEND[15]="AT+CIPSEND\r\n"; //进入透传

//退出透传指令集合
const uint8_t CIP_LEAVE_SIGNAL[5]="+++"; //退出透传的先行信号，防止命令本身被当成透传
const uint8_t AT_CIPMODE_0[20]="AT+CIPMODE=0\r\n"; //退出透传的先行信号，防止命令本身被当成透传

//其它指令
const uint8_t* AT_CWQAP="AT+CWQAP\r\n"; //断开热点
const uint8_t* AT_CIPCLOSE="AT+CIPCLOSE\r\n";
const uint8_t AT_RST[10]="AT+RST\r\n";

uint8_t cip_string[50]={0};

uint8_t esp01_rx_data=0;
uint8_t esp01_rx_buffer[100]={0};
uint8_t esp01_recv_cnt=0;

//当且仅当有返回数据且返回数据包含OK，才允许继续执行
uint8_t isEsp01Rece(){
    if(esp01_recv_cnt!=0){
        if(strstr((const char *)esp01_rx_buffer, "OK") != NULL){
            return 1;
        }
    }
    return 0;
}

//对esp01芯片作基本测试，并设定基本参数
//流程：1、检查芯片状态 2、设置为STA模式 3、开启单链接 4、尝试连接热点 5、完成
uint8_t esp01_init(void){
    uint8_t TIME_OUT=0; //超时退出变量
    //打开ESP串口的接收中断
    HAL_UART_Receive_IT(&ESP_UART_HANDLE, &esp01_rx_data, sizeof(esp01_rx_data));

    //清空并发送新指令
    TIME_OUT=0;
    memset(esp01_rx_buffer,0,sizeof(esp01_rx_buffer));
    esp01_recv_cnt=0;
    printf("send:AT\n");
    HAL_UART_Transmit(&ESP_UART_HANDLE,AT,sizeof(AT),1000);
    HAL_Delay(100);
    while(isEsp01Rece()==0 && TIME_OUT<50){
        HAL_Delay(100);
        TIME_OUT++;
    }
    if(TIME_OUT>=50){
        printf("error:time out!\n");
        return 0;
    }

     //清空并发送新指令
    TIME_OUT=0;
    memset(esp01_rx_buffer,0,sizeof(esp01_rx_buffer));
    esp01_recv_cnt=0;
    printf("send:ATE0\n");
    HAL_UART_Transmit(&ESP_UART_HANDLE,ATE0,sizeof(ATE0),1000);
    HAL_Delay(100);
    while(isEsp01Rece()==0 && TIME_OUT<50){
        HAL_Delay(100);
        TIME_OUT++;
    }
    if(TIME_OUT>=50){
        printf("error:time out!\n");
        return 0;
    }

    //清空并发送新指令
    TIME_OUT=0;
    memset(esp01_rx_buffer,0,sizeof(esp01_rx_buffer));
    esp01_recv_cnt=0;
    printf("send:AT+CWMODE=1\n");
    HAL_UART_Transmit(&ESP_UART_HANDLE,AT_CWMODE_1,sizeof(AT_CWMODE_1),1000);
    HAL_Delay(100);
    while(isEsp01Rece()==0 && TIME_OUT<50){
        HAL_Delay(100);
        TIME_OUT++;
    }
    if(TIME_OUT>=50){
        printf("error:time out!\n");
        return 0;
    }

    //清空并发送新指令
    TIME_OUT=0;
    memset(esp01_rx_buffer,0,sizeof(esp01_rx_buffer));
    esp01_recv_cnt=0;
    printf("send:AT+CIPMUX=0\n");
    HAL_UART_Transmit(&ESP_UART_HANDLE,AT_CIPMUX_0,sizeof(AT_CIPMUX_0),1000);
    HAL_Delay(100);
    while(isEsp01Rece()==0 && TIME_OUT<50){
        HAL_Delay(100);
        TIME_OUT++;
    }
    if(TIME_OUT>=50){
        printf("error:time out!\n");
        return 0;
    }

    //清空并发送新指令
    TIME_OUT=0;
    memset(esp01_rx_buffer,0,sizeof(esp01_rx_buffer));
    esp01_recv_cnt=0;
    printf("send:AT+CWJAP=\"martinx_wifi\",\"qq209644\"\n");
    HAL_UART_Transmit(&ESP_UART_HANDLE,AT_CWJAP_ID_PW,sizeof(AT_CWJAP_ID_PW),1000);
    HAL_Delay(100);
    //wifi连接较慢，给两倍时间
    while(isEsp01Rece()==0 && TIME_OUT<200){
        HAL_Delay(100);
        printf("*");
        TIME_OUT++;
    }printf("\n");
    if(TIME_OUT>=200){
        printf("error:time out!\n");
        return 0;
    }

    //清空并发送新指令
    TIME_OUT=0;
    memset(esp01_rx_buffer,0,sizeof(esp01_rx_buffer));
    esp01_recv_cnt=0;
    printf("send:AT+CIPSTART=\"TCP\",\"192.168.31.35\",8080\n");
    HAL_UART_Transmit(&ESP_UART_HANDLE,AT_CIPSTART_TCP_IP,sizeof(AT_CIPSTART_TCP_IP),1000);
    HAL_Delay(100);
    while(isEsp01Rece()==0 && TIME_OUT<50){
        HAL_Delay(100);
        TIME_OUT++;
    }
    if(TIME_OUT>=50){
        printf("error:time out!\n");
        return 0;
    }

    //清空并发送新指令
    TIME_OUT=0;
    memset(esp01_rx_buffer,0,sizeof(esp01_rx_buffer));
    esp01_recv_cnt=0;
    printf("send:AT+CIPMODE=1\n");
    HAL_UART_Transmit(&ESP_UART_HANDLE,AT_CIPMODE_1,sizeof(AT_CIPMODE_1),1000);
    HAL_Delay(100);
    while(isEsp01Rece()==0 && TIME_OUT<50){
        HAL_Delay(100);
        TIME_OUT++;
    }
    if(TIME_OUT>=50){
        printf("error:time out!\n");
        return 0;
    }

    //清空并发送新指令
    TIME_OUT=0;
    memset(esp01_rx_buffer,0,sizeof(esp01_rx_buffer));
    esp01_recv_cnt=0;
    printf("send:AT+CIPSEND\n");
    HAL_UART_Transmit(&ESP_UART_HANDLE,AT_CIPSEND,sizeof(AT_CIPSEND),1000);
    HAL_Delay(100);
    while(isEsp01Rece()==0 && TIME_OUT<50){
        HAL_Delay(100);
        TIME_OUT++;
    }
    if(TIME_OUT>=50){
        printf("error:time out!\n");
        return 0;
    }

    sprintf((char*)cip_string,"mcu1 is online!\n");
    HAL_UART_Transmit(&ESP_UART_HANDLE,cip_string,sizeof(cip_string),1000);
    printf("esp01 excutes successfully!\n");
    return 1;
}

//本函数放置在esp01对应串口中断上
void ESP01_UART_IRQHandler(void){
    printf("%c",esp01_rx_data);
    if(esp01_recv_cnt>=sizeof(esp01_rx_buffer))
        esp01_recv_cnt=0;
    
    esp01_rx_buffer[esp01_recv_cnt++]=esp01_rx_data;
    HAL_UART_Receive_IT(&ESP_UART_HANDLE, &esp01_rx_data, sizeof(esp01_rx_data));
}
