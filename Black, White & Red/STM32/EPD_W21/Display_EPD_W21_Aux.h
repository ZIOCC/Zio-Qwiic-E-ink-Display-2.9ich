
#ifndef _DISPLAY_EPD_W21_AUX_H_
#define _DISPLAY_EPD_W21_AUX_H_
#include "stm32f10x.h"

#define EPD_W21_SPI_SPEED 0x02

#define EPD_W21_MOSI_0	GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define EPD_W21_MOSI_1	GPIO_SetBits(GPIOA, GPIO_Pin_6)

#define EPD_W21_CLK_0	GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define EPD_W21_CLK_1	GPIO_SetBits(GPIOA, GPIO_Pin_5)

#define EPD_W21_CS_0	GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define EPD_W21_CS_1	GPIO_SetBits(GPIOA, GPIO_Pin_7)

#define EPD_W21_DC_0	GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define EPD_W21_DC_1	GPIO_SetBits(GPIOB, GPIO_Pin_9)

#define EPD_W21_RST_0	GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define EPD_W21_RST_1	GPIO_SetBits(GPIOB, GPIO_Pin_8)

#define EPD_W21_BS_0	GPIO_ResetBits(GPIOB, GPIO_Pin_11)

//#define EPD_W21_BUSY_LEVEL 0
#define isEPD_W21_BUSY GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_10) 


extern void driver_delay_us(unsigned int xus);
extern void driver_delay_xms(unsigned long xms);

void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char command);
void EPD_W21_WriteCMD(unsigned char command);
void EPD_W21_WriteCMD_p1(unsigned char command,unsigned char para);
void EPD_W21_WriteCMD_p2(unsigned char command,unsigned char para1,unsigned char para2);
void EPD_W21_Write(unsigned char *value, unsigned char datalen);

void EPD_W21_WriteDispRam(unsigned char XSize,unsigned int YSize,
							unsigned char *Dispbuff);

void EPD_W21_WriteDispRamMono(unsigned char XSize,unsigned int YSize,
							unsigned char dispdata);
void EPD_W21_WriteDispRamVGray4(unsigned char XSize,unsigned int YSize);
void EPD_W21_WriteDispRamHGray4(unsigned char XSize,unsigned int YSize);
void EPD_W21_WriteDispRamChessboardMono(unsigned char XSize,
										unsigned int YSize,
										unsigned char Mode);
void EPD_W21_WriteDispRamVline(unsigned char XSize,unsigned int YSize);
void EPD_W21_WriteDispRamHline(unsigned char XSize,unsigned int YSize);
void EPD_W21_WriteDispRamDotArray(unsigned char XSize,unsigned int YSize);
#endif
/***********************************************************
						end file
***********************************************************/


