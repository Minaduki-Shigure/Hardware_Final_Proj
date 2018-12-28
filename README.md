# Hardware_Final_Proj
本代码使用MDK5开发，需配合STM32F103开发板使用。  
## 针脚定义
ADC的输入为P1针脚，DAC的输出为P4针脚。  
## 宏定义
各个宏定义如下： 
```
#define SLOPE 25		//k
#define V_ZERO 840		//Vzero
#define CONS 4
#define VOUT_FORMULA (float)1000*pow((float)10,(((float)adc_raw*3300)/(81900*(float)SLOPE)-(float)V_ZERO/(20*(float)SLOPE)-(float)CONS/20))
```
这是通过公式由ADC输入值计算实际波形峰峰值的部分，`VOUT_FORMULA`定义的是计算公式，而`SLOPE`、`V_ZERO`和`CONS`分别对应公式里的k、Vzero和CONS，
这一部分可以直接修改。  
```
#define STATE0 1
#define STATE1 10
#define STATE2 20
#define STATE3 100
```
这一部分定义的是DAC输出各个状态下对应的增益倍数，用来从Vout计算Vin的值，可以直接修改。  
```
#define STATE0_VAL 0
#define STATE1_VAL 1023
#define STATE2_VAL 2482
#define STATE3_VAL 4095
```
这一部分定义的是DAC各个输出状态的寄存器值，DAC的实际输出Vdac=(寄存器值*3.3V)/4096。可以直接修改。
