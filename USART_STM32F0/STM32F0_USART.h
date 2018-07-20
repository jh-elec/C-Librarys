#ifndef STM32F0_USART_H_INCLUDED
#define STM32F0_USART_H_INCLUDED

#define USART_NUM       USART1
#define USART_BAUDRATE  9600

#define GPIO_TX_PIN     GPIO_Pin_9
#define GPIO_RX_PIN     GPIO_Pin_10

/*
*   Init. USART
*/
void Usart_Init(void);

void Usart_Send_Str(USART_TypeDef* USARTx, char *c);


#endif /* STM32F0_USART_H_INCLUDED */
