#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "adc.h"
#include "key.h"
#include "dac.h"

#include <math.h>

//State of the Output(x10,x100,etc)
#define STATE0 1
#define STATE1 10
#define STATE2 20
#define STATE3 100

//Output of DAC raw value
#define STATE0_VAL 0
#define STATE1_VAL 1023
#define STATE2_VAL 2482
#define STATE3_VAL 4095

#define SLOPE 25		//k
#define V_ZERO 840		//Vzero
#define CONS 4
#define VOUT_FORMULA (float)1000*pow((float)10,(((float)adc_raw*3300)/(81900*(float)SLOPE)-(float)V_ZERO/(20*(float)SLOPE)-(float)CONS/20))

//ALIENTEK Mini STM32开发板范例代码15
//ADC实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
   	
 int main(void)
 { 
	u16 stateval[4]={STATE0_VAL,STATE1_VAL,STATE2_VAL,STATE3_VAL};
	u16 statetrans[4]={STATE0,STATE1,STATE2,STATE3};
	u16 state = 0;
	u16 adcx;
	u16 adc_raw;
	float temp;
	double vout;
	double vin;
	int temp1;
 	u8 t=0;	 
	u16 dacval=0;
	u8 key;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	    	 //延时函数初始化	  
	uart_init(9600);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
 	LCD_Init();			   	//初始化LCD 		
	KEY_Init();				//按键初始化		 	
 	Adc_Init();		  		//ADC初始化
	Dac1_Init();		 	//DAC通道1初始化	
	
	POINT_COLOR=DARKBLUE;
	LCD_ShowString(52,50,200,16,16,"Nanjing University");	
	LCD_ShowString(60,70,200,16,16,"  Group 4 & 14");	
	LCD_ShowString(60,90,200,16,16,"Minaduki Shigure");
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,110,200,16,16," P1:IN  P4:OUT");	
	LCD_ShowString(60,130,200,16,16,"WK_UP:+  KEY0:-");	
	//显示提示信息											      
	POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(60,150,200,16,16,"DAC VAL:");	      
	LCD_ShowString(60,170,200,16,16,"DAC VOL:0.000V");	      
	LCD_ShowString(60,190,200,16,16,"ADC RAW:0.000V");
	LCD_ShowString(60,210,200,16,16,"OUT VOL:0.000V");
  LCD_ShowString(60,230,200,16,16,"IN  VOL:0000.00mV");
	
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);//????0	   	      
	while(1)
	{
		t++;
		key=KEY_Scan(0);			  
		if(key==WKUP_PRES)
		{		 
			if(state<3)state+=1;
			dacval = stateval[state];
			DAC_SetChannel1Data(DAC_Align_12b_R, dacval);//??DAC?
		}else if(key==KEY0_PRES)	
		{
			if(state>0)state-=1;
			else state=0;
			dacval = stateval[state];
			DAC_SetChannel1Data(DAC_Align_12b_R, dacval);//??DAC?
		}	 
		if(t==10||key==KEY0_PRES||key==WKUP_PRES) 	//WKUP/KEY1按下了,或者定时时间到了
		{	  
 			adcx=DAC_GetDataOutputValue(DAC_Channel_1);//??????DAC??
			LCD_ShowxNum(124,150,adcx,4,16,0);     	//显示DAC寄存器值
			temp=(float)adcx*(3.3/4096);			//得到DAC电压值
			adcx=temp;
 			LCD_ShowxNum(124,170,temp,1,16,0);     	//显示电压值整数部分
 			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(140,170,temp,3,16,0X80); 	//显示电压值的小数部分
 			adcx=Get_Adc_Average(ADC_Channel_1,10);		//得到ADC转换值	  
			adc_raw=adcx;
			temp=(float)adcx*(3.3/4096);			//得到ADC电压值
			adcx=temp;
 			LCD_ShowxNum(124,190,temp,1,16,0);     	//显示电压值整数部分
 			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(140,190,temp,3,16,0X80); 	//显示电压值的小数部分
			
			vout = VOUT_FORMULA;
			vin = vout;
			//vout = (float)1000*pow((float)10,(((float)adc_raw*33)/(819000*(float)SLOPE)-(float)V_ZERO/(20*(float)SLOPE)-(float)CONS/20));
			temp1 = vout/1000;
			printf("%d\t%lf\t%d\t",adc_raw,vout,temp1);
 			LCD_ShowxNum(124,210,temp1,1,16,0);     	//显示电压值整数部分
 			vout = vout - temp1 * 1000;
			printf("%lf\t",vout);
			LCD_ShowxNum(140,210,vout,3,16,0X80); 	//显示电压值的小数部分
			
			vin = vin / statetrans[state];
			//vin = vin/1000;
			printf("%d\t%lf\t%d\t",adc_raw,vin,(int)vin);
 			LCD_ShowxNum(124,230,(int)vin,4,16,0);     	//显示电压值整数部分
 			vin = (vin - (int)vin) * 100;
			printf("%lf\n",vin);
			LCD_ShowxNum(164,230,(int)vin,2,16,0X80); 	//显示电压值的小数部分
			//printf("\n");
			
			LED0=!LED0;	   
			t=0;
		}	    
		delay_ms(10);	
	}										    
}	

