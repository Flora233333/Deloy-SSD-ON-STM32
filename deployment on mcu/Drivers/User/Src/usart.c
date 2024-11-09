/***
	*************************************************************************************************
	*	@version V1.0
	*  @date    2023-12-13
	*	@author  反客科技	
   *************************************************************************************************
   *  @description
	*
	*	实验平台：反客STM32H743XIH6核心板 （型号：FK743M5-XIH6）
	*	淘宝地址：https://shop212360197.taobao.com
	*	QQ交流群：536665479
	*
>>>>> 文件说明：
	*
	*  初始化usart引脚，配置波特率等参数
	*
	************************************************************************************************
***/


#include "usart.h"
#include "stm32h7xx_hal.h"


UART_HandleTypeDef huart1;  // UART_HandleTypeDef 结构体变量


/*************************************************************************************************
*	函 数 名:	HAL_UART_MspInit
*	入口参数:	huart - UART_HandleTypeDef定义的变量，即表示定义的串口
*	返 回 值:	无
*	函数功能:	初始化串口引脚
*	说    明:	无		
*************************************************************************************************/


void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	if(huart->Instance==USART1)
	{
		__HAL_RCC_USART1_CLK_ENABLE();		// 开启 USART1 时钟

		GPIO_USART1_TX_CLK_ENABLE;				// 开启 USART1 TX 引脚的 GPIO 时钟
		GPIO_USART1_RX_CLK_ENABLE;				// 开启 USART1 RX 引脚的 GPIO 时钟

		GPIO_InitStruct.Pin 			= USART1_TX_PIN;					// TX引脚
		GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;				// 复用推挽输出
		GPIO_InitStruct.Pull 		= GPIO_PULLUP;						// 上拉
		GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;	// 速度等级 
		GPIO_InitStruct.Alternate 	= GPIO_AF7_USART1;				// 复用为USART1
		HAL_GPIO_Init(USART1_TX_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin 			= USART1_RX_PIN;					// RX引脚
		HAL_GPIO_Init(USART1_RX_PORT, &GPIO_InitStruct);		
	}

}

/*************************************************************************************************
*	函 数 名:	USART1_Init
*	入口参数:	无
*	返 回 值:	无
*	函数功能:	初始化串口配置
*	说    明:	无		 
*************************************************************************************************/

void USART1_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {

  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {

  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {

  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {

  }
}


/*************************************************************************************************
*	在有些场合，例如LVGL因为需要用__aeabi_assert或者TouchGFX，不能勾选 microLib 以使用printf
*	添加以下代码，让标准C库支持重定向fput
*  根据编译器，选择对应的代码即可
*************************************************************************************************/


//// AC5编译器使用这段代码
//#pragma import(__use_no_semihosting)  
//int _ttywrch(int ch)    
//{
//    ch=ch;
//	return ch;
//}         
//struct __FILE 
//{ 
//	int handle; 

//}; 
//FILE __stdout;       

//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 



//// AC6编译器使用这段代码
//__asm (".global __use_no_semihosting\n\t");
//void _sys_exit(int x) 
//{
//  x = x;
//}
////__use_no_semihosting was requested, but _ttywrch was 
//void _ttywrch(int ch)
//{
//    ch = ch;
//}

//FILE __stdout;



/*************************************************************************************************
*	函 数 名:	fputc
*	入口参数:	ch - 要输出的字符 ，  f - 文件指针（这里用不到）
*	返 回 值:	正常时返回字符，出错时返回 EOF（-1）
*	函数功能:	重定向 fputc 函数，目的是使用 printf 函数
*	说    明:	无		
*************************************************************************************************/

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);	// 发送单字节数据
	return (ch);
}

