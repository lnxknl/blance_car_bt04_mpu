#include "stm32f10x.h"                  // Device header

void BT04_Init(void)
{
	//GPIO端口设置 
		
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	
	//USART1_TX   GPIOA_2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  //USART1_RX	  GPIOA_3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

	NVIC_InitTypeDef NVIC_InitStructure;
	//Usart2 NVIC 配置
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
  
	
  //USART2 初始化设置 
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口2
  
  USART_Cmd(USART2, ENABLE);                    //使能串口2 
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
}

u8 Res;
void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART2);	//读取接收到的数据// @NOTE 
//		USART_SendData(USART2, Res); 
  } 
}

/******		蓝牙遥控		*******/
u8 car;
void BT04_Control(void)// @NOTE 
{
	car = 0;
	if(Res == 'E') car = 0;
	if(Res == 'A') car = 1;
	if(Res == 'B') car = 2;
	if(Res == 'C') car = 3;
	if(Res == 'D') car = 4;	
}
