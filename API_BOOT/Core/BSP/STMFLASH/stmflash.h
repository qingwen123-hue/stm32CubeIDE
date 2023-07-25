/**
 ****************************************************************************************************
 * @file        stmflash.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       STM32内部FLASH读写 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200426
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __STMFLASH_H
#define __STMFLASH_H

#include "main.h"


///* 要写入到STM32 FLASH的字符串数组 */
//const uint8_t g_text_buf[] = {"STM32 FLASH TEST"};

#define TEXT_LENTH(__buf) sizeof(__buf) /* 数组长度 */

/*SIZE表示半字�??(2字节), 大小必须�??2的整数�??, 如果不是的话, 强制对齐�??2的整数�?? */
#define BUF_FALSH_SIZE(__buf) TEXT_LENTH(__buf) / 2 + ((TEXT_LENTH(__buf) % 2) ? 1 : 0)

#define FLASH_SAVE_ADDR 0X08070000 /* 设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大�?? + 0X08000000) */

/* FLASH起始地址 */
#define STM32_FLASH_SIZE        0x80000         /* STM32 FLASH 总大小 */
#define STM32_FLASH_BASE        0x08000000      /* STM32 FLASH 起始地址 */

 /* STM32F103 扇区大小 */
#if STM32_FLASH_SIZE < 256 * 1024
#define STM32_SECTOR_SIZE   1024                /* 容量小于256K的 F103, 扇区大小为1K字节 */
#else
#define STM32_SECTOR_SIZE   2048                /* 容量大于等于于256K的 F103, 扇区大小为2K字节 */
#endif

/* FLASH解锁键值 */
#define STM32_FLASH_KEY1        0X45670123
#define STM32_FLASH_KEY2        0XCDEF89AB

/* 静态函数(仅限stmflash.c调用) */
static void stmflash_unlock(void);                  /* 解锁STM32 内部FLASH */
static void stmflash_lock(void);                    /* 锁定STM32 内部FLASH */
static uint8_t stmflash_get_error_status(void);     /* 获取FLASH错误状态 */
static uint8_t stmflash_wait_done(uint32_t time);   /* 等待操作完成 */
static uint8_t stmflash_erase_sector(uint32_t saddr);                   /* 擦除扇区 */
static uint8_t stmflash_write_halfword(uint32_t faddr, uint16_t data);  /* FLASH写半字 */

/* 接口函数(外部可调用) */
uint16_t stmflash_read_halfword(uint32_t faddr);                        /* FLASH读半字 */
void stmflash_read(uint32_t raddr, uint16_t *pbuf, uint16_t length);    /* 从指定地址开始读出指定长度的数据 */
void stmflash_write(uint32_t waddr, uint16_t *pbuf, uint16_t length);   /* 在FLASH 指定位置, 写入指定长度的数据(自动擦除) */

/* 测试函数 */
void test_write(uint32_t waddr, uint16_t wdata);

#endif


















