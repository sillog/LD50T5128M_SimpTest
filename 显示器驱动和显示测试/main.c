#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"

typedef  struct
{ 
	u8 XS;
	u8 XE;
	u8 YS;
	u8 YE;
	u16 NUM;
	u16 COLOR;
}WAREA;

int main(void)
{ 
	u8 XS=0,XE=0,YS=0,YE=0,data[2]={'a','b'},SETAREA=0;
	u16 i=0,j=0,k=1,tmp=0;
	u32 num=0;
	WAREA *fill;
  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);		
	uart_init(460800);	
	OLED_INIT();
	delay_ms(100);
	OLED_DSET(0,0,0,127,0,127);
	//OLED_WAREA(0,127,0,127,128*128*3,USART_RX_BUF);
	
	while(1)
	{
	//while(FLAG==0XC0)
		USART_RX_BUF[0]=0X00;USART_RX_BUF[1]=0X7F;USART_RX_BUF[2]=0X00;USART_RX_BUF[3]=0X7F;
		USART_RX_BUF[4]=0X00;USART_RX_BUF[5]=0X01;
		USART_RX_BUF[6]=0X1f;USART_RX_BUF[7]=0X02;USART_RX_BUF[8]=0X03;USART_RX_BUF[9]=0X04;
	{		
		//printf("=====hello=====");
		fill=(WAREA *)USART_RX_BUF;
		OLED_fill(fill->XS,fill->XE,fill->YS,fill->YE,fill->NUM,fill->COLOR);
		
		if(BMPY==1)
		{		
			if(SETAREA==0)
			{
				OLED_CLEAR();
				if(PICW>128) XE=127;
				else XE=PICW-1;
				if(PICH>128) YE=127;
				else YE=PICH-1;
				OLED_AREA(XS,XE,YS,YE);
				OLED_CMD(oled_write);

				for(i=DSTART;i<USART_RX_STA;i++)
				{
					OLED_PARA(USART_RX_BUF[i]/8);
				}
				SETAREA=1;
				//num+=USART_RX_STA-DSTART;
				//if(num>=NUM) 
				{
					BMPY=0;
					SETAREA=0;
					num=0;
					printf("..displayok..");
				}
			}
			else
			{
				for(i=0;i<USART_RX_STA;i++)
				{
					OLED_PARA(USART_RX_BUF[i]/8);
				}
				num+=USART_RX_STA;
				if(num>=NUM) 
				{
					BMPY=0;
					SETAREA=0;
					num=0;
					printf("..displayok..");
				}
			}		
		}
		
		FLAG=0;
		USART_RX_STA=0;
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	}
}
}