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
	printf("260727EXAM4\n");

	/* 아래 코드 수정 금지 : Port-C Clock Enable */
	Macro_Set_Bit(RCC->AHB1ENR, 1); // PORTB clk on 

	// KEY(PC13)을 GPIO 입력으로 선언
	GPIOB->MODER &= ~(0x3 << 16); // PB8 핀 모드 input 0x00 설정
	GPIOB->PUPDR &= ~(0x3 << 16);
	GPIOB->PUPDR |= (0x2 << 16); // 01 pullup, 10 pulldown

	int flag = 0;

	for(;;)
	{
		if((flag == 0) && Macro_Check_Bit_Clear(GPIOB->IDR,8))
		{
			Macro_Invert_Bit(GPIOA->ODR, 5);
			flag = 1;
		}
		else if((flag == 1) && Macro_Check_Bit_Set(GPIOB->IDR,8))
		{
			flag = 0;
		}
	}
}
