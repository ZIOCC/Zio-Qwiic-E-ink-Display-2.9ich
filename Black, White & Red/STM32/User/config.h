/**
  ******************************************************************************
  * @file IAPOverI2C/inc/main.h
  * @author  MCD Application Team
  * @version  V1.0.0
  * @date     09/15/2010
  * @brief  Header file for main.c
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONFIG_H
#define __CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "main.h"
#include "Display_EPD_W21.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_pwr.h"

#include "Display_EPD_W21_Aux.h"
typedef enum {NOEVENT = 0, EVENT_OPCOD_NOTYET_READ = 1,EVENT_OPCOD_READ =2} EventStatus;
void driver_delay_us(unsigned int xus);
void driver_delay_xms(unsigned long xms);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void I2C_Init_Config(void);
void Sys_Standby(void);
void Sys_Enter_Standby(void);
void WKUP_Init(void);
void lcd_chkstatus(void);
void lut_red(void);
void lut_bw(void);
void Ultrachip_red(void);
void Ultrachip(void);
void master_IL0373F_init(void);
void pic_display_black(void);
void pic_display_black_red(void);
#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
