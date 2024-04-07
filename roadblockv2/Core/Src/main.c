/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "wt61c.h"
#include "oled.h"
#include "menu.h"
#include "esp01.h"
#include "motor.h"
#include "uwb.h"
#include "font.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void menu_init(void);

uint32_t ADC_Value=0;

struct page p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14;
void menu_init(void);
float test1 = 0, test2 = 1, test3 = 2;
int16_t test4 = 3, test5 = 4, test6 = 5;
/* USER CODE END PTD */

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
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void menu_func_test(void)
{
  oled_clear();
  oled_show_string(0, 0, "menu_func_test()");

  // oled_show_string(0, 1, "hello world!");
  // oled_show_int(0, 2, 123, 3);
  // oled_show_uint(0, 3, 456, 3);
  // oled_show_float(0, 4, 3.14, 3, 3);

  oled_show_binary_image(40,0,gImage_game2,48,48,48,48);
  //默认都是 阴码、列行式、逆向   16*16
  oled_show_chinese(0,0,16,*sbdxq,5 );
  while (1)
  {

    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_RESET)
    {
      HAL_Delay(KEY_DelayTime);
      if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_RESET)
      {
          MenuRender(1);
          return;
      }
    }
  }
}

void adc_test(void)
{
  oled_clear();
  oled_show_string(0, 0, "adc_test()");

  while (1)
  {
    HAL_Delay(500);
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
    {
      ADC_Value = HAL_ADC_GetValue(&hadc1);
      
    }
    HAL_ADC_Stop (&hadc1);
    oled_show_float(0, 1, (float)(ADC_Value*12.155/2647.0), 2,2);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1,10);

    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_RESET)
    {
      HAL_Delay(KEY_DelayTime);
      if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_RESET)
      {
          MenuRender(1);
          return;
      }
    }
  }
}

void pwm_test(void)
{
  oled_clear();
  oled_show_string(0, 0, "pwm_test()");

  HAL_TIM_Base_Start_IT(&htim7);

  set_motor_pwm(1,200,0);
  set_motor_pwm(2,400,0);
  set_motor_pwm(3,600,0);
  set_motor_pwm(4,800,0);
  
 
  while (1)
  {

    oled_show_int(0,1,spd1,5);
    oled_show_int(0,2,spd2,5);
    oled_show_int(0,3,spd3,5);
    oled_show_int(0,4,spd4,5);

    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_RESET)
    {
      HAL_Delay(KEY_DelayTime);
      if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_RESET)
      {
        HAL_TIM_Base_Stop_IT(&htim7);
        set_motor_pwm(1,0,0);
        set_motor_pwm(2,0,0);
        set_motor_pwm(3,0,0);
        set_motor_pwm(4,0,0);
        MenuRender(1);
        return;
      }
    }
  }
}

void free_move(void)
{
  oled_clear();
  oled_show_string(0, 0, "free_move()");

  HAL_TIM_Base_Start_IT(&htim7);
  
  uint8_t time_cnt=0,stop_flag=0;
  while (1)
  {
    time_cnt++;
    if(time_cnt>=free_move_time){
      car_state=STOP;
      stop_flag=1;
    }else if(stop_flag==0){
      car_state=FREEMOVE;
    }

    
    oled_show_int(0,1,spd1,5);
    oled_show_int(0,2,spd2,5);
    oled_show_int(0,3,spd3,5);
    oled_show_int(0,4,spd4,5);
    oled_show_float(0,5,yaw,3,2);
    HAL_Delay(10);
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_RESET)
    {
      HAL_Delay(KEY_DelayTime);
      if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_RESET)
      {
        HAL_TIM_Base_Stop_IT(&htim7);
        set_motor_pwm(1,0,0);
        set_motor_pwm(2,0,0);
        set_motor_pwm(3,0,0);
        set_motor_pwm(4,0,0);
        MenuRender(1);
        return;
      }
    }
  }
}

void move_test(void)
{
  oled_clear();
  oled_show_string(0, 0, "move_test()");
  HAL_TIM_Base_Start_IT(&htim7);
  uint8_t time_cnt=0,stop_flag=0;
  while (1)
  {
    time_cnt++;
    if(time_cnt>=60){
      stop_flag++;
      time_cnt=0;
    }

    if(stop_flag==0) car_state=FORWARD;
    else if(stop_flag==1) car_state=BACKWARD;
    else if(stop_flag==2) car_state=LEFT;
    else if(stop_flag==3) car_state=RIGHT;
    else if(stop_flag==4) car_state=LEFTUP;
    else if(stop_flag==5) car_state=RIGHTUP;
    else if(stop_flag==6) car_state=RIGHTDOWN;
    else if(stop_flag==7) car_state=LEFTDOWN;
    else car_state=STOP;
    oled_show_int(0,1,spd1,5);
    oled_show_int(0,2,spd2,5);
    oled_show_int(0,3,spd3,5);
    oled_show_int(0,4,spd4,5);
    oled_show_float(0,5,yaw,3,2);

    HAL_Delay(10);

    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_RESET)
    {
      HAL_Delay(KEY_DelayTime);
      if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_RESET)
      {
        HAL_TIM_Base_Stop_IT(&htim7);
        set_motor_pwm(1,0,0);
        set_motor_pwm(2,0,0);
        set_motor_pwm(3,0,0);
        set_motor_pwm(4,0,0);
        MenuRender(1);
        return;
      }
    }
  }
}

