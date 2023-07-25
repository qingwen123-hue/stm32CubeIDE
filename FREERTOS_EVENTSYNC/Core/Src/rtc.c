/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
#include "rtc.h"

/* USER CODE BEGIN 0 */
#include "lcd.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

_calendar_obj calendar;         /* 时钟结构�?????? */
/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
  ////3.秒中�????
//  __HAL_RTC_ALARM_ENABLE_IT(&hrtc, RTC_IT_SEC);   /* 允许秒中�???? */
//  __HAL_RTC_ALARM_ENABLE_IT(&hrtc, RTC_CRH_ALRIE);
  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_IRQn);
    HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
 * @brief       RTC写入后备区域SRAM
 * @param       bkrx : 后备区寄存器编号,范围:0~41
                        对应 RTC_BKP_DR1~RTC_BKP_DR42
 * @param       data : 要写入的数据,16位长�??????
 * @retval      �??????
 */
void rtc_write_bkr(uint32_t bkrx, uint16_t data)
{
    HAL_PWR_EnableBkUpAccess(); /* 取消备份区写保护 */
    HAL_RTCEx_BKUPWrite(&hrtc, bkrx + 1, data);
}

/**
 * @brief       RTC读取后备区域SRAM
 * @param       bkrx : 后备区寄存器编号,范围:0~41
                对应 RTC_BKP_DR1~RTC_BKP_DR42
 * @retval      读取到的�??????
 */
uint16_t rtc_read_bkr(uint32_t bkrx)
{
    uint32_t temp = 0;
    temp = HAL_RTCEx_BKUPRead(&hrtc, bkrx + 1);
    return (uint16_t)temp; /* 返回读取到的�?????? */
}

/**
 * @brief       RTC时钟中断
 *   @note      秒钟中断服务函数,顺带处理闹钟标志
 *              根据RTC_CRL寄存器的 SECF �?????? ALRF 位区分是哪个中断
 * @param       �??????
 * @retval      �??????
 */
//void RTC_IRQHandler(void)
//{
//    if (__HAL_RTC_ALARM_GET_FLAG(&hrtc, RTC_FLAG_SEC) != RESET)     /* 秒中�?????? */
//    {
//    	u8 datatemp[20] = {'\0'};
//        rtc_get_time();                         /* 更新时间 */
//        __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_SEC);            /* 清除秒中�?????? */
//        //printf("sec:%d\r\n", calendar.sec);   /* 打印秒钟 */
//        sprintf((char*)datatemp,"%02d h,%02d m,%02d s",calendar.hour,calendar.min,calendar.sec);
//        LCD_ShowString(30,150,200,12,12,datatemp);
//
//    }
//
//    /* 顺带处理闹钟标志 */
//    if (__HAL_RTC_ALARM_GET_FLAG(&hrtc, RTC_FLAG_ALRAF) != RESET)   /* 闹钟标志 */
//    {
//        __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);          /* 清除闹钟标志 */
//        printf("Alarm Time:%d-%d-%d %d:%d:%d\n", calendar.year, calendar.month, calendar.date, calendar.hour, calendar.min, calendar.sec);
//    }
//
//    __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_OW);                 /* 清除溢出中断标志 */
//    while (!__HAL_RTC_ALARM_GET_FLAG(&hrtc, RTC_FLAG_RTOFF));       /* 等待RTC寄存器操作完�??????, 即等待RTOFF == 1 */
//}

/**
 * @brief       判断年份是否是闰�??????
 *   @note      月份天数�??????:
 *              月份   1  2  3  4  5  6  7  8  9  10 11 12
 *              闰年   31 29 31 30 31 30 31 31 30 31 30 31
 *              非闰�?????? 31 28 31 30 31 30 31 31 30 31 30 31
 * @param       year : 年份
 * @retval      0, 非闰�??????; 1, 是闰�??????;
 */
