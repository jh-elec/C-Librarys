/*
*   USART Driver
*
*   Build.: 03.02.2017
*/

#include "stm32f0xx_conf.h"
#include "STM32F0_USART.h"

void Usart_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  GPIO_InitStruct.GPIO_Pin      = GPIO_TX_PIN | GPIO_RX_PIN;
  GPIO_InitStruct.GPIO_Mode     = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed    = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_OType    = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd     = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

  USART_InitStruct.USART_BaudRate               = USART_BAUDRATE;
  USART_InitStruct.USART_WordLength             = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits               = USART_StopBits_1;
  USART_InitStruct.USART_Parity                 = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl    = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode                   = USART_Mode_Tx | USART_Mode_Rx;

  USART_Init(USART_NUM, &USART_InitStruct);
  USART_Cmd(USART_NUM, ENABLE);
}

void Usart_Send_Str(USART_TypeDef* USARTx, char *c)
{
    while(*c)
    {
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
        USART_SendData(USARTx,*c++);
    }
}
