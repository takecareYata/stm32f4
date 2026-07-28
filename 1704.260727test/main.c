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
	printf("\n260727EXAM4\n");

	TIM2_Out_Init();

	TIM2_Out_PWM_Generation(1000,30);
	
	for(;;)
	{	
		
	}
}
