#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "sys.h"
#include <stdio.h>
#include "math.h"


#define CSn1_H   GPIO_SetBits(GPIOB, GPIO_Pin_3 );		//PB3:CSN1						//SetBits设置IO高电平
#define CSn1_L   GPIO_ResetBits(GPIOB, GPIO_Pin_3 );												//ResetBits设置IO低电平
#define DO1      GPIO_ReadInputData(GPIOB)&0x0010			//PB4:DIO1
#define CLK1_H   GPIO_SetBits(GPIOB, GPIO_Pin_5 );	
#define CLK1_L   GPIO_ResetBits(GPIOB, GPIO_Pin_5 );	//PB5:clk1	
#define PROG1_H  GPIO_SetBits(GPIOB, GPIO_Pin_6 );	
#define PROG1_L  GPIO_ResetBits(GPIOB, GPIO_Pin_6 );	//PB6:ldc1
#define PROG1    GPIO_ReadInputData(GPIOB)&0x2000			//PB6:DIO1


/// ---------------------------------------------------------
/// Created by YuanSu(yuan_sue@yeah.net) on 2022/10/10
///	Add another two fingers
/// ---------------------------------------------------------

#define CSn2_H		GPIO_SetBits(GPIOC, GPIO_Pin_2 );			//PC2	
#define CSn2_L		GPIO_ResetBits(GPIOC, GPIO_Pin_2 );				
#define DO2				GPIO_ReadInputData(GPIOC)&0x0001			//PC0					
#define CLK2_H    GPIO_SetBits(GPIOC, GPIO_Pin_3 );
#define CLK2_L		GPIO_ResetBits(GPIOC, GPIO_Pin_3 );		//PC3

#define CSn3_H		GPIO_SetBits(GPIOB, GPIO_Pin_12 );		//PB12	
#define CSn3_L		GPIO_ResetBits(GPIOB, GPIO_Pin_12 );				
#define DO3				GPIO_ReadInputData(GPIOC)&0x0040			//PC6				
#define CLK3_H    GPIO_SetBits(GPIOB, GPIO_Pin_13 );
#define CLK3_L		GPIO_ResetBits(GPIOB, GPIO_Pin_13 );	//PB13



void send_bytes(void); 															//发送单传感器数据字节串
void GPIO_Configuration(void);
void NVIC_Configuration(void);
//void zero_otp(void);
unsigned int read_otp(void);
void Sensor_GPIO(void);															//初始化IO口
void GetDegree(void);																//获得传感器实时角度
void GetDegreeo(void);															//获得传感器初始角度
void LED_Ctl(char LEDdata);
void GetDegree_(void);

#endif

