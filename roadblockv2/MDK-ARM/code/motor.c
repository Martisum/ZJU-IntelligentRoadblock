#include "main.h"
#include "motor.h"
#include "usart.h"
#include "string.h"
#include "tim.h"
#include "oled.h"
#include "math.h"

int8_t free_move_degree=45;
uint16_t free_move_time=100;

int16_t spd1=0,spd2=0,spd3=0,spd4=0;
int16_t pwm1=0,pwm2=0,pwm3=0,pwm4=0;

int16_t error_debug;

float v_kp1=4,v_ki1=1.5;
float v_kp2=4,v_ki2=1.5;
float v_kp3=4,v_ki3=1.5;
float v_kp4=4,v_ki4=1.5;
float a_kp=2.5,a_kd=10;

int16_t v_filter1[20]={0},v_filter2[20]={0},v_filter3[20]={0},v_filter4[20]={0};

//0-静止 1-左平行移动 2-右平行移动 3-前进 4-后退
uint8_t car_state=0;
int8_t general_spd=60;

SPEED whl1,whl2,whl3,whl4;
SPEED dir;

//平均滤波，滤波长度为20
uint16_t ave_filter(uint8_t id,int16_t spd){
    int16_t sum=0;

    if(id==1) v_filter1[19]=spd;
    if(id==2) v_filter2[19]=spd;
    if(id==3) v_filter3[19]=spd;
    if(id==4) v_filter4[19]=spd;

    for(uint8_t i=1;i<20;i++){
        if(id==1) v_filter1[i-1]=v_filter1[i];
        if(id==2) v_filter2[i-1]=v_filter2[i];
        if(id==3) v_filter3[i-1]=v_filter3[i];
        if(id==4) v_filter4[i-1]=v_filter4[i];
    }

    for(uint8_t i=0;i<20;i++){
        if(id==1) sum+=v_filter1[i];
        if(id==2) sum+=v_filter2[i];
        if(id==3) sum+=v_filter3[i];
        if(id==4) sum+=v_filter4[i];
    }

    return sum/20;
}

void spd_pid_init(){
    whl1.PS=v_kp1;
    whl1.IS=v_ki1;

    whl2.PS=v_kp2;
    whl2.IS=v_ki2;

    whl3.PS=v_kp3;
    whl3.IS=v_ki3;

    whl4.PS=v_kp4;
    whl4.IS=v_ki4;

    dir.PS=a_kp;
    dir.DS=a_kd;
}

void set_angle(float angle,float now_angle){
    dir.set_targetS=angle;
    dir.now_error=dir.set_targetS-now_angle;
    // 这里的pwm输出实际是速度值，不要搞混。这不是PWM
    dir.pwm_out=dir.PS*dir.now_error+dir.DS*(dir.now_error-dir.pre_error);
    if(dir.pwm_out>100) dir.pwm_out=100;
    else if(dir.pwm_out<-100) dir.pwm_out=-100;
    
    dir.pre_error=dir.now_error;
}