static uint8_t rtc_is_leap_year(uint16_t year)
{
    /* 闰年规则: 四年闰百年不闰，四百年又�?????? */
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief       设置时间, 包括年月日时分秒
 *   @note      �??????1970�??????1�??????1日为基准, �??????后累加时�??????
 *              合法年份范围�??????: 1970 ~ 2105�??????
                HAL默认为年份起点为2000�??????
 * @param       syear : 年份
 * @param       smon  : 月份
 * @param       sday  : 日期
 * @param       hour  : 小时
 * @param       min   : 分钟
 * @param       sec   : 秒钟
 * @retval      0, 成功; 1, 失败;
 */
uint8_t rtc_set_time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t seccount = 0;

    seccount = rtc_date2sec(syear, smon, sday, hour, min, sec); /* 将年月日时分秒转换成总秒钟数 */

    __HAL_RCC_PWR_CLK_ENABLE(); /* 使能电源时钟 */
    __HAL_RCC_BKP_CLK_ENABLE(); /* 使能备份域时�?????? */
    HAL_PWR_EnableBkUpAccess(); /* 取消备份域写保护 */
    /* 上面三步是必须的! */

    RTC->CRL |= 1 << 4;         /* 允许配置 */

    RTC->CNTL = seccount & 0xffff;
    RTC->CNTH = seccount >> 16;

    RTC->CRL &= ~(1 << 4);      /* 配置更新 */

    while (!__HAL_RTC_ALARM_GET_FLAG(&hrtc, RTC_FLAG_RTOFF));       /* 等待RTC寄存器操作完�??????, 即等待RTOFF == 1 */

    return 0;
}

/**
 * @brief       设置闹钟, 具体到年月日时分�??????
 *   @note      �??????1970�??????1�??????1日为基准, �??????后累加时�??????
 *              合法年份范围�??????: 1970 ~ 2105�??????
 * @param       syear : 年份
 * @param       smon  : 月份
 * @param       sday  : 日期
 * @param       hour  : 小时
 * @param       min   : 分钟
 * @param       sec   : 秒钟
 * @retval      0, 成功; 1, 失败;
 */
uint8_t rtc_set_alarm(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t seccount = 0;

    seccount = rtc_date2sec(syear, smon, sday, hour, min, sec); /* 将年月日时分秒转换成总秒钟数 */

    __HAL_RCC_PWR_CLK_ENABLE(); /* 使能电源时钟 */
    __HAL_RCC_BKP_CLK_ENABLE(); /* 使能备份域时�?????? */
    HAL_PWR_EnableBkUpAccess(); /* 取消备份域写保护 */
    /* 上面三步是必须的! */

    RTC->CRL |= 1 << 4;         /* 允许配置 */

    RTC->ALRL = seccount & 0xffff;
    RTC->ALRH = seccount >> 16;

    RTC->CRL &= ~(1 << 4);      /* 配置更新 */

    while (!__HAL_RTC_ALARM_GET_FLAG(&hrtc, RTC_FLAG_RTOFF));       /* 等待RTC寄存器操作完�??????, 即等待RTOFF == 1 */

    return 0;
}

/**
 * @brief       得到当前的时�??????
 *   @note      该函数不直接返回时间, 时间数据保存在calendar结构体里�??????
 * @param       �??????
 * @retval      �??????
 */
