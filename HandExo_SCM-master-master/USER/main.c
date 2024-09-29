#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lwip_comm.h"
#include "LAN8720.h"
#include "usmart.h"
#include "timer.h"
#include "lcd.h"
#include "sram.h"
#include "malloc.h"
#include "lwip_comm.h"
#include "includes.h"
#include "lwipopts.h"
#include "udp_demo.h"
#include "sensor.h"

#include "pwm.h"
#include "pid.h"
#include "ODcalc.h"
#include "adc.h"


//KEY任务
#define KEY_TASK_PRIO 		7
//任务堆栈大小
#define KEY_STK_SIZE		128	
//任务堆栈
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//任务函数
void key_task(void *pdata);   

//LED任务
//任务优先级
#define LED_TASK_PRIO		9
//任务堆栈大小
#define LED_STK_SIZE		64
//任务堆栈
OS_STK	LED_TASK_STK[LED_STK_SIZE];
//任务函数
void led_task(void *pdata);  

//在LCD上显示地址信息任务
//任务优先级
#define DISPLAY_TASK_PRIO	10
//任务堆栈大小
#define DISPLAY_STK_SIZE	128
//任务堆栈
OS_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//任务函数
void display_task(void *pdata);

//START任务
//任务优先级
#define START_TASK_PRIO		11
//任务堆栈大小
#define START_STK_SIZE		128
//任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata); 

#define MOTOR_TASK_PRIO		8
//任务堆栈大小
#define MOTOR_STK_SIZE		128
//任务堆栈
OS_STK MOTOR_TASK_STK[MOTOR_STK_SIZE];

//任务函数
void motor_task(void *pdata); 

