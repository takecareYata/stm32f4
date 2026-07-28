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

void Main(void)
{
	Sys_Init(115200);
	printf("TIMER LED test\r\n");

	int i = 0;

	TIM4_Repeat(500);

	for(;;)
	{
// 500ms 동안 on 500ms 동안 off
#if 0
		LED_On();
		TIM2_Delay(500);
		LED_Off();
		TIM2_Delay(500);
#endif
#if 0
		i = (i + 1) % 2;
		if(i == 0)
		{
			LED_On();
		}
		else
		{
			LED_Off();
		}

		TIM2_Delay(500);
#endif
#if 0
		Macro_Invert_Bit(GPIOA->ODR,5);
		TIM2_Delay(500);
#endif
#if 1
		if(TIM4_Check_Timeout())
		{
			Macro_Invert_Bit(GPIOA->ODR,5);
		}
		//printf("*");
#endif
		
	

	}
}
