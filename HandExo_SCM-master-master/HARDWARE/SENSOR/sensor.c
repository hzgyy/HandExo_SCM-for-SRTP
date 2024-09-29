#include "sensor.h" 
#include "delay.h"
#include "usart.h"
uint8_t ADDR=1;//器件地址 



//初始角度
unsigned int out[6];
unsigned int out2[6];
unsigned int out3[6];

float anglef[6];//初始角度
float anglef2[6];
float anglef3[6];


float anglem[6];//中间角度

float angle[6];
float angle2[6];
float angle3[6];

float angle_udp[18];

u8  ZF_flag[6];	//当前角度及符号


unsigned char xun;//for循环变量


//端口接收有关变量
uint8_t Rx_Buffer[6];//上位机命令6字节，帧头+帧长+传感器号+命令+校验+帧尾
uint8_t Rx_Cnt=0;
uint8_t Rx_Cmd=0;
uint8_t Rx_Num=0;
uint8_t Rx_Flag=0;

//uint8_t  err1_flag=0;
//uint8_t  err2_flag=0;


/* Private variables ---------------------------------------------------------*/
/*******************************************************************/	
/*函数名称：Sensor_GPIO                                            */
/*输    入：无   												   */
/*输    出：无                                                     */
/*功能说明：配置I/O口。                                            */
/*                                                                 */
/*******************************************************************/
void Sensor_GPIO(void)
{
	
    GPIO_InitTypeDef GPIO_InitStructure;
		//时钟初始化
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);
	
		//磁编码器传感器1I/O配置
		//PB10:CSN,PB11:DIO,PB12:clk,PB13:PROG,PB14:OUT1,PB15:OUT2,
	
  	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3|GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//推挽输出	
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;									//PB11:DIO1
  	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;       
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		//初始化，prog不用为低
		PROG1_L;
	
	
/// ---------------------------------------------------------
/// Created by YuanSu(yuan_sue@yeah.net) on 2022/10/10
///	Add another two fingers
/// ---------------------------------------------------------

		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;							//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_12|GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;							//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
				
}


/*******************************************************************/	
/*函数名称：read_change                                            */
/*输    入：无   												   */
/*输    出：无                                                     */
/*功能说明：读磁编码器数据                                            */
/*                                                                 */
/*******************************************************************/