void angle_test(void)
{
  extern uint8_t angle_test_flag;
  oled_clear();
  oled_show_string(0, 0, "angle_test()");
  angle_test_flag=1;

  HAL_TIM_Base_Start_IT(&htim7);
 
  while (1)
  {

    oled_show_int(0,1,spd1,5);
    oled_show_int(0,2,spd2,5);
    oled_show_int(0,3,spd3,5);
    oled_show_int(0,4,spd4,5);
    oled_show_float(0,5,yaw,3,2);
    car_state=STOP;

    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_RESET)
    {
      HAL_Delay(KEY_DelayTime);
      if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_RESET)
      {
        angle_test_flag=0;
        HAL_TIM_Base_Stop_IT(&htim7);
        set_motor_pwm(1,0,0);
        set_motor_pwm(2,0,0);
        set_motor_pwm(3,0,0);
        set_motor_pwm(4,0,0);
        MenuRender(1);
        return;
      }
    }
  }
}

void pid_reset(void)
{
  oled_clear();
  oled_show_string(0, 0, "pid_reset()");
  oled_show_string(0, 1, "Reset successfully!");
  spd_pid_init();
  
  HAL_Delay(300);
  MenuRender(1);
  return;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_TIM5_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_TIM9_Init();
  MX_TIM1_Init();
  MX_TIM7_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

	
  printf("System_WakeUp\n");
  HAL_UART_Receive_IT(&huart1,&huart1_RxData,1);
	
  /**********初始化陀螺仪**********/
  wt61c_init(&huart6);
  /**********初始化陀螺仪**********/

  /**********初始化UWB**********/
  uwb_init(&huart3);
  /**********初始化UWB**********/

  /**********初始化ADC**********/
  HAL_ADC_Start(&hadc1);	
  HAL_ADC_PollForConversion(&hadc1,10);
  /**********初始化ADC**********/

  /**********初始化OLED和菜单**********/
  oled_init();
  oled_clear();
  menu_init();
  /**********初始化OLED和菜单**********/

  /**********初始化电机与编码器**********/
  spd_pid_init();
  //初始化PWM接口
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1); //1
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3); //2
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);
  HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1); //3
  HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1); //4
  HAL_TIM_PWM_Start(&htim11, TIM_CHANNEL_1);
  //清空比较值
  __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, 0);
  __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, 0);
  __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 0);
  __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, 0);
  __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, 0);
  __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_2, 0);
  __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 0);
  __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, 0);
  //初始化编码器
  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);  
  __HAL_TIM_SET_COUNTER(&htim1,0);
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);  
  __HAL_TIM_SET_COUNTER(&htim2,0);
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);  
  __HAL_TIM_SET_COUNTER(&htim3,0);
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);  
  __HAL_TIM_SET_COUNTER(&htim4,0);
  /**********初始化电机与编码器**********/

  /**********初始化ESP01**********/
  // uint8_t esp_try_cnt=0;
  // //使能引脚高电平
  // HAL_GPIO_WritePin(ESP_EN_GPIO_Port, ESP_EN_Pin, GPIO_PIN_SET);

  // while(esp_try_cnt<5){
  //   printf("esp_round:%d\n",esp_try_cnt);
  //   HAL_GPIO_WritePin(ESP_RST_GPIO_Port, ESP_RST_Pin, GPIO_PIN_SET);
  //   HAL_Delay(100);
  //   HAL_GPIO_WritePin(ESP_RST_GPIO_Port, ESP_RST_Pin, GPIO_PIN_RESET);
  //   HAL_Delay(100);
  //   HAL_GPIO_WritePin(ESP_RST_GPIO_Port, ESP_RST_Pin, GPIO_PIN_SET);
  //   HAL_Delay(100);
  //   HAL_Delay(500);
  //   if(esp01_init()==1) break;
  //   else esp_try_cnt++;
  // }
  // if(esp_try_cnt>=5) printf("error:esp01 init failed!\n");
  /**********初始化ESP01**********/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    MenuCmd(key_scan());
    if (navigate[cntpage]->dymantic_page)
    {
      MenuRender(0);
      HAL_Delay(100);
    }
    HAL_Delay(5);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void menu_init(void)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
{
  add_subpage(&p0, "function", &p1);
  add_subpage(&p0, "pid", &p2);
  add_subpage(&p0, "state", &p3);

  add_func(&p1, "<free_move>", free_move);
  add_func(&p1, "<pid_reset>", pid_reset);
  add_func(&p1, "<menu_func_test>", menu_func_test);
  add_func(&p1, "<pwm_test>", pwm_test);
  add_func(&p1, "<adc_test>", adc_test);
  add_func(&p1, "<move_test>", move_test);
  add_func(&p1, "<angle_test>", angle_test);
  
  add_value_float(&p2, "[a_kp]", &a_kp, 0.5, NULL);
  add_value_float(&p2, "[a_kd]", &a_kd, 1, NULL);
  add_value(&p2, "[test4]", (int *)&test4, 1, NULL);

  add_value(&p3, "[fm_time]", (int *)&free_move_time, 10, NULL);
  add_value(&p3, "[fm_degree]", (int *)&free_move_degree, 5, NULL);
  add_value(&p3, "[general_spd]", (int *)&general_spd, 5, NULL);
  add_value_float(&p3, "[test3]", &test3, 0.3, NULL);
  add_value(&p3, "[test6]", (int *)&test4, 3, NULL);

  MenuInit(&p0);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
