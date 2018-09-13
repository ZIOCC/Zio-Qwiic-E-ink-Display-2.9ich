//QWIICGDE029A1.c
#include"QWIICGDE029A1.h"


unsigned char full_data_head[] = {0x00,0xAB,0xBC,0xCD,0XDE,0xEF};
unsigned char part_data_head[] = {0xFE,0XED,0xDC,0xCB,0xBA,0xA0};


void QWIICGDE029A1::Full_Display(unsigned char *pic,unsigned char slave_addr) //pic's size must be 4736 for GDE029A1
{
  unsigned int i = 0,j = 0,n = 0;
  unsigned char f=0;
  Wire.beginTransmission(slave_addr); // transmit to device slave_addr
  Wire.write(full_data_head,6);
  Wire.endTransmission();    // stop transmitting

  for(i=0,n=0;i<592;i++)
  {
  Wire.beginTransmission(slave_addr); // transmit to device slave_addr
  for(j=0;j<8;j++)
  {
    f=pgm_read_word_near((pic+n));
    n++;
    Wire.write(f);
//	Serial.println(f); 
    }
   Wire.endTransmission();    // stop transmitting
  } 
//  Serial.println(n);
}

void QWIICGDE029A1::Part_Display(unsigned char xStart,unsigned char xEnd,unsigned int yStart,unsigned int yEnd,unsigned char *DisBuffer,uint16_t part_buffer_size,unsigned char RamDataEntryMode,unsigned char slave_addr)
{
  
  uint16_t i = 0,j = 0,n = 0;
  uint8_t part_buffer_size_H=0,part_buffer_size_L=0;
  uint8_t yStart_H=0,yStart_L=0,yEnd_H=0,yEnd_L=0;
  uint8_t f=0;

  part_buffer_size_H=(uint8_t)(part_buffer_size>>8);
  part_buffer_size_L=(uint8_t)part_buffer_size;

  yStart_H=(uint8_t)(yStart>>8);
  yStart_L=(uint8_t)yStart;
  yEnd_H=(uint8_t)(yEnd>>8);
  yEnd_L=(uint8_t)yEnd;

  Wire.beginTransmission(slave_addr); // transmit to device slave_addr
  Wire.write(part_data_head,6);
  Wire.endTransmission();    // stop transmitting

  
  Wire.beginTransmission(slave_addr); // transmit to device slave_addr
  Wire.write(xStart);
  Wire.write(xEnd);
  Wire.write(yStart_H);
  Wire.write(yStart_L);
  Wire.write(yEnd_H);
  Wire.write(yEnd_L);
  Wire.write(part_buffer_size_H);
  Wire.write(part_buffer_size_L);
  Wire.write(RamDataEntryMode);  // Ram data entry mode
  Wire.endTransmission();    // stop transmitting
  	
  for(i=0,n=0;i<(part_buffer_size/8);i++)    //part_buffer_size must be a multiple of 8
  {
  Wire.beginTransmission(slave_addr); // transmit to device slave_addr
  for(j=0;j<8;j++)
  {
    f=pgm_read_word_near(DisBuffer+n);
    n++;
    Wire.write(f);
//	Serial.println(f); 
    }
   Wire.endTransmission();    // stop transmitting
  }
//  Serial.println(n);
}

