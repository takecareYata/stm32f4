#include "device_driver.h"
#include <stdio.h>

static void Sys_Init(int baud) 
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Clock_Init();
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
}

volatile int Key_Pressed = 0;
volatile int Uart_Data_In = 0;
volatile unsigned char Uart_Data = 0;
volatile int TIM4_Expired = 0;

void Main(void)
{
	Sys_Init(115200);
	printf("\n260727EXAM2\n");

	TIM4_Repeat_Interrupt_Enable(1, 100);

	int d = 0;

	for(;;)
	{	
		if(TIM4_Expired)
	    {
			(d ^= 1) ? LED_On() : LED_Off();
			TIM4_Expired = 0;
	    }
	}
}
