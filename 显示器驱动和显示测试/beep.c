#include "beep.h" 
#include "delay.h"
#include "usart.h"

u8 timer=1,i=0;
u8 DSTART=0,BMPY=0;
u16 PICH=0,PICW=0;
u32 NUM=0;

void OLED_CMD(u8 cmd)
{
	NCS=0,PNC=0,WRB=0;
	//delay_us(timer);
	DATA_OUT=(u16)cmd<<8;
	delay_us(timer);
	WRB=1;
	//delay_us(timer);
	//NCS=1;
}

void OLED_PARA(u8 data)
{
	NCS=0,PNC=1,WRB=0;
	//delay_us(timer);
	DATA_OUT=(u16)data<<8;
	delay_us(timer);
	WRB=1;
	//delay_us(timer);
	//NCS=1;
}

void OLED_INIT(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOFʱ��
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//OPRN DRAIN
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_9|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO
	
  GPIO_SetBits(GPIOD,GPIO_Pin_9|GPIO_Pin_15); 
	
	OLED_CMD(oled_reset);
	delay_ms(50);
	OLED_DON(0);
	OLED_CMD(oled_standby);
	OLED_PARA(0x00);	// 0: start 1:stop
	
	OLED_CMD(oled_mode);
	OLED_PARA(0x03);
	
	OLED_CMD(oled_Ddirection);
	OLED_PARA(0x00);
	OLED_CMD(oled_Wdirection);
	OLED_PARA(0x02);
	
	OLED_DSET(0,0,0,127,0,127);//MEMERY TO DIS
	
	OLED_CMD(oled_datamark);
	OLED_PARA(0x07);
	
///////////one dot timing//////////////
/////precw:0 - peak delay:1 - peak boot:2 - dot current:1 ////////
	
	//OLED_DOTSET(3,0,3,15,6,120,50,100);
	OLED_DOTSET(2,0,3,15,5,64,26,49);
	
	OLED_CMD(oled_framerate);
	OLED_PARA(0x00);
	
	OLED_CMD(oled_precselect);
	OLED_PARA(0x03);
	//row scan
	OLED_CMD(0x17);
	OLED_PARA(0x00);
	//row overclap
	OLED_CMD(0x48);
	OLED_PARA(0x03);
	
//	OLED_AREA(0,127,0,127);
	
	delay_ms(100);
  OLED_DON(1);
}

void OLED_DSET(u8 dxs,u8 dys,u8 sizexs,u8 sizexe,u8 sizeys,u8 sizeye)
{	
	OLED_CMD(oled_diss_x);
	OLED_PARA(dxs);
	OLED_CMD(oled_diss_y);
	OLED_PARA(dys);
	
	OLED_CMD(oled_size_x);
	OLED_PARA(sizexs);
	OLED_PARA(sizexe);
	OLED_CMD(oled_size_y);
	OLED_PARA(sizeys);
	OLED_PARA(sizeye);
}

void OLED_AREA(u8 xs,u8 xe,u8 ys,u8 ye)
{
	OLED_CMD(oled_boxs_x);
	OLED_PARA(xs);
	OLED_CMD(oled_boxe_x);
	OLED_PARA(xe);	
	OLED_CMD(oled_boxs_y);
	OLED_PARA(ys);
	OLED_CMD(oled_boxe_y);
	OLED_PARA(ye);	
}

void OLED_WAREA(u8 xs,u8 xe,u8 ys,u8 ye,u16 num,u8 *data)
{
	u16 i=0;
	OLED_AREA(xs,xe,ys,ye);
	OLED_CMD(oled_write);
	for(i=0;i<num;i++)
	{
		//OLED_PARA(data[i]);
		OLED_PARA(data[i]/8); i++;
		OLED_PARA(data[i]/4); i++;
		OLED_PARA(data[i]/8);
	}
}

void OLED_fill(u8 xs,u8 xe,u8 ys,u8 ye,u16 num,u32 data)
{
	u32 i=0;
	OLED_AREA(xs,xe,ys,ye);
	OLED_CMD(oled_write);
	for(i=0;i<(xe-xs+1)*(ye-ys+1)*3;i++)
	{
		//OLED_PARA(data[i]);
		OLED_PARA(data); i++;
		OLED_PARA(data>>8); i++;
		OLED_PARA(data>>16);
	}
}