//在LCD上显示地址信息
//mode:1 显示DHCP获取到的地址
//	  其他 显示静态地址
float R_R2force(float R_R)
{
	float f = 0;
	if (R_R < 0.003) {
		f = (R_R - 0) / 0.003 * 2;
	} else if (R_R < 0.010) {
		f = (R_R - 0.003) / 0.007 * 3 + 2;
	} else if (R_R < 0.021) {
		f = (R_R - 0.010) / (0.011) * 5 + 5;
	} else if (R_R < 0.030) {
		f = (R_R - 0.021) / (9) * 10000 + 10;
	} else if (R_R < 0.040) {
		f = (R_R - 0.030) / (1e-3) + 20;
	} else if (R_R < 0.055) {
		f = (R_R - 0.040) / (3) * 4000 + 30;
	} else if (R_R < 0.081) {
		f = (R_R - 0.055) / (13) * 25000 + 50;
	} else if (R_R < 0.131) {
		f = (R_R - 0.081) * 2000 + 100;
	} else if (R_R < 0.166) {
		f = (R_R - 0.131) * 20000 / 7 + 200;
	} else if (R_R < 0.199) {
		f = (R_R - 0.166) * 100000 / 33 + 300;
	} else if (R_R < 0.229){
		f = (R_R - 0.199) * 10000 / 3 + 400;
	} else if (R_R < 0.260){
		f = (R_R - 0.229) * 100000 / 31 + 500;
	} else if (R_R < 0.293){
		f = (R_R - 0.26) * 100000 / 33 + 600;
	} else if (R_R < 0.320){
		f = (R_R - 0.293) * 100000 / 27 + 700;
	} else if (R_R < 0.351){
		f = (R_R - 0.320) * 100000 / 31 + 800;
	} else if (R_R < 0.380){
		f = (R_R - 0.351) * 100000 / 29 + 900;
	} else if (R_R < 0.428){
		f = (R_R - 0.380) * 12500 / 3 + 1000; 
	} else if (R_R < 0.448){
		f = (R_R - 0.428) * 5000 + 1200;
	} else if (R_R < 0.484){
		f = (R_R - 0.448) * 50000 / 9 + 1300;
	} else if (R_R < 0.567){
		f = (R_R - 0.484) * 100000 / 83 + 1400;
	} else {
		f = 2000;
	}
	return f;
}
float gravity_torch(void) 
{
	extern float angle[6];
	float m1 = 12.6,                // g
		  m2 = 34.0,                // g
		  m3 = 8.6;                  // g
	float g_torch;
		
	float l1 = 74.33,   // mm
		  l2 = 90,   // mm
		  l3 = 14.16;   // mm
	float alpha[3];
	float d2r = pi/180;
	float t1, t2, t3;
	t1 = angle[0]+79; t2 = -angle[1]+126; t3 = -angle[5]+20;
	alpha[0] = t1;alpha[1] = t1 - t2;alpha[2] = t1 - t2 - t3;
	
	g_torch = l1 * m3 * 9.8 * cosf(alpha[0]) + l2 * m3 * 9.8 * cosf(alpha[1]) + 1/2 * l3 * m3 * 9.8 * cosf(alpha[2]) + 1/2 * l2 * m2 * 9.8 * cosf(alpha[1]) + l1 * m2 * 9.8 * cosf(alpha[0]) + 1/2 * l1 * m1 * 9.8 * cosf(alpha[0]) ;

	return g_torch*1e-3;
}
void show_address(u8 mode)
{
	u8 buf[30];
	if(mode==2)
	{
		sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		LCD_ShowString(30,170,210,16,16,buf); 
		sprintf((char*)buf,"DHCP GW :%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
		LCD_ShowString(30,190,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
		LCD_ShowString(30,210,210,16,16,buf); 
		LCD_ShowString(30,230,210,16,16,"Port:8089!"); 
	}
	else 
	{
		sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		LCD_ShowString(30,170,210,16,16,buf); 
		sprintf((char*)buf,"Static GW:%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
		LCD_ShowString(30,190,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
		LCD_ShowString(30,210,210,16,16,buf); 
		LCD_ShowString(30,230,210,16,16,"Port:8089!"); 
	}	
}

int main(void)
{	
	delay_init(168);       																		//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//中断分组配置
	uart_init(115200);   																			//串口波特率设置
	usmart_dev.init(84);																			//初始化USMART
	LED_Init(); 																							//LED初始化
//	KEY_Init();  																						
	LCD_Init();  																							//LCD初始化
	
	Sensor_GPIO();																		  			//传感器GPIO_INTT

	TIM14_PWM_Init(499,83);
	PWM_Init(499,83);
	TIM_SetCompare1(TIM3, 100);
	
	
	Adc_Init();
	
	FSMC_SRAM_Init(); 																				//SRAM初始化
	
	mymem_init(SRAMIN);  																			//初始化内部内存池
	mymem_init(SRAMEX);  																			//初始化外部内存池
	mymem_init(SRAMCCM); 																			//初始化CCM内存池
	
	POINT_COLOR = RED; 																				
	LCD_ShowString(30,30,200,20,16, "Explorer STM32F407");
	LCD_ShowString(30,50,200,20,16, "Opening HandExo...");
	LCD_ShowString(30,70,200,20,16, "ATOM@ALIENTEK");
	LCD_ShowString(30,90,200,20,16, "SuYuan 2022/10/18");
	LCD_ShowString(30,110,200,20,16,"Address/Port Information:");
	
	POINT_COLOR = BLUE; 			//蓝色字体

	OSInit(); 																								//UCOS init
	
	while(lwip_comm_init()) 																	//lwip init 
	{
		 LCD_ShowString(30,130,200,20,16,"Lwip Init failed!"); 	
		 delay_ms(500);
		 LCD_Fill(30,130,230,150,WHITE);
		 delay_ms(500);
	}
	LCD_ShowString(30,130,200,20,16,"Lwip Init Success!"); 		

	
	while(udp_demo_init()) 																		//create UDP thread
	{
		LCD_ShowString(30,150,200,20,16,"Created UDP thread fail !"); 				
		delay_ms(500);
		LCD_Fill(30,150,230,170,WHITE);
		delay_ms(500);
	}	
	LCD_ShowString(30,150,200,20,16,"Created UDP thread success!"); 

	POINT_COLOR = BLACK;
	LCD_ShowString(30,250,300,40,16,"Author: YuanSu(yuan_sue@yeah.net)"); 
	LCD_ShowString(30,270,300,40,16,"Date: 2022-10-01");
	LCD_ShowString(30,290,300,20,16,"Project: HandExo for telerobotics"); 
	LCD_ShowString(30,310,400,20,16,"Thanks to Zuqiang Yuan and Gaofeng Li");
	LCD_ShowString(30,330,400,20,16,"for their contributions to this project!");

	
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
//			while(1){}
	
	OSStart(); //start UCOS
	
}


//start task
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr;
	pdata = pdata ;
	
	OSStatInit();  							//初始化统计任务
	OS_ENTER_CRITICAL();  			//关中断
#if	LWIP_DHCP
	lwip_comm_dhcp_creat();			//创建DHCP任务  //这里我取消创建DHCP任务 2023-04-28 YSu
#endif
	OSTaskCreate(led_task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO); 	//创建LED任务
	OSTaskCreate(key_task,(void*)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO); 	//创建KEY任务
	OSTaskCreate(display_task,(void*)0,(OS_STK*)&DISPLAY_TASK_STK[DISPLAY_STK_SIZE-1],DISPLAY_TASK_PRIO); //显示任务
	
	OSTaskCreate(motor_task,(void*)0,(OS_STK*)&MOTOR_TASK_STK[MOTOR_STK_SIZE-1],MOTOR_TASK_PRIO);
	OSTaskSuspend(OS_PRIO_SELF); //挂起start_task任务
	OS_EXIT_CRITICAL();  //开中断
}


//显示地址等信息的任务函数
void display_task(void *pdata)
{
	while(1)
	{ 
#if LWIP_DHCP															//当开启DHCP的时候
		if(lwipdev.dhcpstatus != 0) 					//开启DHCP
		{
			show_address(lwipdev.dhcpstatus );	//显示地址信息
			OSTaskSuspend(OS_PRIO_SELF); 				//显示完地址信息后挂起自身任务
		}
#else
		show_address(0); 											//显示静态地址
		OSTaskSuspend(OS_PRIO_SELF); 					//显示完地址信息后挂起自身任务
#endif //LWIP_DHCP
		OSTimeDlyHMSM(0,0,0,500);
	}
}

//key任务
void key_task(void *pdata)
{
	u8 key; 
	while(1)
	{
		key = KEY_Scan(0);
		if(key==KEY0_PRES) //发送数据
		{
			udp_flag |= LWIP_SEND_DATA; //标记LWIP有数据要发送
		}
		OSTimeDlyHMSM(0,0,0,10);  //延时10ms
	}
}

//led任务
void led_task(void *pdata)
{
	u16 adcx;
	float temp;
	POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(30,350,200,16,16,"ADC1_CH5_VALh:");	      
	LCD_ShowString(30,370,200,16,16,"ADC1_CH5_VOL:0.000V");	//先在固定位置显示小数点  
	LCD_ShowString(30,390,200,16,16,"force_sen:");
	while(1)
	{ 
		adcx=Get_Adc_Average(ADC_Channel_5,20);//获取通道5的转换值，20次取平均
		LCD_ShowxNum(134,350,adcx,4,16,0);    //显示ADCC采样后的原始值
		temp=(float)adcx*(3.3/4096);          //获取计算后的带小数的实际电压值，比如3.1111
		adcx=temp;                            //赋值整数部分给adcx变量，因为adcx为u16整形
		LCD_ShowxNum(134,370,adcx,1,16,0);    //显示电压值的整数部分，3.1111的话，这里就是显示3
		temp-=adcx;                           //把已经显示的整数部分去掉，留下小数部分，比如3.1111-3=0.1111
		temp*=1000;                           //小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留三位小数。
		LCD_ShowxNum(150,370,temp,3,16,0X80); //显示小数部分（前面转换为了整形显示），这里显示的就是111.
		
		LED0=!LED0;
		OSTimeDlyHMSM(0,0,0,250);
	}
}

void motor_task(void *pdata)
{
	extern float angle[6];
	extern float anglef[6];
	u16 adcx;
	TransMatrix TM;
	
	PID pid;
	
	float torch_des, OutPut;
	float allegro_torch = 8;
	
	float R0 = 10,R_R;
	float force_sen; 
	float maxOutput = 10000;
	Jacobe J1;
	FT oriFt, TransFt;
	
	float 	kp = 290,
			kd = 8,
			ki = 22;
	pid_InitWithMax(&pid, kp, kd, ki, 9999, maxOutput-1);         // set the kp as 1, set the kd as 1, set the ki as 0, set the max Interval and the max Output

	oriFt.fx = 0;
	oriFt.fy = 0;
	oriFt.fz = 0;
	oriFt.tx = 0;
	oriFt.ty = 0;
	oriFt.tz = 0;
	
	while(1)
	{	
		TM_calc(&TM, angle);
		
		///////////////////////////////////
		adcx=Get_Adc_Average(ADC_Channel_5,20);
		R_R = 1/R0 * (4096-adcx) / adcx;
		force_sen = R_R2force(R_R) * 1e-2;
		if (force_sen < 0) {force_sen=0;}
		///////////////////////////////////

		printf(" a1=%.4f;a2=%.4f;a3=%.4f;\n a4=%.4f;a5=%.4f;a6 = %.4f;\n",angle[0],angle[1],angle[2],angle[3],angle[4],angle[5]);
		printf(" cali:angle[0]:%.4f, angle[1]:%.4f, angle[2]:%.4f\n angle[3]:%.4f, angle[4]:%.4f, angle[5]:%.4f\n\n",anglef[0],anglef[1],anglef[2],anglef[3],anglef[4],anglef[5]);
		LCD_ShowxNum(110,390,R_R,3,16,0);
		printf("position: %.4f, %.4f, %.4f\n",TM.P1_4,TM.P2_4,TM.P3_4);
		printf("%.4f,%.4f,%.4f\n%.4f,%.4f,%.4f\n%.4f,%.4f,%.4f\n",
		TM.R1_1,TM.R1_2,TM.R1_3,
		TM.R2_1,TM.R2_2,TM.R2_3,
		TM.R3_1,TM.R3_2,TM.R3_3);
		printf("R_R: %.4f\n\n", R_R);
		printf("*************division line *************\n\n");
		printf("force_sen: %.4f @N\n", force_sen);
		printf("****************************************\n\n");

		/////////////////////////////////
		oriFt.fz = -force_sen;                        
		TransFt = TransFT(TM, oriFt);                 // get the transfered force
		J1 = GetJacobe(&TM);
		torch_des = GetTorch(J1, TransFt);
		torch_des = fabs(torch_des);
		/////////////////////////////////

		printf("Generallized force: %.4f N,%.4f N, %.4f N \n", TransFt.fx, TransFt.fy, TransFt.fz);
		printf("Jacobe matrix: %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n", J1.d1, J1.d2, J1.d3, J1.r1, J1.r2, J1.r3);
		printf("torch_des: %.4f @Nmm\n", torch_des);
		printf("torch_ref: %.4f @Nmm\n", allegro_torch);
		printf("Error: %.4f @Nmm\n", allegro_torch-torch_des);
		printf("****************************************\n\n");
		
		pid_calc(&pid, allegro_torch, torch_des);
		OutPut = pid_getPIDOutput(&pid);
		//OutPut = OutPut + gravity_torch();
	  OutPut = OutPut>0?OutPut:200;
		OutPut = OutPut<350?350:OutPut;
//		printf("gravity_torch: %.4f\n", gravity_torch());
		printf("output: %.4f\n\n", OutPut);
		

		TIM_SetCompare1(TIM14, (int)OutPut);
		OSTimeDlyHMSM(0,0,0,5);
	}
}