void read_change(void)
{
	
  unsigned int tempdat[6];
	unsigned int tempdat2[6];
	unsigned int tempdat3[6];
		
  u8 Ocf[6],Cof[6],Lin[6];							//OCF为偏差补偿完成标识,COF为CARDIC单元溢出标识，LIN为磁场线性度报警
	u8 Ocf2[6],Cof2[6],Lin2[6];
	u8 Ocf3[6],Cof3[6],Lin3[6];
	
  CSn1_H; CSn2_H;	CSn3_H;								//CSn片选先保持高电平
  CLK1_H; CLK2_H;	CLK3_H;								//时钟保持高电平
  delay_us(1);
	
  CSn1_L;	CSn2_L;	CSn3_L; 							//如果CSn变为低电平，数据输出DO将从高阻状态变为高电平并启动读取操作
  delay_us(1);
  CLK1_L; CLK2_L; CLK3_L;
  delay_us(1);
	
	
  for(xun=0;xun<15;xun++)											//daisy chain 1 18bit
  {
      CLK1_H;	CLK2_H; CLK3_H;									//数据将在CLK的第一个上升沿锁存至输出移位寄存器内
  delay_us(1);
      if(DO1)
      {
        tempdat[0]=tempdat[0]<<1; 						//finger1第一个传感器
        tempdat[0]|=0x0001;
      }
      else 
      {
        tempdat[0]=tempdat[0]<<1;
        tempdat[0]&=0xfffe;
      }
			
/// ---------------------------------------------------------
/// Created by YuanSu(yuan_sue@yeah.net) on 2022/10/10
///	finger2/finger3 first sensor
/// ---------------------------------------------------------	
			
			if(DO2)
			{
				tempdat2[0]=tempdat2[0]<<1;		
				tempdat2[0]|=0x0001;
			}
			else
			{
				tempdat2[0]=tempdat2[0]<<1;
				tempdat2[0]&=0xfffe;
			}
			
			if(DO3)
			{
				tempdat3[0]=tempdat3[0]<<1;
				tempdat3[0]|=0x0001;
			}
			else
			{
				tempdat3[0]=tempdat3[0]<<1;
				tempdat3[0]&=0xfffe;
			}
						
  delay_us(1);
      CLK1_L; CLK2_L; CLK3_L;
  delay_us(1);
  };
	
  for(xun=0;xun<4;xun++)						//这个时钟为什么要反复拉高拉低4次？？？
  {   
			CLK1_H; CLK2_H; CLK3_H;
			delay_us(1);
			CLK1_L; CLK2_L; CLK3_L;
			delay_us(1);
	};
	
	
	/////////////////////////////////////////////////////////////////////////////////////////
  for(xun=0;xun<15;xun++)						//daisy chain 1 18bit
  {
		
      CLK1_H;	CLK2_H; CLK3_H;											//数据将在CLK的第一个上升沿锁存至输出移位寄存器内
			delay_us(1);
      
			if(DO1)
      {
        tempdat[1]=tempdat[1]<<1;		//第三个传感器
        tempdat[1]|=0x0001;
      }
      else 
      {
        tempdat[1]=tempdat[1]<<1;
        tempdat[1]&=0xfffe;
      }
			
			if(DO2)
			{
				tempdat2[1]=tempdat2[1]<<1;
				tempdat2[1]|=0x0001;
			}
			else
			{
				tempdat2[1]=tempdat2[1]<<1;
				tempdat2[1]&=0xfffe;
			}
			
			if(DO3)
			{
				tempdat3[1]=tempdat3[1]<<1;
				tempdat3[1]|=0x0001;
			}
			else
			{
				tempdat3[1]=tempdat3[1]<<1;
				tempdat3[1]&=0xfffe;
			}
					
  delay_us(1);
      CLK1_L; CLK2_L; CLK3_L;
  delay_us(1);
  }
	
  for(xun=0;xun<4;xun++)
  {    
			CLK1_H; CLK2_H; CLK3_H;
			delay_us(1);
      CLK1_L;	CLK2_L;	CLK3_L;
			delay_us(1);
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
  for(xun=0;xun<15;xun++)						//daisy chain 1 18bit
  {
		
      CLK1_H;	CLK2_H; CLK3_H;											//数据将在CLK的第一个上升沿锁存至输出移位寄存器内
			delay_us(1);
      
			if(DO1)
      {
        tempdat[2]=tempdat[2]<<1;		//第三个传感器
        tempdat[2]|=0x0001;
      }
      else 
      {
        tempdat[2]=tempdat[2]<<1;
        tempdat[2]&=0xfffe;
      }
			
			if(DO2)
			{
				tempdat2[2]=tempdat2[2]<<1;
				tempdat2[2]|=0x0001;
			}
			else
			{
				tempdat2[2]=tempdat2[2]<<1;
				tempdat2[2]&=0xfffe;
			}
			
			if(DO3)
			{
				tempdat3[2]=tempdat3[2]<<1;
				tempdat3[2]|=0x0001;
			}
			else
			{
				tempdat3[2]=tempdat3[2]<<1;
				tempdat3[2]&=0xfffe;
			}
					
  delay_us(1);
      CLK1_L; CLK2_L; CLK3_L;
  delay_us(1);
  }
	
  for(xun=0;xun<4;xun++)
  {    
			CLK1_H; CLK2_H; CLK3_H;
			delay_us(1);
      CLK1_L;	CLK2_L;	CLK3_L;
			delay_us(1);
	};
	
	
	////////////////////////////////////////////////////////////////////////////////////////
  for(xun=0;xun<15;xun++)//daisy chain 1 18bit
  {
		
      CLK1_H; CLK2_H; CLK3_H;
			delay_us(1);
      if(DO1)
      {
        tempdat[3]=tempdat[3]<<1;//第四个传感器
        tempdat[3]|=0x0001;
      }
      else 
      {
        tempdat[3]=tempdat[3]<<1;
        tempdat[3]&=0xfffe;
      }
			
			if(DO2)
			{
				tempdat2[3]=tempdat2[3]<<1;
				tempdat2[3]|=0x0001;
			}
			else
			{
				tempdat2[3]=tempdat2[3]<<1;
				tempdat2[3]&=0xfffe;
			}
			
			if(DO3)
			{
				tempdat3[3]=tempdat3[3]<<1;
				tempdat3[3]|=0x0001;
			}
			else
			{
				tempdat3[3]=tempdat3[3]<<1;
				tempdat3[3]&=0xfffe;
			}
		
			delay_us(1);
      CLK1_L; CLK2_L; CLK3_L;
			delay_us(1);
  }
  for(xun=0;xun<4;xun++)
  {    
			CLK1_H; CLK2_H; CLK3_H;
			delay_us(1);
      CLK1_L; CLK2_L; CLK3_L;
			delay_us(1);
	};
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////
  for(xun=0;xun<15;xun++)//daisy chain 1 18bit
  {
		
      CLK1_H; CLK2_H; CLK3_H;
			delay_us(1);
      if(DO1)
      {
        tempdat[4]=tempdat[4]<<1;//第四个传感器
        tempdat[4]|=0x0001;
      }
      else 
      {
        tempdat[4]=tempdat[4]<<1;
        tempdat[4]&=0xfffe;
      }
			
			if(DO2)
			{
				tempdat2[4]=tempdat2[4]<<1;
				tempdat2[4]|=0x0001;
			}
			else
			{
				tempdat2[4]=tempdat2[4]<<1;
				tempdat2[4]&=0xfffe;
			}
			
			if(DO3)
			{
				tempdat3[4]=tempdat3[4]<<1;
				tempdat3[4]|=0x0001;
			}
			else
			{
				tempdat3[4]=tempdat3[4]<<1;
				tempdat3[4]&=0xfffe;
			}
		
			delay_us(1);
      CLK1_L; CLK2_L; CLK3_L;
			delay_us(1);
  }
  for(xun=0;xun<4;xun++)
  {    
			CLK1_H; CLK2_H; CLK3_H;
			delay_us(1);
      CLK1_L; CLK2_L; CLK3_L;
			delay_us(1);
	};
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////
  for(xun=0;xun<15;xun++)//daisy chain 1 18bit
  {
		
      CLK1_H;	CLK2_H; CLK3_H;
			delay_us(1);
      if(DO1)
      {
        tempdat[5]=tempdat[5]<<1;	//第六个传感器
        tempdat[5]|=0x0001;
      }
      else 
      {
        tempdat[5]=tempdat[5]<<1;
        tempdat[5]&=0xfffe;
      }
			
			if(DO2)
			{
        tempdat2[5]=tempdat2[5]<<1;	
        tempdat2[5]|=0x0001;			
			}
			else
			{
        tempdat2[5]=tempdat2[5]<<1;
        tempdat2[5]&=0xfffe;			
			}
			
			if(DO3)
			{
				tempdat3[5]=tempdat3[5]<<1;
				tempdat3[5]|=0x0001;
			}
			else
			{
				tempdat3[5]=tempdat3[5]<<1;
				tempdat3[5]&=0xfffe;
			}
					
			delay_us(1);
      CLK1_L; CLK2_L; CLK3_L;
			delay_us(1);
  }
	
			delay_us(1);
	
			CSn1_H; CSn2_H; CSn3_H; 		//通过一个CSN的高脉冲启动后续测量
			CLK1_H;	CLK2_H; CLK3_H;
			delay_us(1);
	
	
	//degree out
	for(xun=0;xun<6;xun++)
{
	
  Ocf[xun]=(tempdat[xun] & (1<<2))>>2;	Ocf2[xun]=(tempdat2[xun] & (1<<2))>>2;	Ocf3[xun]=(tempdat3[xun] & (1<<2))>>2;
  Cof[xun]=(tempdat[xun] & (1<<1))>>1;	Cof2[xun]=(tempdat2[xun] & (1<<1))>>1;	Cof3[xun]=(tempdat3[xun] & (1<<1))>>1;
  Lin[xun]=(tempdat[xun] & (1<<0))>>0;	Lin2[xun]=(tempdat2[xun] & (1<<0))>>0;	Lin3[xun]=(tempdat3[xun] & (1<<0))>>0;
	
  if(Ocf[xun]==1 && Cof[xun]==0 && Lin[xun]==0)
			{out[xun]=tempdat[xun]>> 3;}									//补偿算法完成OCF=1，没有溢出COF=0和线性度报警LIN=0
  else{out[xun]=4096;}															//AS5045范围0-4095，出现4096则说明磁铁安装不合适，需要调整。
	
	if(Ocf2[xun]==1 && Cof2[xun]==0 && Lin2[xun]==0)
			{out2[xun]=tempdat2[xun]>> 3;}								
  else{out2[xun]=4096;}	

	if(Ocf3[xun]==1 && Cof3[xun]==0 && Lin3[xun]==0)
			{out3[xun]=tempdat3[xun]>> 3;}									
  else{out3[xun]=4096;}													
		
}

}

/*******************************************************************/	
/*函数名称：zero_otp                                            */
/*输    入：无   												   */
/*输    出：无                                                     */
/*功能说明：磁编码器非永久性零位设置                                            */
/*                                                                 */
/*******************************************************************/
//void zero_otp(void)
//{
//  unsigned int tmpdata[5];
//  unsigned int ssivalue=0;
//  unsigned char i;
//  for(i=0;i<5;i++){tmpdata[i]=read_change();}//读取当前角度采集值 
////  tmpdata[4]|=0x0004;//PWM禁止，禁止LIN报警
//  ssivalue=(((tmpdata[4]<<3) | (0<<15))| 0x3);
//  delay_ms(10);//延时10ms
//  CSn1_L;
//  CLK1_L;//时钟先为低
//  PROG1_L;
//  delay_ms(10);//延时10ms
//  PROG1_H;
//  delay_ms(10);//延时10ms
//  CSn1_H; //CSn片选先保持高电平
//  delay_ms(10);//延时10ms
//  for(i=0;i<16;i++)//daisy chain 1 18bit
//  {
//   if(ssivalue&0x8000){PROG1_H;}
//   else{PROG1_L;}
//   delay_ms(1);//延时1ms
//   ssivalue=ssivalue<<1;
//   CLK1_H;//数据将在CLK的第一个上升沿锁存至输出移位寄存器内
//   delay_ms(2);//延时2ms
//   CLK1_L;
//   delay_ms(1);//延时1ms
// }
//   PROG1_L;
//   CLK1_L;
//   delay_ms(10);//延时10ms
//   CSn1_L;//通过一个CSN的低脉冲启动后续测量
//}
/*******************************************************************/	


/*******************************************************************/	
/*函数名称：GetDegreeo                                       */
/*输    入：无   												   */
/*输    出：无                                                     */
/*功能说明：读取6个磁传感器角度初始值                                            */
/*                                                                 */
/*******************************************************************/
void GetDegreeo(void)
{

	read_change();
	//log: replace the auto calibration before each start
	//by a fixed calibration value in the consideration of convenience.
	for(xun=0;xun<6;xun++)
{
  anglef[xun]=out[xun]*45.0/512.0; 				//finger1 初始角度	value*360/4096 =value=45/512
	anglef2[xun]=out2[xun]*45.0/512.0;			//finger2 初始角度
	anglef3[xun]=out3[xun]*45.0/512.0;			//finger3 初始角度
	
};
	/*
	anglef[0] = anglef2[0] = anglef3[0] = 277.9102;
	anglef[1] = anglef2[1] = anglef3[1] = 62.3145;
	anglef[2] = anglef2[2] = anglef3[2] = 399.9902;
	anglef[3] = anglef2[3] = anglef3[3] = 677.4609;
	anglef[4] = anglef2[4] = anglef3[4] = 189.1406;
	anglef[5] = anglef2[5] = anglef3[5] = 1.0547;
	*/
}
/*******************************************************************/	
/*函数名称：Get_LegsDegree                                       */
/*输    入：无   												   */
/*输    出：无                                                     */
/*功能说明：读取6个磁传感器角度值                                            */
/*                                                                 */
/*******************************************************************/

void GetDegree(void)
{
	
  float angletmp[6];
	float angletmp2[6];
	float angletmp3[6];
	
	read_change();
	
	for(xun=0;xun<6;xun++)
{  
	
	angletmp[xun]=out[xun]*45.0/512.0;																//当前读到的6个编码器角度
	angletmp2[xun]=out2[xun]*45.0/512.0;
	angletmp3[xun]=out3[xun]*45.0/512.0;
	
  angletmp[xun]=angletmp[xun]-anglef[xun];													//当前角度-初始角度 = 相对角度
	angletmp2[xun]=angletmp2[xun]-anglef2[xun];
	angletmp3[xun]=angletmp3[xun]-anglef3[xun];
		
	if(fabs(angletmp[xun])<180){angle[xun]=angletmp[xun];}						//将相对角度赋给angle[6]
  else if (angletmp[xun]<-180){angle[xun]=angletmp[xun]+360;}				//采集值正向跨过零点
  else if (angletmp[xun]>180){angle[xun]=angletmp[xun]-360;}				//采集值反向跨过零点
  else{;}
		
	if(fabs(angletmp2[xun])<180){angle2[xun]=angletmp2[xun];}
	else if (angletmp2[xun]<-180){angle2[xun]=angletmp2[xun]+360;}	
	else if (angletmp2[xun]>180){angle2[xun]=angletmp2[xun]-360;}
	else{;}
		
	if(fabs(angletmp3[xun])<180){angle3[xun]=angletmp3[xun];}
	else if (angletmp3[xun]<-180){angle3[xun]=angletmp3[xun]+360;}
	else if (angletmp3[xun]>180){angle3[xun]=angletmp3[xun]-360;}
	else{;}
	
			
	if(angle[xun]<-180){angle[xun]+=360;}  													//采集值正向跨过零点，保证采集值在-180°~180°
	if(angle[xun]>180){angle[xun]+=-360;}; 													//采集值反向跨过零点
		
	if(angle2[xun]<-180){angle2[xun]+=360;}
	if(angle2[xun]>180){angle2[xun]+=-360;};
	
	if(angle3[xun]<-180){angle3[xun]+=360;}
	if(angle3[xun]>180){angle3[xun]+=-360;};
	
		
}; 
  
//  if(OUT1)err1_flag=0;//正常为0，错误为1
//  else err1_flag=1;
//  if(OUT2)err2_flag=0;
//  else err2_flag=1;

		angle_udp[0] = angle[0];	angle_udp[1] = angle[1];	angle_udp[2] = angle[2];	angle_udp[3] = angle[3];	angle_udp[4] = angle[4];	angle_udp[5] = angle[5];
		angle_udp[6] = angle2[0];	angle_udp[7] = angle2[1];	angle_udp[8] = angle2[2];	angle_udp[9] = angle2[3];	angle_udp[10] = angle2[4];	angle_udp[11] = angle2[5];
		angle_udp[12] = angle3[0];	angle_udp[13] = angle3[1];	angle_udp[14] = angle3[2];	angle_udp[15] = angle3[3];	angle_udp[16] = angle3[4];	angle_udp[17] = angle3[5];
		
//			printf("\n\r");
//	printf("index angle 1 : %0.2f\n",angle[0]);
//	printf("index angle 2 : %0.2f\n",angle[1]);
//	printf("index angle 3 : %0.2f\n",angle[2]);
//	printf("index angle 4 : %0.2f\n",angle[3]);
//	printf("index angle 5 : %0.2f\n",angle[4]);
//	printf("index angle 6 : %0.2f\n",angle[5]);
}
void GetDegree_(void) {
	GetDegree();
	
	printf("\n\r");
	printf("index angle 1 : %0.2f\n",angle[0]);
	printf("index angle 2 : %0.2f\n",angle[1]);
	printf("index angle 3 : %0.2f\n",angle[2]);
	printf("index angle 4 : %0.2f\n",angle[3]);
	printf("index angle 5 : %0.2f\n",angle[4]);
	printf("index angle 6 : %0.2f\n",angle[5]);
}
/*                                                                 */
/*******************************************************************/
void send_bytes(void) //发送单传感器数据字节串
  {
		
  uint8_t dat=0;
		
//  dat=angle[0];//先发数据高字节
//printf("%d %f\r\n",ZF_flag[0],angle[0]);
//  dat=angle[1];//再发数据低字节
	printf("%f\r\n",angle[5]);
//  dat=err1_flag;//再发磁铁近指示位1
//printf("%d\r\n",dat);
//  dat=err2_flag;//再发磁铁远指示位2
//printf("%d\r\n",dat);
//  dat=0;//再发0
//printf("%d\r\n",dat); 
 }
	
 
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
//void GPIO_Configuration(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  /* CAN Periph clock enable */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO ,ENABLE);
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
//  GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE); 
//  /* Configure CAN pin: RX */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);   
//  /* Configure CAN pin: TX */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);	     

//}
