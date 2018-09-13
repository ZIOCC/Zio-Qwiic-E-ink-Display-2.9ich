
#include "stm32f10x.h"
#include "config.h"
#define  ClockSpeed      400000
extern EventStatus i2c_event;
extern EventStatus i2c_event;
ErrorStatus HSEStartUpStatus;
NVIC_InitTypeDef NVIC_InitStructure;
EventStatus i2c_event= NOEVENT;
I2C_InitTypeDef   I2C_InitStructure;
extern u8 I2C_SLAVE_ADDRESS7;
extern unsigned char Black_Data_I2C1_Buffer_Rx[5000];
extern unsigned char Red_Data_I2C1_Buffer_Rx[5000];
void driver_delay_us(unsigned int xus)
{
	for(;xus>1;xus--);
}

void driver_delay_xms(unsigned long xms)	
{	
    unsigned long i = 0 , j=0;

    for(j=0;j<xms;j++)
	{
        for(i=0; i<256; i++);
    }
}

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

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5 |GPIO_Pin_6;    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	 //PA.0
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;//input
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//Init IO
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1 | GPIO_Pin_2;	 //PA.1,2
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_8 |GPIO_Pin_9;    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3;	 //PB.3
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;
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
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 

	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}
void I2C_Init_Config(void)
{
	I2C_InitTypeDef I2C_InitStructure; 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
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
void Sys_Standby(void)
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	
	PWR_WakeUpPinCmd(ENABLE); 
	PWR_EnterSTANDBYMode();	  
}

void Sys_Enter_Standby(void)
{			 
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	
	Sys_Standby();
}
void WKUP_Init(void)
{	  		  
	EXTI_InitTypeDef EXTI_InitStructure;

    
	EXTI_ClearITPendingBit(EXTI_Line0);	
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	
	
	EXTI_ClearITPendingBit(EXTI_Line2);
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);	
	
}

void pic_display_black(void)
{
	EPD_W21_Init();

	EPD_W21_WriteCMD(0x06);         //boost设定
	EPD_W21_WriteDATA (0x17);
	EPD_W21_WriteDATA (0x17);
	EPD_W21_WriteDATA (0x17);

	EPD_W21_WriteCMD(0x04);         //上电 
	lcd_chkstatus();                //查看芯片状态  

	EPD_W21_WriteCMD(0X00);
	EPD_W21_WriteDATA(0x8f);		//选择分辨率128x296

	EPD_W21_WriteCMD(0X50);
	EPD_W21_WriteDATA(0x77);

	EPD_W21_WriteCMD(0x61);			//像素设定
	EPD_W21_WriteDATA (0x80);       //200像素	 
	EPD_W21_WriteDATA (0x01);		//300像素 
	EPD_W21_WriteDATA (0x28);
	
	EPD_W21_WriteCMD(0x10);			//开始传输黑白图像
	Ultrachip();
}

void pic_display_black_red(void)
{
	EPD_W21_Init();

	EPD_W21_WriteCMD(0x06);         //boost设定
	EPD_W21_WriteDATA (0x17);
	EPD_W21_WriteDATA (0x17);
	EPD_W21_WriteDATA (0x17);

	EPD_W21_WriteCMD(0x04);         //上电 
	lcd_chkstatus();                //查看芯片状态  

	EPD_W21_WriteCMD(0X00);
	EPD_W21_WriteDATA(0x8f);		//选择分辨率128x296

	EPD_W21_WriteCMD(0X50);
	EPD_W21_WriteDATA(0x77);

	EPD_W21_WriteCMD(0x61);			//像素设定
	EPD_W21_WriteDATA (0x80);       //200像素	 
	EPD_W21_WriteDATA (0x01);		//300像素 
	EPD_W21_WriteDATA (0x28);
	
	EPD_W21_WriteCMD(0x10);			//开始传输黑白图像
	Ultrachip();
	
	EPD_W21_WriteCMD(0x13);			//开始传输红图像
	Ultrachip_red();
}
void Ultrachip(void)
{
	unsigned int i;
	for(i=0;i<4736;i++)	     
		{EPD_W21_WriteDATA(Black_Data_I2C1_Buffer_Rx[i]);}  
	driver_delay_xms(2);		     
} 
void Ultrachip_red(void)
{
	unsigned int i;
	for(i=0;i<4736;i++)	     
	{EPD_W21_WriteDATA(~Red_Data_I2C1_Buffer_Rx[i]);}  
	driver_delay_xms(2);
}


void lcd_chkstatus(void)
{
	while(1)
  	{	 
     if(isEPD_W21_BUSY==0) break;
  	}   
	driver_delay_xms(20000);                       
}


