#include "main.h"
#include "esp01.h"
#include "usart.h"
#include "string.h"

//������debugģʽ�󣬻�ʹ��printf�����ڴ��ڴ�ӡesp01�Ļ��ԣ��Է������
//ǰ�᣺printfȷʵ����һ��ʵ�ʴ��ڵĴ���
#define ESP01_DEBUG_MODULE 1

//�趨esp01���õĴ���
#define ESP_UART_HANDLE huart2

#ifdef ESP01_DEBUG_MODULE
#endif // ESP01_DEBUG_MODULE

//��ʼ��ָ���
const uint8_t AT[5]="AT\r\n"; //AT����ָ��
const uint8_t ATE0[10]="ATE0\r\n";
const uint8_t AT_CWMODE_1[15]="AT+CWMODE=1\r\n"; //����оƬΪSTAģʽ
const uint8_t AT_CIPMUX_0[15]="AT+CIPMUX=0\r\n"; //����оƬΪ������ģʽ
const uint8_t AT_CWJAP_ID_PW[40]="AT+CWJAP=\"martinx_wifi\",\"qq209644\"\r\n"; //��������wifi�ȵ���˺ź�����
const uint8_t AT_CIPSTART_TCP_IP[50]="AT+CIPSTART=\"TCP\",\"192.168.31.35\",8080\r\n"; //���� TCP ���ӵ�ĳ��IP��ַ�����
const uint8_t AT_CIPMODE_1[20]="AT+CIPMODE=1\r\n"; //��������͸��ģʽ
const uint8_t AT_CIPSEND[15]="AT+CIPSEND\r\n"; //����͸��

//�˳�͸��ָ���
const uint8_t CIP_LEAVE_SIGNAL[5]="+++"; //�˳�͸���������źţ���ֹ���������͸��
const uint8_t AT_CIPMODE_0[20]="AT+CIPMODE=0\r\n"; //�˳�͸���������źţ���ֹ���������͸��

//����ָ��
const uint8_t* AT_CWQAP="AT+CWQAP\r\n"; //�Ͽ��ȵ�
const uint8_t* AT_CIPCLOSE="AT+CIPCLOSE\r\n";
const uint8_t AT_RST[10]="AT+RST\r\n";

uint8_t cip_string[50]={0};

uint8_t esp01_rx_data=0;
uint8_t esp01_rx_buffer[100]={0};
uint8_t esp01_recv_cnt=0;

//���ҽ����з��������ҷ������ݰ���OK�����������ִ��
uint8_t isEsp01Rece(){
    if(esp01_recv_cnt!=0){
        if(strstr((const char *)esp01_rx_buffer, "OK") != NULL){
            return 1;
        }
    }
    return 0;
}

//��esp01оƬ���������ԣ����趨��������
//���̣�1�����оƬ״̬ 2������ΪSTAģʽ 3������������ 4�����������ȵ� 5�����
uint8_t esp01_init(void){
    uint8_t TIME_OUT=0; //��ʱ�˳�����
    //��ESP���ڵĽ����ж�
    HAL_UART_Receive_IT(&ESP_UART_HANDLE, &esp01_rx_data, sizeof(esp01_rx_data));

    //��ղ�������ָ��
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

     //��ղ�������ָ��
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

    //��ղ�������ָ��
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

    //��ղ�������ָ��
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

    //��ղ�������ָ��
    TIME_OUT=0;
    memset(esp01_rx_buffer,0,sizeof(esp01_rx_buffer));
    esp01_recv_cnt=0;
    printf("send:AT+CWJAP=\"martinx_wifi\",\"qq209644\"\n");
    HAL_UART_Transmit(&ESP_UART_HANDLE,AT_CWJAP_ID_PW,sizeof(AT_CWJAP_ID_PW),1000);
    HAL_Delay(100);
    //wifi���ӽ�����������ʱ��
    while(isEsp01Rece()==0 && TIME_OUT<200){
        HAL_Delay(100);
        printf("*");
        TIME_OUT++;
    }printf("\n");
    if(TIME_OUT>=200){
        printf("error:time out!\n");
        return 0;
    }

    //��ղ�������ָ��
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

    //��ղ�������ָ��
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

    //��ղ�������ָ��
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

//������������esp01��Ӧ�����ж���
void ESP01_UART_IRQHandler(void){
    printf("%c",esp01_rx_data);
    if(esp01_recv_cnt>=sizeof(esp01_rx_buffer))
        esp01_recv_cnt=0;
    
    esp01_rx_buffer[esp01_recv_cnt++]=esp01_rx_data;
    HAL_UART_Receive_IT(&ESP_UART_HANDLE, &esp01_rx_data, sizeof(esp01_rx_data));
}
