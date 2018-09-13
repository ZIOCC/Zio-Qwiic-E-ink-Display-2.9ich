
/*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "main.h"
#include "Display_EPD_W21.h"
#include "stm32f10x_i2c.h"
#include "string.h"
#include "config.h"
#include "stm32f10x_pwr.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



#define xDot 128
#define yDot 296

unsigned char  I2C1_Buffer_Tx[100];
unsigned char Black_Data_I2C1_Buffer_Rx[5000];
unsigned char Red_Data_I2C1_Buffer_Rx[5000];
unsigned char Black_Display_Flag = 0,Red_Display_Flag = 0;
u8 ii=0;
u8 l=0;
u16 harware=0;
u16 y_start=0,y_end=0;
u8 I2C_SLAVE_ADDRESS7 = 0xA7;
u8 clear_buffer[4736]={0xFF};
extern unsigned char RamDataEntryMode[2];	// Ram data entry mode
unsigned char  EPD_W21_ReadDATA(void);

int main(void)
{
	memset(clear_buffer,0xFF,4736);
	#ifdef DEBUG
	  debug();
	#endif

	RCC_Configuration();

	GPIO_Configuration();
	WKUP_Init();
	NVIC_Configuration();
	harware=GPIO_ReadInputData(GPIOB);
	I2C_SLAVE_ADDRESS7=((harware&0x07)|0xA0)<<1;
	I2C_Init_Config();
	
	EPD_W21_Init();

	while(1)
	{
		if(Black_Display_Flag==1)
		{
			Black_Display_Flag=0;
			Red_Display_Flag=0;
			pic_display_black();
			EPD_W21_WriteCMD(0x12);              
		  lcd_chkstatus();
			driver_delay_xms(300000);	  //wait for fresh display
			EPD_W21_WriteCMD(0x02);			//power off
      lcd_chkstatus();
			EPD_W21_WriteCMD(0X07);			//deep sleep
			EPD_W21_WriteDATA(0xA5);
			driver_delay_xms(40000);
		}
		else 
			if(Red_Display_Flag==1)
		{
			l=10;
			Black_Display_Flag=0;
			Red_Display_Flag=0;
			pic_display_black_red();
			EPD_W21_WriteCMD(0x12);              
		  lcd_chkstatus();
			driver_delay_xms(300000);	  //wait for fresh display
			EPD_W21_WriteCMD(0x02);			//power off
      lcd_chkstatus();
			EPD_W21_WriteCMD(0X07);			//deep sleep
			EPD_W21_WriteDATA(0xA5);
			driver_delay_xms(40000);
		}
		else{};
	
	}
}


/***********************************************************
						end file
***********************************************************/




#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif






