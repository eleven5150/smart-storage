/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
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
char str1[250]={0};
typedef struct USART_prop{
  uint8_t usart_buf[250];
  uint8_t usart_cnt;
} USART_prop_ptr;
USART_prop_ptr usartprop = {{0}, 0};

bool ledStripFlag = false;
bool ledStripFlag2 = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void string_parse(char* buf_str)
{
//    DEBUG_PRINT(DEBUG_PRINT_INFO, "[ESP] %s\r\n", buf_str);
    if (!strcmp(buf_str, "0,CONNECT\r\n"))
    {
        ledStripFlag = true;
    }
    if (ledStripFlag && !strcmp(buf_str, "\r\n"))
    {
        ledStripFlag2 = true;
    }
}

void UART1_RxCpltCallBack(void)
{
        uint8_t b;
        b = str1[0];
        //если вдруг случайно превысим длину буфера
        if (usartprop.usart_cnt>249)
        {
          usartprop.usart_cnt=0;
          HAL_UART_Receive_IT(&huart1,(uint8_t*)str1,1);
          return;
        }
        usartprop.usart_buf[usartprop.usart_cnt] = b;
        if(b==0x0A)
        {
          usartprop.usart_buf[usartprop.usart_cnt+1]=0;
          string_parse((char*)usartprop.usart_buf);
          usartprop.usart_cnt=0;
          HAL_UART_Receive_IT(&huart1,(uint8_t*)str1,1);
          return;
        }
        usartprop.usart_cnt++;
        HAL_UART_Receive_IT(&huart1,(uint8_t*)str1,1);
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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
    rfidStatus_t status = MI_ERR;
    RetargetInit(&huart2);
    MFRC522_Init();
    DEBUG_PRINT(DEBUG_PRINT_INFO, "[DEBUG] LedStripStorage started!\r\n");
    HAL_UART_Receive_IT(&huart1,(uint8_t*)str1,1);
    HAL_Delay(1000);

    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(CH_PD_GPIO_Port, CH_PD_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);
    HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"), -1);
    HAL_Delay(1000);
    HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CIPSERVER=1\r\n", strlen("AT+CIPSERVER=1\r\n"), -1);
    HAL_Delay(1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    uint8_t *pRxData;
    pRxData = (uint8_t *) malloc(sizeof(*pRxData)*48);
    LedController_OffAllLeds();
    LedController_Load();
    LedController_OffAllLeds();
    while (1)
    {
        if (ledStripFlag2)
        {
            DEBUG_PRINT(DEBUG_PRINT_INFO, "[ESP] %s\r\n", usartprop.usart_buf);
            char a_x_coord[2] = {(char)usartprop.usart_buf[9], '\0'};
            DEBUG_PRINT(DEBUG_PRINT_INFO, "[ESP] a_x_coord -> %s\r\n", a_x_coord);
            char a_y_coord[2] = {(char)usartprop.usart_buf[10], '\0'};
            DEBUG_PRINT(DEBUG_PRINT_INFO, "[ESP] a_y_coord -> %s\r\n", a_y_coord);
            int x_coord = atoi(a_x_coord);
            int y_coord = atoi(a_y_coord);
            DEBUG_PRINT(DEBUG_PRINT_INFO, "[ESP] x_coord -> %d\r\n", x_coord);
            DEBUG_PRINT(DEBUG_PRINT_INFO, "[ESP] y_coord -> %d\r\n", y_coord);
            LedController_OnXY(x_coord, y_coord);
            ledStripFlag = false;
            ledStripFlag2 = false;
        }
        status = RFID_ReadFullMem();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart==&huart1)
    {
        UART1_RxCpltCallBack();
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
    DEBUG_PRINT(DEBUG_PRINT_ERROR, "[ERROR] Error_Handler\r\n");
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
