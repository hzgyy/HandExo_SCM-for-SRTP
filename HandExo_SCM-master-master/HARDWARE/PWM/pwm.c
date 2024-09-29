#include "pwm.h"
 
void PWM_Init(u32 arr,u32 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;              						//定义GPIO结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;    						//定义TIMx定时器结构体
    TIM_OCInitTypeDef TIM_OCInitStructure;            						//定义定时器脉宽调制结构体
     
	TIM_OCStructInit(&TIM_OCInitStructure);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);                     //使能TIM3时钟  
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);					//使能GPIOB时钟和AFIO复用时钟
    
//    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);                     //TIM3部分重映射 TIM3_CH2->PB5
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                               //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                         //复用推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;                       //配置输出速率
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
    GPIO_Init(GPIOA,&GPIO_InitStructure);                                   //初始化GPIOB
	
	TIM_TimeBaseStructure.TIM_Period = arr;                                 //设置自动重装载寄存器周期的值 arr=value-1
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                              //设置预分频值 psc=value-1
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                            //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);                          //初始化TIMx时间基数
	
																			//初始化TIM3 Channel2 PWM模式     
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                       //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //使能比较输出
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;               //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	
    TIM_OC1Init(TIM3,&TIM_OCInitStructure);                                 //根据T指定的参数初始化外设TIM3 OC2
	
	TIM_Cmd(TIM3, ENABLE);                                                  //使能TIM3
		
}


void motor_v(int v) 
{
	TIM_SetCompare1(TIM3, v);
}


void TIM14_PWM_Init(uint32_t arr, uint32_t psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    //使能时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
    
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14); //复用
    
    //GPIOF初始化
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    //TIM_TimeBaseInitStructure.TIM_RepetitionCounter = ; only TIM1 TIM8
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStructure);
    
    //only for TIM1 and TIM8 仅高级定时器用
    //TIM_OCInitStructure.TIM_OCIdleState = ; //TIM互补输出比较状态
    //TIM_OCInitStructure.TIM_OCNIdleState = ; //Idle状态下TIM输出比较引脚状态
    //TIM_OCInitStructure.TIM_OCNPolarity = ; //指定互补输出极性
    //TIM_OCInitStructure.TIM_OutputNState = ; //Idle状态下TIM输出比较引脚状态
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //TIM输出比较模式
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性低
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    //TIM_OCInitStructure.TIM_Pulse = ; //捕获/比较寄存器CCR的值，此程序在后面设置以控制LED的亮度
    TIM_OC1Init(TIM14, &TIM_OCInitStructure);
    
    TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable); //使能预装载寄存器
    TIM_ARRPreloadConfig(TIM14,ENABLE); //ARPE使能
    
    TIM_Cmd(TIM14, ENABLE); //使能TIM14
}


void PWM_SetCompareLow(void) 
{
	TIM_SetCompare1(TIM14, 0);
}

void PWM_SetCompareMid(void) 
{
	TIM_SetCompare1(TIM14, 250);
}

void PWM_SetCompareHigh(void) 
{
	TIM_SetCompare1(TIM14, 500);
}

void PWM_SetCompare( u8 ch, u16 count)
{
	if (ch == 1)TIM_SetCompare1(TIM14, count);
	else TIM_SetCompare1(TIM3, count);
}
