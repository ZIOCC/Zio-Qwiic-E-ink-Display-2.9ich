
/*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "main.h"
#include "Display_EPD_W21.h"
#include "stm32f10x_i2c.h"
#include "string.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

ErrorStatus HSEStartUpStatus;
NVIC_InitTypeDef NVIC_InitStructure;
EventStatus i2c_event= NOEVENT;
I2C_InitTypeDef   I2C_InitStructure;
/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void I2C_Init_Config(void);

#define  ClockSpeed      400000
#define xDot 128
#define yDot 296
extern EventStatus i2c_event;
unsigned char  I2C1_Buffer_Tx[100];
unsigned char Full_Data_I2C1_Buffer_Rx[5000];
unsigned char Part_Data_I2C1_Buffer_Rx[5000];
unsigned char Part_Cmd_I2C1_Buffer_Rx[9]={0,0,0,0,0,0,0,0,0};
extern  uint16_t Tx_Idx , Rx_Idx;
unsigned char Full_Display_Flag = 0,Part_Display_Flag = 0;
uint16_t Part_DisBuffer_Size=0;
u8 ii=0;
u16 harware=0;
u16 y_start=0,y_end=0;
u8 I2C_SLAVE_ADDRESS7 = 0xA7;
u8 clear_buffer[4736]={0xFF};
extern unsigned char RamDataEntryMode[2];	// Ram data entry mode
int main(void)
{
	memset(clear_buffer,0xFF,4736);
	#ifdef DEBUG
	  debug();
	#endif

	RCC_Configuration();

	GPIO_Configuration();
	NVIC_Configuration();
	harware=GPIO_ReadInputData(GPIOB);
	I2C_SLAVE_ADDRESS7=((harware&0x07)|0xA0)<<1;
	I2C_Init_Config();

//	EPD_init_Full();
//	driver_delay_xms(300);
	while(1)
	{
	
	if(Full_Display_Flag==1)
	{
		Full_Display_Flag=0;
		EPD_init_Full();
		driver_delay_xms(300);
		EPD_Dis_Full((unsigned char *)Full_Data_I2C1_Buffer_Rx,1);  //	pic		
		driver_delay_xms(1000);
//		__WFI();         //STM32 SLEEP MODE
	}
	else if(Part_Display_Flag==1)
	{
		if(RamDataEntryMode[1]!=Part_Cmd_I2C1_Buffer_Rx[8])
		{
		RamDataEntryMode[1]=Part_Cmd_I2C1_Buffer_Rx[8];
		EPD_init_Part();
		driver_delay_xms(300);
		}
		y_start=((u16)Part_Cmd_I2C1_Buffer_Rx[2]<<8)|(u16)Part_Cmd_I2C1_Buffer_Rx[3];
		y_end=((u16)Part_Cmd_I2C1_Buffer_Rx[4]<<8)|(u16)Part_Cmd_I2C1_Buffer_Rx[5];
		EPD_Dis_Part(Part_Cmd_I2C1_Buffer_Rx[0],Part_Cmd_I2C1_Buffer_Rx[1],y_start,y_end,(unsigned char *)clear_buffer,1);
		driver_delay_xms(600);
		EPD_init_Part();
	  driver_delay_xms(300);
		
		EPD_Dis_Part(Part_Cmd_I2C1_Buffer_Rx[0],Part_Cmd_I2C1_Buffer_Rx[1],y_start,y_end,(unsigned char *)Part_Data_I2C1_Buffer_Rx,1);
		driver_delay_xms(600);
		Part_Display_Flag=0;
		Part_DisBuffer_Size=0;
		RamDataEntryMode[1]=0x01;
		for(ii=0;ii<9;ii++)
			{
				Part_Cmd_I2C1_Buffer_Rx[ii]=0;
			}
//		__WFI();         //STM32 SLEEP MODE
	}
	else
	{		
		EPD_W21_POWEROFF(); //POWER OFF
		EPD_W21_SLEEP(); //SLEEP MODE
		
	}
	}
}


/***********************************************************
						end file
***********************************************************/

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
 
  RCC_DeInit();
 

  RCC_HSEConfig(RCC_HSE_ON);
  

  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus == SUCCESS)
  {
    // PLLCLK = 8MHz * 9 = 72 MHz 
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
  }
  else {

    RCC_HSICmd(ENABLE);

    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);


    RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_16);
  }


  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);


  FLASH_SetLatency(FLASH_Latency_2);
	

  RCC_HCLKConfig(RCC_SYSCLK_Div1); 


  RCC_PCLK2Config(RCC_HCLK_Div1); 


  RCC_PCLK1Config(RCC_HCLK_Div2);
  

  RCC_PLLCmd(ENABLE);
  

  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
  

  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  

  while(RCC_GetSYSCLKSource() != 0x08);
    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
            RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO,
            ENABLE);
}

/*******************************************************************************
* Function       : GPIO_Configuration
* Description    : Configures GPIO
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5 |GPIO_Pin_6;    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_8 |GPIO_Pin_9;    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2;    //input
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{ 
  //NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure and enable I2Cx event interrupt -------------------------------*/  
	  NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
  
    /* Configure and enable I2C1 error interrupt -------------------------------*/  
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
    NVIC_Init(&NVIC_InitStructure); 
}
void I2C_Init_Config()
{
	I2C_InitTypeDef I2C_InitStructure; 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	/*GPIO?IIC??????*/  
	I2C_DeInit(I2C1);
    /* I2C1 configuration ------------------------------------------------------*/
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
    I2C_Init(I2C1, &I2C_InitStructure);
		I2C_Cmd(I2C1, ENABLE ); 
		I2C_ITConfig(I2C1,I2C_IT_EVT|I2C_IT_BUF,ENABLE); 
		I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE); 
}

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