void rtc_get_time(void)
{
    static uint16_t daycnt = 0;
    uint32_t seccount = 0;
    uint32_t temp = 0;
    uint16_t temp1 = 0;
    const uint8_t month_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; /* 平年的月份日期表 */

    seccount = RTC->CNTH;       /* 得到计数器中的�??(秒钟�??????) */
    seccount <<= 16;
    seccount += RTC->CNTL;

    temp = seccount / 86400;    /* 得到天数(秒钟数对应的) */

    if (daycnt != temp)         /* 超过�??????天了 */
    {
        daycnt = temp;
        temp1 = 1970;           /* �??????1970年开�?????? */

        while (temp >= 365)
        {
            if (rtc_is_leap_year(temp1)) /* 是闰�?????? */
            {
                if (temp >= 366)
                {
                    temp -= 366; /* 闰年的秒钟数 */
                }
                else
                {
                    break;
                }
            }
            else
            {
                temp -= 365;    /* 平年 */
            }

            temp1++;
        }

        calendar.year = temp1;  /* 得到年份 */
        temp1 = 0;

        while (temp >= 28)      /* 超过了一个月 */
        {
            if (rtc_is_leap_year(calendar.year) && temp1 == 1) /* 当年是不是闰�??????/2月份 */
            {
                if (temp >= 29)
                {
                    temp -= 29; /* 闰年的秒钟数 */
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (temp >= month_table[temp1])
                {
                    temp -= month_table[temp1]; /* 平年 */
                }
                else
                {
                    break;
                }
            }

            temp1++;
        }

        calendar.month = temp1 + 1; /* 得到月份 */
        calendar.date = temp + 1;   /* 得到日期 */
    }

    temp = seccount % 86400;                                                    /* 得到秒钟�?????? */
    calendar.hour = temp / 3600;                                                /* 小时 */
    calendar.min = (temp % 3600) / 60;                                          /* 分钟 */
    calendar.sec = (temp % 3600) % 60;                                          /* 秒钟 */
    calendar.week = rtc_get_week(calendar.year, calendar.month, calendar.date); /* 获取星期 */
}

/**
 * @brief       将年月日时分秒转换成秒钟�??????
 *   @note      输入公历日期得到星期(起始时间�??????: 公元0�??????3�??????1日开�??????, 输入�??????后的任何日期, 都可以获取正确的星期)
 *              使用 基姆拉尔森计算公�?????? 计算, 原理说明见此�??????:
 *              https://www.cnblogs.com/fengbohello/p/3264300.html
 * @param       syear : 年份
 * @param       smon  : 月份
 * @param       sday  : 日期
 * @retval      0, 星期�??????; 1 ~ 6: 星期�?????? ~ 星期�??????
 */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t week = 0;

    if (month < 3)
    {
        month += 12;
        --year;
    }

    week = (day + 1 + 2 * month + 3 * (month + 1) / 5 + year + (year >> 2) - year / 100 + year / 400) % 7;
    return week;
}

/**
 * @brief       将年月日时分秒转换成秒钟�??????
 *   @note      �??????1970�??????1�??????1日为基准, 1970�??????1�??????1�??????, 0�??????0�??????0�??????, 表示�??????0秒钟
 *              �??????大表示到2105�??????, 因为uint32_t�??????大表�??????136年的秒钟�??????(不包括闰�??????)!
 *              本代码参考只linux mktime函数, 原理说明见此�??????:
 *              http://www.openedv.com/thread-63389-1-1.html
 * @param       syear : 年份
 * @param       smon  : 月份
 * @param       sday  : 日期
 * @param       hour  : 小时
 * @param       min   : 分钟
 * @param       sec   : 秒钟
 * @retval      转换后的秒钟�??????
 */
static long rtc_date2sec(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t Y, M, D, X, T;
    signed char monx = smon;    /* 将月份转换成带符号的�??????, 方便后面运算 */

    if (0 >= (monx -= 2))       /* 1..12 -> 11,12,1..10 */
    {
        monx += 12;             /* Puts Feb last since it has leap day */
        syear -= 1;
    }

    Y = (syear - 1) * 365 + syear / 4 - syear / 100 + syear / 400; /* 公元元年1到现在的闰年�?????? */
    M = 367 * monx / 12 - 30 + 59;
    D = sday - 1;
    X = Y + M + D - 719162;                      /* 减去公元元年�??????1970年的天数 */
    T = ((X * 24 + hour) * 60 + min) * 60 + sec; /* 总秒钟数 */
    return T;
}


void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *nhrtc)
{
	u8 datatemp[20] = {'\0'};
	rtc_get_time();
	sprintf((char*)datatemp,"%02d h,%02d m,%02d s",calendar.hour,calendar.min,calendar.sec);
	LCD_ShowString(30,150,200,12,12,datatemp);
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *nhrtc)
{
	HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
	LCD_ShowString(30,170,200,12,12,"Alarm OK!");
}
/* USER CODE END 1 */
