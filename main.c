
#include	"./driver/config.h"
#include	"./driver/USART1.h"
#include	"./driver/delay.h"
#include	"./driver/GPIO.h"
#include	"./driver/Exti.h"


/*************	功能说明	**************

双串口全双工中断方式收发通讯程序。

通过PC向MCU发送数据, MCU收到后通过串口把收到的数据原样返回.

******************************************/

/*************	本地常量声明	**************/



/*************	本地变量声明	**************/

u8 HRflag = 0;
u8 BT0Flag = 1;
u8 BT1Flag = 1;
u8 BT2Flag = 1;


/*************	本地函数声明	**************/
void  HumanRadar();
void	Button0();
void	Button1();
void	Button2();
void SPortCommand(u8 SPortCommand[32]);

/*************  外部函数和变量声明 *****************/


/*************  串口1初始化函数 *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 9600ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//初始化串口1 USART1,USART2

	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1发送一个字符串
}

/*************  GPIO初始化函数 *****************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	
	GPIO_InitStructure.Pin  = GPIO_Pin_1|GPIO_Pin_3;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//初始化
	
	P11 = 0;
	P13 = 1;
	
	//按键初始化
	GPIO_InitStructure.Pin  = GPIO_Pin_5; 		//指定要初始化的IO, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;					//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);				//P3.2、P3.3初始化为上拉输入
	P15 = 1;
	
	//按键初始化
	GPIO_InitStructure.Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6; 		//指定要初始化的IO, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;					//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);				//P3.2、P3.3初始化为上拉输入
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
		if(COM1.RX_TimeOut > 0)		//超时计数
		{
			if(--COM1.RX_TimeOut == 0)
			{
				if(COM1.RX_Cnt > 0)
				{
					//for(i=0; i<COM1.RX_Cnt; i++)	TX1_write2buff(RX1_Buffer[i]);	//收到的数据原样返回
					
						u8 Temp[32] = ""; 
					for(i=0; i<32; i++){
						Temp[i] = RX1_Buffer[i] ;
					}
					SPortCommand(Temp);

				}
				//PrintString1("SPort ok\r\n");	//SUART1发送一个字符串
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
						PrintString1("DAH01");	//SUART1发送一个字符串
						delay_ms(1000);
						HRflag = 1;
					}
					
				}
				else{
						P11 = 0;
						PrintString1("EAH01");	//SUART1发送一个字符串
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

/***串口指令处理函数***/
void SPortCommand(u8 SPortCommand[32]){
	
	u8 a ;//计数变量
	//PrintString1("Start ok\r\n");	//SUART1发送一个字符串
	/*****循环检测帧头*******/
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
	