void QWIICGDE029A1::EPD_Print_F16x8(unsigned char X, unsigned int Y, unsigned char len, char ch[],unsigned char slave_addr)
{
  int i = 0, j = 0, k = 0, c = 0;
  unsigned char buffer = 0;
  uint16_t part_buffer_size = 0;
  uint8_t part_buffer_size_H=0,part_buffer_size_L=0;
  uint8_t yStart_H=0,yStart_L=0,yEnd_H=0,yEnd_L=0;
  uint8_t xStart=0,xEnd=0;
  uint16_t yStart=0,yEnd=0;
  uint8_t f=0;

  part_buffer_size=16*len;
  part_buffer_size_H=(uint8_t)(part_buffer_size>>8);
  part_buffer_size_L=(uint8_t)part_buffer_size;

  xStart=X;
  xEnd=xStart+15;
  yEnd=295-Y;
  yStart=yEnd-8*len+1;

  yStart_H=(uint8_t)(yStart>>8);
  yStart_L=(uint8_t)yStart;
  yEnd_H=(uint8_t)(yEnd>>8);
  yEnd_L=(uint8_t)yEnd;

  Wire.beginTransmission(slave_addr); // transmit to device slave_addr
  Wire.write(part_data_head,6);
  Wire.endTransmission();    // stop transmitting

  Wire.beginTransmission(slave_addr); // transmit to device slave_addr
  Wire.write(xStart);
  Wire.write(xEnd);  
  Wire.write(yStart_H);
  Wire.write(yStart_L);
  Wire.write(yEnd_H);
  Wire.write(yEnd_L);
  Wire.write(part_buffer_size_H);
  Wire.write(part_buffer_size_L);
  Wire.write(0x01);				// Ram data entry mode
  Wire.endTransmission();    // stop transmitting


  for (i = 0; i < len; i++)
  {
    c = ch[i] - 32;
 //   Serial.println(ch[i]);
	   
	Wire.beginTransmission(slave_addr); // transmit to device slave_addr
    for (j = 0; j < 8; j++)
    {
      buffer = pgm_read_word_near(F16x8+c * 16 + j);
      buffer = (buffer << 4) | (buffer >> 4);
      buffer = ((buffer << 2) & 0xcc) | ((buffer >> 2) & 0x33);
      buffer = ((buffer << 1) & 0xaa) | ((buffer >> 1) & 0x55);
      Wire.write(buffer);

//      Serial.print(buffer, HEX);
//	  Serial.print(", ");
    }
	
	Wire.endTransmission();    // stop transmitting
		Wire.beginTransmission(slave_addr); // transmit to device slave_addr
    for (j = 8; j < 16; j++)
    {
      buffer = pgm_read_word_near(F16x8+c * 16 + j);
      buffer = (buffer << 4) | (buffer >> 4);
      buffer = ((buffer << 2) & 0xcc) | ((buffer >> 2) & 0x33);
      buffer = ((buffer << 1) & 0xaa) | ((buffer >> 1) & 0x55);
      Wire.write(buffer);

//      Serial.print(buffer, HEX);
//	  Serial.print(", ");
    }
	
	Wire.endTransmission();    // stop transmitting
  	}

}

void QWIICGDE029A1::EPD_Print_F8x16(unsigned char X, unsigned int Y, unsigned char len, char ch[],unsigned char slave_addr)
{
  int i = 0, j = 0, k = 0, c = 0;
  unsigned char buffer = 0;
  uint16_t part_buffer_size = 0;
  uint8_t part_buffer_size_H=0,part_buffer_size_L=0;
  uint8_t yStart_H=0,yStart_L=0,yEnd_H=0,yEnd_L=0;
  uint8_t xStart=0,xEnd=0;
  uint16_t yStart=0,yEnd=0;
  uint8_t f=0;

  part_buffer_size=16*len;
  part_buffer_size_H=(uint8_t)(part_buffer_size>>8);
  part_buffer_size_L=(uint8_t)part_buffer_size;

  xStart=X;
  xEnd=xStart+8*len-1;
  yStart=Y;
  yEnd=yStart+15;
  yStart_H=(uint8_t)(yStart>>8);
  yStart_L=(uint8_t)yStart;
  yEnd_H=(uint8_t)(yEnd>>8);
  yEnd_L=(uint8_t)yEnd;

  Wire.beginTransmission(slave_addr); // transmit to device slave_addr
  Wire.write(part_data_head,6);
  Wire.endTransmission();    // stop transmitting

  Wire.beginTransmission(slave_addr); // transmit to device slave_addr
  Wire.write(xStart);
  Wire.write(xEnd);  
  Wire.write(yStart_H);
  Wire.write(yStart_L);
  Wire.write(yEnd_H);
  Wire.write(yEnd_L);
  Wire.write(part_buffer_size_H);
  Wire.write(part_buffer_size_L);
  Wire.write(0x04);          // Ram data entry mode
  Wire.endTransmission();    // stop transmitting


  for (i = 0; i < len; i++)
  {
    c = ch[i] - 32;
 //   Serial.println(ch[i]);
	   
	Wire.beginTransmission(slave_addr); // transmit to device slave_addr
    for (j = 0; j < 8; j++)
    {
      buffer = pgm_read_word_near(F8x16+c * 16 + j);
      buffer = (buffer << 4) | (buffer >> 4);
      buffer = ((buffer << 2) & 0xcc) | ((buffer >> 2) & 0x33);
      buffer = ((buffer << 1) & 0xaa) | ((buffer >> 1) & 0x55);
      Wire.write(buffer);

//      Serial.print(buffer, HEX);
//	  Serial.print(", ");
    }
	
	Wire.endTransmission();    // stop transmitting
	Wire.beginTransmission(slave_addr); // transmit to device slave_addr
    for (j = 8; j < 16; j++)
    {
      buffer = pgm_read_word_near(F8x16+c * 16 + j);
      buffer = (buffer << 4) | (buffer >> 4);
      buffer = ((buffer << 2) & 0xcc) | ((buffer >> 2) & 0x33);
      buffer = ((buffer << 1) & 0xaa) | ((buffer >> 1) & 0x55);
      Wire.write(buffer);

//      Serial.print(buffer, HEX);
//	  Serial.print(", ");
    }
	
	Wire.endTransmission();    // stop transmitting
  	}

}
