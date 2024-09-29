#include "udp_demo.h"
#include "lwip_comm.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
#include "lwip/api.h"
#include "lwip/lwip_sys.h"
#include "string.h"
#include "sensor.h"
#include "lcd.h"
//#include "get_pose.h"


//TCP客户端任务
#define UDP_PRIO		6
//任务堆栈大小
#define UDP_STK_SIZE	300
//任务堆栈
OS_STK UDP_TASK_STK[UDP_STK_SIZE];


u8 udp_demo_recvbuf[UDP_DEMO_RX_BUFSIZE];	//UDP接收数据缓冲区

//UDP发送数据内容
const u8 *udp_demo_sendbuf="Explorer STM32F407 NETCONN UDP demo send data\r\n";
u8 udp_flag;							//UDP数据发送标志位
//如果我发送的内容是一个数组
float test[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.1, 11.1, 12.2, 13.3, 14.4, 15.5, 16.6, 17.7, 18.8};

	
	

//udp任务函数
static void udp_thread(void *arg)
{
	
	extern float angle[6];
	extern float angle2[6];
	extern float angle3[6];	
	extern float angle_udp[18];
	
	OS_CPU_SR cpu_sr;
	err_t err;
	static struct netconn *udpconn;
	static struct netbuf  *recvbuf;
	static struct netbuf  *sentbuf;
	struct ip_addr destipaddr;
	u32 data_len = 0;
	struct pbuf *q;
	
	LWIP_UNUSED_ARG(arg);
	udpconn = netconn_new(NETCONN_UDP);  																																			//创建一个UDP链接
	udpconn->recv_timeout = 1;  
	
	if(udpconn != NULL)  																																											//创建UDP连接成功
	{
		err = netconn_bind(udpconn,IP_ADDR_ANY,UDP_DEMO_PORT); 
		IP4_ADDR(&destipaddr,lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3]); //构造目的IP地址
		netconn_connect(udpconn,&destipaddr,UDP_DEMO_PORT); 																										//连接到远端主机
		
		//获取传感器初始角度
		GetDegreeo();
		
		if(err == ERR_OK)//绑定完成
		{
			
			while(1)
			{
				
				GetDegree();
							
				// if((udp_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //有数据要发送
				if( 1 ) 
				{
					sentbuf = netbuf_new();						//创建一个netbuf的结构体
						
					netbuf_alloc(sentbuf,sizeof(angle_udp));
									
					memcpy(sentbuf->p->payload,(void*)angle_udp,sizeof(angle_udp));
										
					err = netconn_send(udpconn,sentbuf);  	//将netbuf中的数据发送出去
					
					if(err != ERR_OK)
					{
						printf("发送失败\r\n");
						LCD_ShowString(30,510,210,16,16,"123");
						netbuf_delete(sentbuf);      //删除buf
					}
					udp_flag &= ~LWIP_SEND_DATA;	//清除数据发送标志
					netbuf_delete(sentbuf);      	//删除buf
				}	
								
				netconn_recv(udpconn,&recvbuf); //接收数据 ，屏蔽掉这行代码不会进入lwip_dhcp_task(); 这里还要研究一下为什么？？？
				
				
			}
		}else printf("UDP绑定失败\r\n");
	}else printf("UDP连接创建失败\r\n");
}


//创建UDP线程
//返回值:0 UDP创建成功
//		其他 UDP创建失败
INT8U udp_demo_init(void)
{
	
	INT8U res;
	OS_CPU_SR cpu_sr;
	
	OS_ENTER_CRITICAL();	//关中断
	res = OSTaskCreate(udp_thread,(void*)0,(OS_STK*)&UDP_TASK_STK[UDP_STK_SIZE-1],UDP_PRIO); 		// Created UDP thread
	OS_EXIT_CRITICAL();		//开中断
	
	return res;
}

