#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "rtc.h" 
#include "adc.h"
#include "key.h"
#include "timer.h"
 
/************************************************
 ALIENTEK战舰STM32开发板实验1
 跑马灯实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


void display_test()
{
			HC595_write_byte(1<<0);
		HC595_Latch_Output();
		delay_ms(500);
		HC595_write_byte(1<<1);
		HC595_Latch_Output();
		delay_ms(500);
		HC595_write_byte(1<<2);
		HC595_Latch_Output();
		delay_ms(500);
		HC595_write_byte(1<<3);
		HC595_Latch_Output();
		delay_ms(500);
		HC595_write_byte(1<<4);
		HC595_Latch_Output();
		delay_ms(500);
		HC595_write_byte(1<<5);
		HC595_Latch_Output();
		delay_ms(500);
		HC595_write_byte(1<<6);
		HC595_Latch_Output();
		delay_ms(500);
		HC595_write_byte(1<<7);
		HC595_Latch_Output();
		delay_ms(500);
		HC595_write_byte(1<<8);
		HC595_Latch_Output();
		delay_ms(500);
		HC595_write_byte(1<<9);
		HC595_Latch_Output();
		delay_ms(500);
		HC595_write_byte(1<<10);
		HC595_Latch_Output();
		delay_ms(500);
		HC595_write_byte(1<<11);
		HC595_Latch_Output();
		delay_ms(500);
 
}
u8 My_Rand()
{
	u8 i;
	u8 rand_temp=0,rand=0;
	u16 adcx;
	for(i=0;i<8;i++)
	{
		adcx=Get_Adc(ADC_Channel_1);
		rand_temp=adcx&0x0001;
		rand=(rand<<1)+rand_temp;
	}
	return rand;
}
		int sec,min,hour;
int status = 0;

 int main(void)
 {	
		u8 t = 0;
	 u8 i;
	 u8 key;
	 u8 num[4] = {0,0,0,0},set_no;
	 u32 cnt;
	 u32 start_time = 0;
	 u32 show_time = 0;
		delay_init();	    //延时函数初始化	  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
		LED_Init();		  	//初始化与LED连接的硬件接口
		RTC_Init();	  			//RTC初始化
		Adc_Init();		  		//ADC初始化
	 TIM3_Int_Init(9,7199);//10Khz的计数频率，计数到10为1ms  
	 KEY_Init();
		//RTC_Set(2019,8,9,18,22,50);
	while(1)
	{
		cnt++;
		delay_ms(100);
		if(status == 0) //动画
		{
			for(i = 0;i<32;i++)
			{
				HC595_display_on(My_Rand(),My_Rand(),0,0,0,0);
				delay_ms(50);
			}
			
			status = 3;
		}
		else if(status ==1) //黑暗等待
		{
			if(t != calendar.sec)
			{
				t = calendar.sec;
				HC595_display_off();
			}
			key=KEY_Scan(0);	//得到键值
			if(key)
			{						   
				switch(key)
				{				 
					case KEY0_PRES:	//进入调时模式
						status = 0;
						break;
					case KEY1_PRES:	//控制LED1翻转	 
						status = 3;
						break;
					case KEY2_PRES:
						status = 2;
						break;
				}
			}else delay_ms(10); 
		}
		else if(status ==2) //显示时间
		{
			show_time = core_counter;			
			status =5;
		}
		else if(status ==5) //显示时间
		{
//			display_test();
			if(core_counter - show_time > 1000*60*5)
			{
				status = 1;
			}
			if(t != calendar.sec)
			{
				t = calendar.sec;
				HC595_display_on(calendar.hour,calendar.min,0,2,1,0);
				start_time = core_counter;
			}
			if(core_counter - start_time > 500){
				HC595_display_on(calendar.hour,calendar.min,0,0,0,0);
			}
			key=KEY_Scan(0);	//得到键值
			if(key)
			{						   
				switch(key)
				{				 
					case KEY0_PRES:	//进入调时模式
						num[0] = calendar.hour/10;
						num[1] = calendar.hour%10;
						num[2] = calendar.min/10;
						num[3] = calendar.min%10;
						set_no = 0;
						cnt =0;
						status = 4;
						break;
					case KEY1_PRES:	//控制LED1翻转	 
						status = 0;
						break;
					case KEY2_PRES:
						status = 3;
						break;
				}
				
			}
			
			
		}
		else if(status == 3) //世界线显示
		{
			HC595_display_on(7,8,2,0,0,0);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			status = 2;
		}
		else if(status == 4) //调时模式
		{
			switch(set_no)
			{				 
				case 0:	
					HC595_display_on(num[0]*10 + num[1],num[2]*10 + num[3],3,0,0,0);
					break;
				case 1:	
					HC595_display_on(num[0]*10 + num[1],num[2]*10 + num[3],0,3,0,0);
					break;
				case 2:	
					HC595_display_on(num[0]*10 + num[1],num[2]*10 + num[3],0,0,3,0);
					break;
				case 3:	
					HC595_display_on(num[0]*10 + num[1],num[2]*10 + num[3],0,0,0,3);
					break;
			}
			key=KEY_Scan(0);	//得到键值
			if(key)
			{						   
				switch(key)
				{				 
					case KEY0_PRES:	//进入调时模式
						if(set_no < 3)
						{
							set_no++;
						}
						else
						{
							status = 2;
						}
						break;
					case KEY1_PRES:	//控制LED1翻转	 
						switch(set_no)
						{				 
							case 0:	
								if(num[0] < 2)
								{
									num[0]++;
								}
								else
								{
									num[0]=0;
								}
								break;
							case 1:	
								if(num[0] == 2)
								{
									if(num[1] < 3)
									{
										num[1]++;
									}
									else{
										num[1] = 0;
									}
								}
								else{
									if(num[1] < 9)
									{
										num[1]++;
									}
									else{
										num[1] = 0;
									}
								}
								break;
							case 2:	
									if(num[2] < 5)
									{
										num[2]++;
									}
									else{
										num[2] = 0;
									}
								break;
							case 3:	
									if(num[3] < 9)
									{
										num[3]++;
									}
									else{
										num[3] = 0;
									}
								break;
						}
						break;
					case KEY2_PRES:
					if(num[0]*10 + num[1] < 24 && num[2] * 10 + num[3] < 60)
						RTC_Set(2016,7,8,num[0]*10 + num[1],num[2] * 10 + num[3],0);
					status = 2;
				}
			}else delay_ms(10); 
		}
		 
	}
 }


