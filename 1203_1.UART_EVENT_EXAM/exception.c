#include "device_driver.h"
#include <stdio.h>

void _Invalid_ISR(void)
{
	unsigned int r = Macro_Extract_Area(SCB->ICSR, 0x1ff, 0);
	printf("\nInvalid_Exception: %d!\n", r);
	printf("Invalid_ISR: %d!\n", r - 16);
	for(;;);
}

extern volatile int Key_Pressed;

void EXTI15_10_IRQHandler(void)
{
	Key_Pressed = 1;
	
	EXTI->PR = 0x1 << 13;
	NVIC_ClearPendingIRQ(40);
}

extern volatile int Uart_Data_In;
extern volatile unsigned char Uart_Data;

extern volatile char *p;
extern volatile int len;
extern volatile int Uart_Tx_End;

void USART2_IRQHandler(void)
{
	// tx IRQ 확인용
	static int i = 0;	
	USART2->DR = p[i];
	
	if(p[i] == '\0')
	{
		Uart_Tx_End = 1;
		Macro_Clear_Bit(USART2->CR1, 7);
		NVIC_DisableIRQ(38);
	}

	i++;
	NVIC_ClearPendingIRQ(38);
}
