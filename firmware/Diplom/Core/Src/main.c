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
#include "stdlib.h"
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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  RetargetInit(&huart2);
  DEBUG_PRINT(DEBUG_PRINT_TRACE, "[DEBUG] Led Strip Inited\r\n");
  MFRC522_Init();
  DEBUG_PRINT(DEBUG_PRINT_INFO, "[DEBUG] MFRC522 Inited\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t status;
  uint8_t *pSakBuff;
  uint8_t *pCardBuff;
  uint8_t sizeRC;
  uint8_t *pSerialNum;

  uint8_t sectorKeyA[16][16] = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                                 {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                                 {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                                 {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},};
  while (1)
  {
      pSakBuff = (uint8_t *) malloc(sizeof(*pSakBuff)*16);
      MFRC522_Init();
      HAL_Delay(20);
      status = MI_ERR;
      status = MFRC522_Request(PICC_REQIDL, pSakBuff);
      if (status == MI_OK)
      {
          switch(pSakBuff[0])
          {
          case 0x44:
              DEBUG_PRINT(DEBUG_PRINT_INFO, "Card type: MF1S500yX\r\n");
              break;
          case 0x04:
              DEBUG_PRINT(DEBUG_PRINT_INFO, "Card type: MF1S503yX\r\n");
              break;
          case 0x42:
              DEBUG_PRINT(DEBUG_PRINT_INFO, "Card type: MF1S700yX\r\n");
              break;
          case 0x02:
              DEBUG_PRINT(DEBUG_PRINT_INFO, "Card type: MF1S703yX\r\n");
              break;
          case 0x08:
              DEBUG_PRINT(DEBUG_PRINT_INFO, "Card type: MF1S50yyX/V1\r\n");
              break;
          default:
              DEBUG_PRINT(DEBUG_PRINT_INFO, "Unknown card type\r\n");
          }
          DEBUG_PRINT(DEBUG_PRINT_INFO, "SAK: 0x%02X 0x%02X\r\n", pSakBuff[1], pSakBuff[0]);
      }
      free(pSakBuff);

      // Anti-collision, return the card's 4-byte serial number
      pSerialNum = (uint8_t *) malloc(sizeof(*pSerialNum)*16);
      status = MFRC522_Anticoll(pSerialNum);
      if (status == MI_OK)
      {
          DEBUG_PRINT
          (
              DEBUG_PRINT_INFO,
              "SN: 0x%02X 0x%02X 0x%02X 0x%02X\r\n",
              pSerialNum[0],
              pSerialNum[1],
              pSerialNum[2],
              pSerialNum[3]
          );
      }


      // Election card, return capacity
      sizeRC = MFRC522_SelectTag(pSerialNum);
      if (sizeRC != 0)
      {
          DEBUG_PRINT(DEBUG_PRINT_INFO, "CS: %d\r\n", sizeRC);
      }


      // Card reader
      pCardBuff = (uint8_t *) malloc(sizeof(*pCardBuff)*16);

      for (int i = 0;i<64;i++)
      {
          status = MFRC522_Auth(PICC_AUTHENT1A, i, sectorKeyA[0], pSerialNum);
          if (status == MI_OK)
          {
              status = MFRC522_Read(i, pCardBuff);
              if (status == MI_OK)
              {
                  DEBUG_PRINT(DEBUG_PRINT_INFO,"%02X   ", i);
                  for (int j = 0;j<16;j++)
                  {
                      DEBUG_PRINT(DEBUG_PRINT_INFO,"%02X ", pCardBuff[j]);
                  }
                  DEBUG_PRINT(DEBUG_PRINT_INFO,"\r\n");
              }
          }
      }
      free(pCardBuff);
      free(pSerialNum);

      MFRC522_Halt();
      MFRC522_AntennaOff();

      HAL_Delay(1000);
      HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
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
