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

/* Key 인식 */

#if 0

void Main(void)
{
	Sys_Init(115200);
	printf("KEY Input Test #1\n");

	/* 아래 코드 수정 금지 : Port-C Clock Enable */
	Macro_Set_Bit(RCC->AHB1ENR, 2); // PORTC clk on 

	// KEY(PC13)을 GPIO 입력으로 선언
	GPIOC->MODER &= ~(0x3 << 26); // PC13 핀 모드 input 0x00 설정

	for(;;)
	{
		// KEY가 눌렸으면 LED(PA5) ON, 안 눌렸으면 OFF
		if((GPIOC->IDR >> 13) & 0x1)
		{
			LED_Off();
		}
		else
		{
			LED_On();
		}
	}
}
#endif
#if 0
void Main(void)
{
	Sys_Init(115200);
	printf("KEY Input Test #1\n");

	/* 아래 코드 수정 금지 : Port-C Clock Enable */
	Macro_Set_Bit(RCC->AHB1ENR, 2); // PORTC clk on 

	// KEY(PC13)을 GPIO 입력으로 선언
	Macro_Write_Block(GPIOC->MODER,0x3,0x0,26); // PC13 핀 모드 input 0x00 설정

	for(;;)
	{
#if 0
		// KEY가 눌렸으면 LED(PA5) ON, 안 눌렸으면 OFF
		if(Macro_Check_Bit_Set(GPIOC->IDR,13))
		{
			LED_Off();
		}
		else
		{
			LED_On();
		}
#else
		int k = Macro_Extract_Area(~GPIOC->IDR, 0x1, 13); 
		Macro_Write_Block(GPIOA->ODR, 0x1, k, 5);
#endif

	}
}

#endif

#if 1 // 과제 PC7에 연결한 Key 인식
void Main(void)
{
	Sys_Init(115200);
	printf("KEY Input Test #1\n");

	/* 아래 코드 수정 금지 : Port-C Clock Enable */
	Macro_Set_Bit(RCC->AHB1ENR, 1); // PORTB clk on 

	// KEY(PC13)을 GPIO 입력으로 선언
	GPIOB->MODER &= ~(0x3 << 16); // PB8 핀 모드 input 0x00 설정
	GPIOB->PUPDR &= ~(0x3 << 16);
	GPIOB->PUPDR |= (0x2 << 16); // 01 pullup, 10 pulldown

	for(;;)
	{
		// KEY가 눌렸으면 LED(PA5) ON, 안 눌렸으면 OFF
		if((GPIOB->IDR >> 8) & 0x1)
		{
			GPIOA->ODR |= (0x1<<5);
		}
		else
		{
			GPIOA->ODR &= ~(0x1<<5);
		}
	}
}
#endif

/* Key에 의한 LED Toggling */

#if 0

void Main(void)
{
	Sys_Init(115200);
	printf("KEY Input Toggling #1\n");

	Macro_Set_Bit(RCC->AHB1ENR, 2); 
	Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 26);

	int flag = 0;

	for(;;)
	{
		// KEY(PC13)이 눌릴때마다 LED(PA5)가 Toggling하도록 코드 작성
		if((flag == 0) && Macro_Check_Bit_Clear(GPIOC->IDR,13))
		{
			Macro_Invert_Bit(GPIOA->ODR, 5);
			flag = 1;
		}
		else if((flag == 1) && Macro_Check_Bit_Set(GPIOC->IDR,13))
		{
			flag = 0;
		}
	}
}
#endif

#if 0
void Main(void)
{
	Sys_Init(115200);
	printf("KEY Input Test\n");

	Macro_Set_Bit(RCC->AHB1ENR,1); // portB CLK init
	Macro_Set_Bit(RCC->AHB1ENR,2); // portC CLK init

	GPIOC->MODER &= ~(0x3 << 14); // PC7 INPUT init
	GPIOC->PUPDR &= ~(0x3 << 14); 
	GPIOC->PUPDR |= (0x1 << 14); // PC7 pull up init

	GPIOB->MODER &= ~(0x3 << 6); // PB3 output init
	GPIOB->MODER |= (0x1 << 6);
	GPIOB->ODR |= (0x1 << 3); // 초기값 led off (pull up)

	int flag = 0;

	for(;;)
	{
		if ((flag == 0) && (Macro_Check_Bit_Clear(GPIOC->IDR,7)))
		{
			Macro_Invert_Bit(GPIOB->ODR,3);
			flag = 1;
		}
		else if ((flag == 1) && (Macro_Check_Bit_Set(GPIOC->IDR,7)))
		{
			flag = 0;
		}
	}
}
#endif

