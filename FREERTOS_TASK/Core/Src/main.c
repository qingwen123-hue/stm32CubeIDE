/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "cmsis_os.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "lcd.h"
#include "w25qxx.h"
#include <string.h>
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
void MX_FREERTOS_Init(void);
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
	int x = 0,FLASH_SIZE,id;
	u8 lcd_id[12],datatemp[20];						//存放LCD ID字符�?????????????
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
  MX_USART1_UART_Init();
  MX_FSMC_Init();
  MX_SPI2_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	POINT_COLOR=RED;     				//画笔颜色：红�?????????????
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组�?????????????
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	LCD_ShowString(30,40,210,24,24,"WarShip STM32 ^_^");
	//LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
	LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,90,200,16,16,lcd_id);		//显示LCD ID
	//LCD_ShowString(30,110,200,12,12,"2017/5/27");
	//LCD_ShowString(30,110,200,16,16,"KEY1:Write KEY0:Read  WK_UP:ADD");	//显示提示信息
	LCD_ShowString(30,110,200,12,12,"RTC TEST");


	////2.设置时间
	//rtc_set_time(2023, 7, 12, 18, 15, 0);
	RTC_TimeTypeDef nTime;
	RTC_DateTypeDef nData;
//	nTime.Hours = 18;
//	nTime.Minutes = 26;
//	nTime.Seconds = 0;
//	nData.Year = 23;
//	nData.Month = 7;
//	nData.Date = 12;
//	HAL_RTC_SetTime(&hrtc,&nTime,RTC_FORMAT_BIN);
//	HAL_RTC_SetDate(&hrtc, &nData, RTC_FORMAT_BIN);

	//rtc_get_time();
	HAL_RTC_GetTime(&hrtc,&nTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &nData, RTC_FORMAT_BIN);

	sprintf((char*)datatemp,"%02d h,%02d m,%02d s",nTime.Hours,nTime.Minutes,nTime.Seconds);
	//sprintf((char*)datatemp,"%02d h,%02d m,%02d s",calendar.hour,calendar.min,calendar.sec);
	LCD_ShowString(30,150,200,12,12,datatemp);
	memset(datatemp,'\0',sizeof(datatemp));
	sprintf((char*)datatemp,"%04d y,%02d m,%02d d",nData.Year + 2000,nData.Month,nData.Date);
	//sprintf((char*)datatemp,"%04d y,%02d m,%02d d",calendar.year,calendar.month,calendar.date);
	LCD_ShowString(30,130,200,12,12,datatemp);
	//__HAL_RTC_ALARM_ENABLE_IT(&hrtc, RTC_IT_SEC);   /* 允许秒中�???? */

	////4.闹钟
//	if(calendar.sec < 50)
//	{
//		calendar.sec += 10;
//	}
//	else
//	{
//		calendar.min += 1;
//		calendar.sec = 0;
//	}


	RTC_AlarmTypeDef aTime;

	aTime.Alarm = 0;
	aTime.AlarmTime.Hours = nTime.Hours;
	aTime.AlarmTime.Minutes = nTime.Minutes;
	if(calendar.sec < 50)
	{
		aTime.AlarmTime.Seconds = nTime.Seconds + 10;
	}
	else
	{
		aTime.AlarmTime.Minutes += 1;
		aTime.AlarmTime.Seconds = 0;
	}
	HAL_RTC_SetAlarm_IT(&hrtc,&aTime,RTC_FORMAT_BIN);

	//rtc_set_alarm(calendar.year, calendar.month, calendar.date, calendar.hour, calendar.min, calendar.sec);

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		////1.获取时间
//		rtc_get_time();
//		sprintf((char*)datatemp,"%04d y,%02d m,%02d d",calendar.year,calendar.month,calendar.date);
//		LCD_ShowString(30,130,200,12,12,datatemp);
//		memset(datatemp,'\0',sizeof(datatemp));
//		sprintf((char*)datatemp,"%02d h,%02d m,%02d s",calendar.hour,calendar.min,calendar.sec);
//		LCD_ShowString(30,150,200,12,12,datatemp);
		//printf("test\n\r");
		osDelay(500);
		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void delay_us(uint32_t udelay)
{
  uint32_t startval,tickn,delays,wait;

  startval = SysTick->VAL;
  tickn = HAL_GetTick();
  //sysc = 72000;  //SystemCoreClock / (1000U / uwTickFreq);
  delays =udelay * 72; //sysc / 1000 * udelay;
  if(delays > startval)
    {
      while(HAL_GetTick() == tickn)
        {

        }
      wait = 72000 + startval - delays;
      while(wait < SysTick->VAL)
        {

        }
    }
  else
    {
      wait = startval - delays;
      while(wait < SysTick->VAL && HAL_GetTick() == tickn)
        {

        }
    }
}

void delay_ms(uint32_t udelay)
{
	HAL_Delay(udelay);
	//osDelay(udelay);
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
