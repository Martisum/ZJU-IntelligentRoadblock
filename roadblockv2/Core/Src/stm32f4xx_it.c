/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "wt61c.h"
#include "esp01.h"
#include "motor.h"
#include "tim.h"
#include "oled.h"
#include "uwb.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
uint8_t angle_test_flag=0;
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim7;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */
  spd2=__HAL_TIM_GET_COUNTER(&htim1); //2
  __HAL_TIM_SET_COUNTER(&htim1,0);
  spd2=-spd2;
  //spd1=ave_filter(1,spd1);

  spd3=__HAL_TIM_GET_COUNTER(&htim2); //3
  __HAL_TIM_SET_COUNTER(&htim2,0);
  spd3=-spd3;
  // //spd2=ave_filter(2,spd2);

  spd1=__HAL_TIM_GET_COUNTER(&htim3); //1
  __HAL_TIM_SET_COUNTER(&htim3,0);
  spd1=-spd1;
  // //spd3=ave_filter(2,spd3);

  spd4=__HAL_TIM_GET_COUNTER(&htim4); //4
  __HAL_TIM_SET_COUNTER(&htim4,0);
  //spd4=ave_filter(2,spd4);

  // �����PWM���ٶ�ֵ������PWM��ֻ����������
  set_angle(0,yaw);
  //dir.pwm_out=0;
  // if(angle_test_flag){
  //   set_motor_speed(1,-dir.pwm_out,spd1);
  //   set_motor_speed(3,-dir.pwm_out,spd3);
  //   set_motor_speed(2,dir.pwm_out,spd2);
  //   set_motor_speed(4,dir.pwm_out,spd4);
  // }
  

  // if(car_state==STOP) rb_stop();                                                                                                                                                                                      
  // else if(car_state==LEFT) move_left(general_spd,dir.pwm_out);
  // else if(car_state==RIGHT) move_right(general_spd,dir.pwm_out);
  // else if(car_state==FORWARD) move_forward(general_spd,dir.pwm_out);                                                                                     
  // else if(car_state==BACKWARD) move_backward(general_spd,dir.pwm_out);         
  // else if(car_state==LEFTUP) left_up(general_spd,dir.pwm_out);
  // else if(car_state==RIGHTUP) right_up(general_spd,dir.pwm_out);
  // else if(car_state==LEFTDOWN) left_down(general_spd,dir.pwm_out);
  // else if(car_state==RIGHTDOWN) right_down(general_spd,dir.pwm_out);

  if(car_state==STOP) rb_stop(dir.pwm_out);                                                                                                                                                                                      
  else if(car_state==LEFT) move(general_spd,PI/2,dir.pwm_out);
  else if(car_state==RIGHT) move(general_spd,-PI/2,dir.pwm_out);
  else if(car_state==FORWARD) move(general_spd,0,dir.pwm_out);                                                                                     
  else if(car_state==BACKWARD) move(general_spd,PI,dir.pwm_out);         
  else if(car_state==LEFTUP) move(general_spd,PI/4,dir.pwm_out);
  else if(car_state==RIGHTUP) move(general_spd,-PI/4,dir.pwm_out);
  else if(car_state==LEFTDOWN) move(general_spd,3*PI/4,dir.pwm_out);
  else if(car_state==RIGHTDOWN) move(general_spd,-3*PI/4,dir.pwm_out);
  else if(car_state==FREEMOVE) move(general_spd,(float)((float)free_move_degree/180.0)*PI,dir.pwm_out);

  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */

  /* USER CODE END USART6_IRQn 0 */
  HAL_UART_IRQHandler(&huart6);
  /* USER CODE BEGIN USART6_IRQn 1 */

  /* USER CODE END USART6_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance==USART1)
  {
    HAL_UART_Transmit(&huart1,&huart1_RxData,sizeof(huart1_RxData),10);  //�����Է�
    HAL_UART_Receive_IT(&huart1,&huart1_RxData,1);
  }

  if(huart->Instance==USART2){
    ESP01_UART_IRQHandler();
  }

  if(huart->Instance==USART3){
    UWB_UART_handler();
  }

  if(huart->Instance==USART6){
    wt61c_handler();
  }
}
/* USER CODE END 1 */