void OLED_DON(u8 sw)
{
	OLED_CMD(oled_onoff);
	OLED_PARA(sw); // 0: off 1: on
}

void OLED_DOTSET(u8 precw,u8 delay,u8 RW,u8 GW,u8 BW,u8 RC,u8 GC,u8 BC)
{
	OLED_CMD(oled_precwidth);
	OLED_PARA(precw);
	
	OLED_CMD(0x16); //peak delay
	OLED_PARA(delay);
	
	RANGE(RW,31);RANGE(GW,31);RANGE(BW,31);
	RANGE(RC,255);RANGE(GC,255);RANGE(BC,255);
	OLED_CMD(0x3a);
	OLED_PARA(RW);
	OLED_CMD(0x3b);
	OLED_PARA(GW);
	OLED_CMD(0x3c);
	OLED_PARA(BW);
	
	OLED_CMD(0x40);
	OLED_PARA(RC);
	OLED_CMD(0x41);
	OLED_PARA(GC);
	OLED_CMD(0x42);
	OLED_PARA(BC);
}


void OLED_CLEAR(void)
{
	u16 i=0;
	OLED_AREA(0,127,0,127);
	OLED_CMD(oled_write);
	for(i=0;i<128*128*3;i++) OLED_PARA(0);
}

void RANGE(u16 target,u16 max)
{
	if(target>max) target=max;
}

void NUM2CHAR(u16 num)
{
	
}

typedef __packed struct
{
    u32 biSize ;		   	//˵��BITMAPINFOHEADER�ṹ����Ҫ��������
    long  biWidth ;		   	//˵��ͼ��Ŀ�ȣ�������Ϊ��λ 
    long  biHeight ;	   	//˵��ͼ��ĸ߶ȣ�������Ϊ��λ 
    u16  biPlanes ;	   		//ΪĿ���豸˵��λ��������ֵ�����Ǳ���Ϊ1 
    u16  biBitCount ;	   	//˵��������/���أ���ֵΪ1��4��8��16��24����32
    u32 biCompression ;  	//˵��ͼ������ѹ�������͡���ֵ����������ֵ֮һ��
	//BI_RGB��û��ѹ����
	//BI_RLE8��ÿ������8���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����(�ظ����ؼ�������ɫ����)��  
    //BI_RLE4��ÿ������4���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����
  	//BI_BITFIELDS��ÿ�����صı�����ָ�������������
    u32 biSizeImage ;		//˵��ͼ��Ĵ�С�����ֽ�Ϊ��λ������BI_RGB��ʽʱ��������Ϊ0  
    long  biXPelsPerMeter ;	//˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ
    long  biYPelsPerMeter ;	//˵����ֱ�ֱ��ʣ�������/�ױ�ʾ
    u32 biClrUsed ;	  	 	//˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ������
    u32 biClrImportant ; 	//˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ�� 
}BITMAPINFOHEADER ;
//BMPͷ�ļ�
typedef __packed struct
{
    u16  bfType ;     //�ļ���־.ֻ��'BM',����ʶ��BMPλͼ����
    u32  bfSize ;	  //�ļ���С,ռ�ĸ��ֽ�
    u16  bfReserved1 ;//����
    u16  bfReserved2 ;//����
    u32  bfOffBits ;  //���ļ���ʼ��λͼ����(bitmap data)��ʼ֮��ĵ�ƫ����
}BITMAPFILEHEADER ;

typedef __packed struct
{ 
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;  
	u32 RGB_MASK[3];			//��ɫ�����ڴ��RGB����.
	//RGBQUAD bmiColors[256];  
}BITMAPINFO;


void USART_BMP_INFO(u8 *data)
{
	BITMAPINFO *pbmp;
	pbmp=(BITMAPINFO*)data;
	if(pbmp->bmfHeader.bfType==0x4d42)
	{
		DSTART=pbmp->bmfHeader.bfOffBits;
		NUM=pbmp->bmiHeader.biSizeImage;
		PICW=pbmp->bmiHeader.biWidth;
		PICH=pbmp->bmiHeader.biHeight;
    BMPY=1;	
		printf("..bmp..");
	}
}

