#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"

#define STOP 0
#define LEFT 1
#define RIGHT 2
#define FORWARD 3
#define BACKWARD 4
#define LEFTUP 5
#define LEFTDOWN 6
#define RIGHTUP 7
#define RIGHTDOWN 8
#define FREEMOVE 9

#define PI 3.1415926

/*
1号电机使用TIM8_CH1，使用TIM1的encoder
2号电机使用TIM8_CH2，使用TIM2的encoder
......以此类推
*/

typedef struct {
    int16_t set_targetS;
    int16_t pre_targetS;

    int PS;
    float IS;
    float DS;

    int16_t now_error;
    int16_t sum_error;
    int16_t pre_error;
    int diff_tar;

    int16_t pwm_out;
} SPEED;

//自由移动角度（要换算成pi）
extern int8_t free_move_degree;
//自由移动时间
extern uint16_t free_move_time;

extern int16_t spd4,spd1,spd2,spd3;
extern int16_t pwm4,pwm1,pwm2,pwm3;
extern int16_t error_debug;
extern uint8_t car_state;
extern int8_t general_spd;

extern float v_kp1,v_ki1;
extern float v_kp2,v_ki2;
extern float v_kp3,v_ki3;
extern float v_kp4,v_ki4;
extern float a_kp,a_kd;

extern SPEED whl1,whl2,whl3,whl4;
extern SPEED dir;

void spd_pid_init(void);
uint16_t ave_filter(uint8_t id,int16_t spd);
void set_angle(float angle,float now_angle);
void set_motor_speed(uint8_t id,int16_t spd,int now_spd);
void set_motor_pwm(uint8_t id,int16_t pwm,uint8_t dir);
void sent_data(int16_t A, int16_t B, int16_t C, int16_t D, int16_t E);

// //麦轮控制
void rb_stop(int16_t bias);
void move_left(uint16_t speed,int16_t bias);
void move_right(uint16_t speed,int16_t bias);
void move_forward(uint16_t speed,int16_t bias);
void move_backward(uint16_t speed,int16_t bias);
void left_up(uint16_t speed,int16_t bias);
void left_down(uint16_t speed,int16_t bias);                                                                                                                                                              
void right_up(uint16_t speed,int16_t bias);
void right_down(uint16_t speed,int16_t bias);
void move(uint16_t speed,double angle,int16_t bias);
#endif /* __MOTOR_H__ */
