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
#include "spi.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  RetargetInit(&huart2);
  DEBUG_PRINT(DEBUG_PRINT_TRACE, "[DEBUG] Led Strip Inited\r\n");
  MFRC522_Init();
  DEBUG_PRINT(DEBUG_PRINT_INFO, "[DEBUG] MFRC522 Inited\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t status;
  uint8_t sakBuff[2];
  uint8_t cardBuff[MFRC522_MAX_LEN];
  uint8_t sizeRC;
  uint8_t serialNum[4];

  uint8_t sectorKeyA[16][16] = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                                {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                                {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                                {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},};
  while (1)
  {
      MFRC522_Init();
      HAL_Delay(100);
      status = MI_ERR;
      status = MFRC522_Request(PICC_REQIDL, sakBuff);
      if (status == MI_OK)
      {
          DEBUG_PRINT(DEBUG_PRINT_INFO, "SAK: 0x%02X, 0x%02X\r\n", sakBuff[1], sakBuff[0]);
      }
      else
      {
          DEBUG_PRINT(DEBUG_PRINT_INFO, "SAK: ----  ----\r\n");
      }

      // Anti-collision, return the card's 4-byte serial number
      status = MFRC522_Anticoll(serialNum);
      if (status == MI_OK)
      {
          DEBUG_PRINT
          (
              DEBUG_PRINT_INFO,
              "CN: %x%x%x%x\r\n",
              serialNum[0],
              serialNum[1],
              serialNum[2],
              serialNum[3]
          );
      }
      else
      {
          DEBUG_PRINT(DEBUG_PRINT_INFO, "CN: --------\r\n");
      }

      // Election card, return capacity
      sizeRC = MFRC522_SelectTag(serialNum);
      if (sizeRC != 0)
      {
          DEBUG_PRINT(DEBUG_PRINT_INFO, "CS: %d\r\n", sizeRC);
      }
      else
      {
          DEBUG_PRINT(DEBUG_PRINT_INFO, "CS: -\r\n");
      }

      // Card reader
      status = MFRC522_Auth(PICC_AUTHENT1A, 11, sectorKeyA[2], serialNum);
      if (status == MI_OK)
      {
          // Read data
          status = MFRC522_Read(11, cardBuff);
              if (status == MI_OK)
              {
                  DEBUG_PRINT
                  (
                      DEBUG_PRINT_INFO,
                      "%02X %02X %02X %02X %02X %02X %02X %02X\r\n",
                      cardBuff[0],
                      cardBuff[1],
                      cardBuff[2],
                      cardBuff[3],
                      cardBuff[4],
                      cardBuff[5],
                      cardBuff[6],
                      cardBuff[7]
                  );
                  DEBUG_PRINT
                  (
                      DEBUG_PRINT_INFO,
                      "%02X %02X %02X %02X %02X %02X %02X %02X\r\n",
                      cardBuff[8],
                      cardBuff[9],
                      cardBuff[10],
                      cardBuff[11],
                      cardBuff[12],
                      cardBuff[13],
                      cardBuff[14],
                      cardBuff[15]
                  );
              }
      }
      else
      {
          DEBUG_PRINT(DEBUG_PRINT_INFO, "-- -- -- -- -- -- -- --\r\n");
      }

      MFRC522_Halt();
      MFRC522_AntennaOff();

      HAL_Delay(1000);
      HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
}

/* USER CODE BEGIN 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
