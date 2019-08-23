#include "led.h"
#include "delay.h"

u16 nums[11] = {0x0400,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0000};
u16 dots[4] = {0x0000,0x0001,0x0800,0x0801};

//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_ResetBits(GPIOB,GPIO_Pin_12);						 //PB.5 输出高
	 GPIO_ResetBits(GPIOB,GPIO_Pin_13);						 //PB.5 输出高
	 GPIO_ResetBits(GPIOB,GPIO_Pin_14);						 //PB.5 输出高

}
void HC595_Latch_Output(void)
{
	LCLK = 0;
	delay_us(2);
	LCLK = 1;
	delay_us(2);
	LCLK = 0;
	delay_us(2);
	
}
	
void HC595_write_byte(u16 byte)
{
	u8 t = 0;
	for(t = 0;t<12;t++)
	{
		if((byte&0x800)>>11)
			INPUT = 1;
		else
			INPUT = 0;
		delay_us(2);
//		SCLK = 0;
//		delay_us(2);
		SCLK = 1;
		delay_us(2);
		SCLK = 0;
		byte<<=1; 
	}
}
void HC595_display_on(u8 hour,u8 min,u8 dot1,u8 dot2,u8 dot3,u8 dot4)
{
	int num1,num2,num3,num4;
	num1 = min%10;
	num2 = min/10%10;
	num3 = hour%10;
	num4 = hour/10%10;
	HC595_write_byte(nums[num1] + dots[dot4]);
	HC595_write_byte(nums[num2] + dots[dot3]);
	HC595_write_byte(nums[num3] + dots[dot2]);
	HC595_write_byte(nums[num4] + dots[dot1]);
	
	
	HC595_Latch_Output();
}
void HC595_display_off(void)
{
	int i = 0;
	for(i = 0; i < 4 ; i++)
	{
		HC595_write_byte(0x0000);
	}
	HC595_Latch_Output();
}

	
 
