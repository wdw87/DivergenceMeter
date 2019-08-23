#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define INPUT PBout(12)// PB5
#define LCLK PBout(13)// PE5	
#define SCLK PBout(14)// PE5	

void LED_Init(void);//��ʼ��
void HC595_write_byte(u16 byte);
void HC595_Latch_Output(void);
void HC595_display_on(u8 hour,u8 min,u8 dot1,u8 dot2,u8 dot3,u8 dot4);
void HC595_display_off(void);
		 				    
#endif