void set_motor_speed(uint8_t id,int16_t spd,int now_spd){
    if(id==1){
        whl1.set_targetS=spd;
        whl1.now_error=whl1.set_targetS-now_spd;
        whl1.sum_error+=whl1.now_error;
        if(whl1.sum_error>10000) whl1.sum_error=10000;
        else if(whl1.sum_error<-10000) whl1.sum_error=-10000;

        //说明目标值是大的，pwm要正着给
        whl1.pwm_out=whl1.PS*whl1.now_error+whl1.IS*whl1.sum_error;
        if(whl1.pwm_out>1000) whl1.pwm_out=1000;
        else if(whl1.pwm_out<-1000) whl1.pwm_out=-1000;
        pwm1=whl1.pwm_out;
        if(whl1.pwm_out>=0) set_motor_pwm(1,whl1.pwm_out,0);
        else set_motor_pwm(1,-whl1.pwm_out,1);

    }

    if(id==2){
        whl2.set_targetS=spd;
        whl2.now_error=whl2.set_targetS-now_spd;
        whl2.sum_error+=whl2.now_error;
        if(whl2.sum_error>10000) whl2.sum_error=10000;
        else if(whl2.sum_error<-10000) whl2.sum_error=-10000;

        //说明目标值是大的，pwm要正着给
        whl2.pwm_out=whl2.PS*whl2.now_error+whl2.IS*whl2.sum_error;

        // error_debug=whl2.pwm_out;
        // oled_show_int(0,5,spd,5);
        // oled_show_int(0,6,now_spd,5);
        // oled_show_int(0,7,whl2.now_error,5);

        if(whl2.pwm_out>1000) whl2.pwm_out=1000;
        else if(whl2.pwm_out<-1000) whl2.pwm_out=-1000;

        if(whl2.pwm_out>=0) set_motor_pwm(2,whl2.pwm_out,0);
        else set_motor_pwm(2,-whl2.pwm_out,1);
    }

    if(id==3){
        whl3.set_targetS=spd;
        whl3.now_error=whl3.set_targetS-now_spd;
        whl3.sum_error+=whl3.now_error;
        if(whl3.sum_error>10000) whl3.sum_error=10000;
        else if(whl3.sum_error<-10000) whl3.sum_error=-10000;

        //说明目标值是大的，pwm要正着给
        whl3.pwm_out=whl3.PS*whl3.now_error+whl3.IS*whl3.sum_error;
        if(whl3.pwm_out>1000) whl3.pwm_out=1000;
        else if(whl3.pwm_out<-1000) whl3.pwm_out=-1000;

        if(whl3.pwm_out>=0) set_motor_pwm(3,whl3.pwm_out,0);
        else set_motor_pwm(3,-whl3.pwm_out,1);
    }

    if(id==4){
        whl4.set_targetS=spd;
        whl4.now_error=whl4.set_targetS-now_spd;
        whl4.sum_error+=whl4.now_error;
        if(whl4.sum_error>10000) whl4.sum_error=10000;
        else if(whl4.sum_error<-10000) whl4.sum_error=-10000;

        //说明目标值是大的，pwm要正着给
        whl4.pwm_out=whl4.PS*whl4.now_error+whl4.IS*whl4.sum_error;
        if(whl4.pwm_out>1000) whl4.pwm_out=1000;
        else if(whl4.pwm_out<-1000) whl4.pwm_out=-1000;

        if(whl4.pwm_out>=0) set_motor_pwm(4,whl4.pwm_out,0);
        else set_motor_pwm(4,-whl4.pwm_out,1);
    }
}

//id为电机编号，取1~4。pwm取值0~1000，方向0正1负
//极性要求：dir=0时，电机往前转，且编码器数值为正，调整gpio输出和编码器正负值即可
void set_motor_pwm(uint8_t id,int16_t pwm,uint8_t dir){
    if(id==1){
        if(dir==0){
            __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, 0);
            __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, pwm);
        }else if(dir==1){
            __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, pwm);
            __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, 0);
        }
    }

    if(id==2){
        if(dir==0){
            __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, pwm);
            __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, 0);
        }else if(dir==1){
            __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 0);
            __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, pwm);
        }
    }

    if(id==3){
        if(dir==0){
            __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, pwm);
            __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_2, 0);
        }else if(dir==1){
            __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, 0);
            __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_2, pwm);
        }
    }

    if(id==4){
        if(dir==0){
            __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 0);
            __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, pwm);
        }else if(dir==1){
            __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, pwm);
            __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, 0);
        }
    }
}

// #define PC_WIRELESS 1
#define BYTE0(dwTemp) (*(char *)(&dwTemp))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))
uint8_t BUFF[30];
void sent_data(int16_t A, int16_t B, int16_t C, int16_t D, int16_t E) {
    int i;
    uint8_t sumcheck = 0;
    uint8_t addcheck = 0;
    uint8_t _cnt = 0;
    BUFF[_cnt++] = 0xAA;
    BUFF[_cnt++] = 0xFF;
    BUFF[_cnt++] = 0XF1;
    BUFF[_cnt++] = 0x0A;
    BUFF[_cnt++] = BYTE0(A);
    BUFF[_cnt++] = BYTE1(A);
    BUFF[_cnt++] = BYTE0(B);
    BUFF[_cnt++] = BYTE1(B);
    BUFF[_cnt++] = BYTE0(C);
    BUFF[_cnt++] = BYTE1(C);
    BUFF[_cnt++] = BYTE0(D);
    BUFF[_cnt++] = BYTE1(D);
    BUFF[_cnt++] = BYTE0(E);
    BUFF[_cnt++] = BYTE1(E);

    for (i = 0; i < BUFF[3] + 4; i++) {
        sumcheck += BUFF[i];
        addcheck += sumcheck;
    }
    BUFF[_cnt++] = sumcheck;
    BUFF[_cnt++] = addcheck;

    for (i = 0; i < _cnt; i++) {
        //bluetooth_ch9141_send_buff_ch1(&BUFF[i], 1);
        //wireless_uart_send_byte((uint8) BUFF[i]);
        //uart_write_byte(UART_3, BUFF[i]);
        HAL_UART_Transmit(&huart1, &BUFF[i], sizeof(BUFF[i]), 50);
        //printf("%d", BUFF[i]);
    }
}

