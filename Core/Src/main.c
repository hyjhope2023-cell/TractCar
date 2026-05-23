/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "car.h"
#include <stdio.h>
#include <string.h>
#include "OLED.h"
#include "track.h"
#include "button.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
int fputc(int ch, FILE *f)
{
  // 利用 HAL 库发送一个字符
  // &huart1 是你在 CubeMX 里配置的串口 1
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

void Update_PID_From_Knob(ADC_HandleTypeDef *hadc,PID_Controller pid) {
    uint32_t raw_val = Get_Knob_Value(hadc);
    
    // 映射计算：将 0-4095 映射到 0.0 - 50.0
    // 公式：(当前值 / 最大值) * 目标量程
    float mapped_kd = (raw_val / 4095.0f) * 50.0f;
    
    // 更新结构体中的参数
    pid->Kd = mapped_kd;
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
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
	Motor_Init(); 
	//uint16_t ccr = 100;
	CarPosition car=createCar();
  HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // Oled->OLED_Clear(Oled);
  // Oled->OLED_ShowString(Oled, 0, 0, "SYSTEM READY", 16);
  // Oled->OLED_Update(Oled);
  // uint32_t last_oled_time = 0;
  // HAL_Delay(1000);

	while(HAL_GPIO_ReadPin(Button_GPIO_Port,Button_Pin)==GPIO_PIN_SET);
	printf("start");
  while (1)
  {
		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

    //  printf("continue");

		/*旋钮修改pid
		static uint32_t last_tick = 0;
    if (HAL_GetTick() - last_tick > 50) {
        Update_PID_From_Knob(&hadc1,car->pid); // 使用你的 PID 指针对象
        last_tick = HAL_GetTick();
        
        // 打印调试，看看旋钮生效没 (注意：大量 printf 会影响实时性，调稳后关闭)
        if(last_tick%1000<50){
        printf("ADC Raw: %d, Target Kd: %.2f\r\n", Get_Knob_Value(&hadc1), car->pid->Kd);
        }
    }
		*/
    car->TRACK_PID(car);
    HAL_Delay(10);


    /* 循环遍历车的状态
    for(enum Direction i=stop;i<DIR_MAX_COUNT;i++){
      printf("%d",i);
      car->MOVE(i,40);
      HAL_Delay(3000);
    }
    */
    /*
    car->MOVE(car->direction,car->speed);
    car->MOVE(forward,car->speed);
    HAL_Delay(10);
    HAL_Delay(1000);
    */
    
		if(HAL_GPIO_ReadPin(Button_GPIO_Port,Button_Pin)==GPIO_PIN_RESET){
      car->MOVE(stop,0);
	    printf("stop");
      while(HAL_GPIO_ReadPin(Button_GPIO_Port,Button_Pin)==GPIO_PIN_RESET);

	    while(HAL_GPIO_ReadPin(Button_GPIO_Port,Button_Pin)==GPIO_PIN_SET);
	    HAL_Delay(500);

    }
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim3){
		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    int sensor_val[4];

    read_track(sensor_val);

		printf("STATE: [%d %d %d %d]\r\n", 
            sensor_val[0],
            sensor_val[1],
            sensor_val[2],
            sensor_val[3]);
    
	}
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
#ifdef USE_FULL_ASSERT
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
