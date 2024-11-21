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

void TIM1_PWM_Init(void) {
		GPIO_InitTypeDef GPIO_InitStruct;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
		TIM_OCInitTypeDef TIM_OCInitStruct;
		
    // 1. 启用GPIOA和TIM1的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // 2. 配置GPIO引脚
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;        // 复用功能
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;      // 推挽输出
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;    // 无上下拉
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   // 速度50MHz

    // 配置PA8为TIM1_CH1
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);

    // 配置PA9为TIM1_CH2
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);

    // 配置PA10为TIM1_CH3
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);

    // 3. 配置TIM1的PWM输出
    // 定时器基础配置
    TIM_TimeBaseStruct.TIM_Period = 999;                      // 自动重装载值（周期）
    TIM_TimeBaseStruct.TIM_Prescaler = 83;                    // 预分频器
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;      // 时钟分频
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct);

    // 通道1（TIM1_CH1）配置
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;            // PWM模式1
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable; // 禁用互补输出
    TIM_OCInitStruct.TIM_Pulse = 0;                         // 占空比50%
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;    // 高电平有效
    TIM_OC1Init(TIM1, &TIM_OCInitStruct);

    // 通道2（TIM1_CH2）配置
    TIM_OCInitStruct.TIM_Pulse = 0;                         // 占空比25%
    TIM_OC2Init(TIM1, &TIM_OCInitStruct);

    // 通道3（TIM1_CH3）配置
    TIM_OCInitStruct.TIM_Pulse = 0;                         // 占空比75%
    TIM_OC3Init(TIM1, &TIM_OCInitStruct);

    // 4. 启动TIM1
    TIM_Cmd(TIM1, ENABLE);

    // 使能TIM1的主输出（MOE位），高级定时器需要
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void Set_TIM1_PWM_DutyCycle(uint16_t channel1_duty, uint16_t channel2_duty, uint16_t channel3_duty) {
    // 设置 TIM1 通道1 的占空比
    TIM_SetCompare1(TIM1, channel1_duty);  // 设置通道1（PA8）的CCR1值
    // 设置 TIM1 通道2 的占空比
    TIM_SetCompare2(TIM1, channel2_duty);  // 设置通道2（PA9）的CCR2值
    // 设置 TIM1 通道3 的占空比
    TIM_SetCompare3(TIM1, channel3_duty);  // 设置通道3（PA10）的CCR3值
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
	
		
    
		//pwm正反转控制初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // 推挽输出  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100 MHz  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 无上下拉  
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
