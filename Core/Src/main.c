/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uart_type        uart; 
timing_type       tim;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM6_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  uart.errorflag      = false;
  uart.validmsg       = false;
  uart.msg_state      = STATESTART;
  uart.len_verify     = 0;                        // Initialize length verify counter to 0
  uart.producer_index = 0;                        // Initialize consumer index
  uart.consumer_index = 0;                        // Initialize producer index
  uart.inmenu         = false;                    // Will not start out in console menu
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  init_timer (&tim);

  // TODO: do we need to init the UART?

  HAL_TIM_Base_Start_IT(&htim6);

  HAL_UART_Receive_IT(&huart1, &uart->rxchar, 1);  // UART to console interface

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {

    /**
     * Message handling
     */
    if (uart.byte_counter >= MAX_RX_BUF_INDEX) {
      ResetRxBuffer(&uart);                        //Something went wrong, reset the RX buffer.
    }
    else if(uart.consumer_index != uart.producer_index) {             //We have unprocessed data when indices do not agree
      HandleByte(&uart);
    }

    //TODO: we should never get here ... Delete?
    // if(uart.validmsg == true) {     //A valid message confirmed in buffer
    //     uart.validmsg = false;      //Avoid diving into ProcessMessage for no reason
    //     ProcessMessage(& uart);
    // }

    // if(tim.timer_100ms_running && ((tim.timer_100ms_cntr) >= FUSE_100MS_TICKS_TIMEOUT)) {
    //   anlg_sw_all_off();
    //   fus.fuse_lighting_bool = false;
    //   tim.timer_100ms_running = false;
    //   tim.timer_100ms_cntr = 0;

    // }


    if(tim.flag_10ms_tick) {
      tim.flag_10ms_tick = false;
    }

    // if(tim.flag_100ms_tick) {
    // tim.flag_100ms_tick = false;
    // if(fus.fuse_lighting_bool) {
    //   HAL_GPIO_TogglePin(EXT_LED_2_GPIO_Port, EXT_LED_2_Pin); // External RED LED
    // }
    // else {
    //   HAL_GPIO_WritePin(EXT_LED_2_GPIO_Port,EXT_LED_2_Pin, GPIO_PIN_RESET);
    // }
    
    /* If all fuses are bad, illuminate the amber LED */
    // if(fus.all_fuses_bad_bool){
    //   HAL_GPIO_TogglePin(EXT_LED_3_GPIO_Port, EXT_LED_3_Pin); // External Amber LED
    // }
    // else {
    //   HAL_GPIO_WritePin(EXT_LED_3_GPIO_Port, EXT_LED_3_Pin, GPIO_PIN_RESET);
    // }
	  

    if(tim.flag_500ms_tick) {
      tim.flag_500ms_tick = false;
      HAL_GPIO_TogglePin(HLTH_LED_GPIO_Port, HLTH_LED_Pin);   // Board LED
      // HAL_GPIO_TogglePin(EXT_LED_1_GPIO_Port, EXT_LED_1_Pin); // External GRN LED
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL_NONE;
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

  /** Configure the Systick interrupt time
  */
  __HAL_RCC_PLLI2S_ENABLE();
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 72-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 10000-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 57600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/********************************************//**
*  @brief Initialize timer events 
***********************************************/
void init_timer (timing_type * tim) {
  tim -> led_fast_blink = false;
  tim -> flag_10ms_tick = false;
  tim -> flag_100ms_tick = false;
  tim -> flag_500ms_tick = false;
  tim -> ticks10ms = 0;
  tim -> ticks100ms = 0;
  tim -> ticks500ms = 0;

  tim -> timer_100ms_running = false;
  tim -> timer_100ms_cntr = 0;

}

/********************************************//**
*  @brief Handle Timer Interrupts 
***********************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim == &htim6){
			tim.flag_10ms_tick = true;

		 if(tim.ticks10ms == 9) {
		   tim.ticks10ms = 0;
		   tim.flag_100ms_tick = true;
       if(tim.timer_100ms_running) {
        tim.timer_100ms_cntr++;
       }
       else {
        tim.timer_100ms_cntr = 0;
       }

		   if(tim.ticks100ms == 4) {
			 tim.ticks100ms = 0;
			 tim.flag_500ms_tick = true;

			 if(tim.ticks500ms == 119)										// One minute worth of half seconds
			   tim.ticks500ms = 0;
			 else
			   tim.ticks500ms += 1;
		   }
		   else {
			   tim.ticks100ms += 1;
		   }
		 }
		 else {
		   tim.ticks10ms += 1;
		 }

		}
}

/********************************************//**
 *  @brief Handle UART RX interrupts
 ***********************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	
  /**
   * Console UART Interface
   * 
   */
  if(huart == &huart1) {

    uart.rxbuf[uart.producer_index] = uart.rxchar;          // Load this byte into rx buffer  
    uart.byte_counter++;                                                   //Increase data counter
    (uart.producer_index >= MAX_RX_BUF_INDEX) ? (uart.producer_index = 0):(uart.producer_index++);       
		HAL_UART_Receive_IT(&huart1, &uart.rxchar, 1);
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
