
#include	"./driver/config.h"
#include	"./driver/USART1.h"
#include	"./driver/delay.h"
#include	"./driver/GPIO.h"
#include	"./driver/Exti.h"


/*************	����˵��	**************

˫����ȫ˫���жϷ�ʽ�շ�ͨѶ����

ͨ��PC��MCU��������, MCU�յ���ͨ�����ڰ��յ�������ԭ������.

******************************************/

/*************	���س�������	**************/



/*************	���ر�������	**************/

u8 HRflag = 0;
u8 BT0Flag = 1;
u8 BT1Flag = 1;
u8 BT2Flag = 1;


/*************	���غ�������	**************/
void  HumanRadar();
void	Button0();
void	Button1();
void	Button2();
void SPortCommand(u8 SPortCommand[32]);

/*************  �ⲿ�����ͱ������� *****************/


/*************  ����1��ʼ������ *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 9600ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//��ʼ������1 USART1,USART2

	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1����һ���ַ���
}

/*************  GPIO��ʼ������ *****************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	
	GPIO_InitStructure.Pin  = GPIO_Pin_1|GPIO_Pin_3;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//��ʼ��
	
	P11 = 0;
	P13 = 1;
	
	//������ʼ��
	GPIO_InitStructure.Pin  = GPIO_Pin_5; 		//ָ��Ҫ��ʼ����IO, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;					//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);				//P3.2��P3.3��ʼ��Ϊ��������
	P15 = 1;
	
	//������ʼ��
	GPIO_InitStructure.Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6; 		//ָ��Ҫ��ʼ����IO, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;					//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);				//P3.2��P3.3��ʼ��Ϊ��������
	P32 = 1;
	P33 = 1;
	P36 = 1;
}


/**********************************************/

void main(void)
{
	u8	i;

	UART_config();
	//EXTI_config();
	GPIO_config();
	
	
	EA = 1;

	while (1)
	{
			Button0();
			Button1();
			Button2();
		 HumanRadar();
		delay_ms(1);
		if(COM1.RX_TimeOut > 0)		//��ʱ����
		{
			if(--COM1.RX_TimeOut == 0)
			{
				if(COM1.RX_Cnt > 0)
				{
					//for(i=0; i<COM1.RX_Cnt; i++)	TX1_write2buff(RX1_Buffer[i]);	//�յ�������ԭ������
					
						u8 Temp[32] = ""; 
					for(i=0; i<32; i++){
						Temp[i] = RX1_Buffer[i] ;
					}
					SPortCommand(Temp);

				}
				//PrintString1("SPort ok\r\n");	//SUART1����һ���ַ���
				COM1.RX_Cnt = 0;
			}
		}
	}
}

void  HumanRadar(){
			if(P15 == 0){
				delay_ms(1000);
				
				if(P15 == 0){
					P11 = 1;
					
					if(HRflag ==0){
						PrintString1("DAH01");	//SUART1����һ���ַ���
						delay_ms(1000);
						HRflag = 1;
					}
					
				}
				else{
						P11 = 0;
						PrintString1("EAH01");	//SUART1����һ���ַ���
						HRflag = 0;
						delay_ms(1000);
				}
			}		
				
}

void	Button0(){
	
		if(P32 != BT0Flag ){
				delay_ms(1000);
			if(P32 != BT0Flag){
				BT0Flag = P32;
				P11 = ~P11;
				
				if(BT0Flag == 0){
					PrintString1("EAB01");	
				}
				else{
					PrintString1("EAB00");	
				}
			}
		
		}
}

void	Button1(){
	
		if(P33 != BT1Flag ){
				delay_ms(1000);
			if(P33 != BT1Flag){
				BT1Flag = P33 ;
				P11 = ~P11;
				
				if(BT1Flag == 0){
					PrintString1("EAB11");	
				}
				else{
					PrintString1("EAB10");	
				}
			}
		
		}
}



void	Button2(){
	
		if(P36 != BT2Flag ){
				delay_ms(1000);
			if(P36 != BT2Flag){
				BT2Flag = P36;
				P11 = ~P11;
				
				if(BT2Flag == 0){
					PrintString1("EAB21");	
				}
				else{
					PrintString1("EAB20");	
				}
			}
		
		}
}

/***����ָ�����***/
void SPortCommand(u8 SPortCommand[32]){
	
	u8 a ;//��������
	//PrintString1("Start ok\r\n");	//SUART1����һ���ַ���
	/*****ѭ�����֡ͷ*******/
	for(a=0;a<=32;a++){
		if(SPortCommand[a] == 'D'){
		//	u8 b;
			//u8 c;
			a++;
			if( 'A'==SPortCommand[a]){
			a++;
							if('1'==SPortCommand[a]){
								P11 = 1;
								PrintString1("EAL01");	
							}
							if('0'==SPortCommand[a]){
								P11 = 0;
								PrintString1("EAL01");
							}
						}
			}
		}
	}
	
