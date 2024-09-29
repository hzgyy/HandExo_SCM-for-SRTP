#include "usmart.h"
#include "usmart_str.h"
#include "stm32f4x7_eth.h"
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ�����) 
#include "delay.h"	
#include "usart.h"		
#include "sys.h" 	
  
#include "sensor.h"
#include "pwm.h"
//�������б���ʼ��(�û��Լ�����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		   
	(void*)delay_ms,"void delay_ms(u16 nms)",
 	(void*)delay_us,"void delay_us(u32 nus)", 
	(void*)ETH_ReadPHYRegister,"uint16_t ETH_ReadPHYRegister(uint16_t PHYAddress, uint16_t PHYReg)",
	(void*)ETH_WritePHYRegister,"uint32_t ETH_WritePHYRegister(uint16_t PHYAddress, uint16_t PHYReg, uint16_t PHYValue)",
	(void*)PWM_SetCompareLow,"void PWM_SetCompareLow(void)",
	(void*)PWM_SetCompareMid,"void PWM_SetCompareMid(void)",
	(void*)PWM_SetCompareHigh,"void PWM_SetCompareHigh(void)",
	(void*)PWM_SetCompare,"void PWM_SetCompare( u8 ch, u16 count)",
	(void*) GetDegreeo,"void GetDegreeo(void)",
};						


/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   


