void rb_stop(int16_t bias){
    set_motor_speed(2,bias,spd2);
    set_motor_speed(4,bias,spd4);
    set_motor_speed(1,-bias,spd1);
    set_motor_speed(3,-bias,spd3);
}

void move_left(uint16_t speed,int16_t bias){
    set_motor_speed(2,speed+bias,spd2);
    set_motor_speed(4,-speed+bias,spd4);
    set_motor_speed(1,-speed-bias,spd1);
    set_motor_speed(3,speed-bias,spd3);
}

void move_right(uint16_t speed,int16_t bias){
    set_motor_speed(2,-speed+bias,spd2);
    set_motor_speed(4,speed+bias,spd4);
    set_motor_speed(1,speed-bias,spd1);
    set_motor_speed(3,-speed-bias,spd3);
}

void move_forward(uint16_t speed,int16_t bias){
    set_motor_speed(2,speed+bias,spd2);
    set_motor_speed(4,speed+bias,spd4);
    set_motor_speed(1,speed-bias,spd1);
    set_motor_speed(3,speed-bias,spd3);
}

void move_backward(uint16_t speed,int16_t bias){
    set_motor_speed(2,-speed+bias,spd2);
    set_motor_speed(4,-speed+bias,spd4);
    set_motor_speed(1,-speed-bias,spd1);
    set_motor_speed(3,-speed-bias,spd3);
}

void left_up(uint16_t speed,int16_t bias){
    set_motor_speed(2,speed+bias,spd2);
    set_motor_speed(4,0+bias,spd4);
    set_motor_speed(1,0-bias,spd1);
    set_motor_speed(3,speed-bias,spd3);
}

void left_down(uint16_t speed,int16_t bias){
    set_motor_speed(2,0+bias,spd2);
    set_motor_speed(4,-speed+bias,spd4);
    set_motor_speed(1,-speed-bias,spd1);
    set_motor_speed(3,0-bias,spd3);
}

void right_up(uint16_t speed,int16_t bias){
    set_motor_speed(2,0+bias,spd2);
    set_motor_speed(4,speed+bias,spd4);
    set_motor_speed(1,speed-bias,spd1);
    set_motor_speed(3,0-bias,spd3);
}

void right_down(uint16_t speed,int16_t bias){
    set_motor_speed(2,-speed+bias,spd2);
    set_motor_speed(4,0+bias,spd4);
    set_motor_speed(1,0-bias,spd1);
    set_motor_speed(3,-speed-bias,spd3);
}

//全向移动，其中左向angle大于0，右向angle小于0，两侧最大都是PI
//speed为速度模长
void move(uint16_t speed,double angle,int16_t bias){
    static int16_t Va=0,Vb=0; //Va为左下-右上方向速度分量，Vb为右下-左上方向速度分量
    angle+=PI/4; //旋转车辆坐标系为辊轮坐标系
    if(angle>PI) angle-=2*PI; //保持角度范围在[-PI,PI]
    Va=speed*cos(angle);
    Vb=speed*sin(angle);

    //换算成车子单个轮子的速度
    Va*=sqrt(2);
    Vb*=sqrt(2);                                                                                                                                                                
    
    set_motor_speed(2,Va+bias,spd2);                                                                             
    set_motor_speed(3,Va-bias,spd3);
    set_motor_speed(1,Vb-bias,spd1);
    set_motor_speed(4,Vb+bias,spd4);
}
