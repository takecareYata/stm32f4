#include "device_driver.h"

#define TIM2_TICK         	(20) 					// usec
#define TIM2_FREQ 	  		(1000000./TIM2_TICK)	// Hz
#define TIME2_PLS_OF_1ms  	(1000./TIM2_TICK)
#define TIM2_MAX	  		(0xffffffffu)

void TIM2_Stopwatch_Start(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->PSC = (unsigned int)(TIMXCLK/50000.0 + 0.5)-1;
	TIM2->ARR = TIM2_MAX;

	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Set_Bit(TIM2->CR1, 0);
}

unsigned int TIM2_Stopwatch_Stop(void)
{
	unsigned int time;

	Macro_Clear_Bit(TIM2->CR1, 0);
	time = (TIM2_MAX - TIM2->CNT) * TIM2_TICK;
	return time;
}

void TIM2_Delay(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->PSC = (unsigned int)(TIMXCLK/TIM2_FREQ + 0.5)-1;
	TIM2->ARR = TIME2_PLS_OF_1ms * time;

	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Clear_Bit(TIM2->SR, 0);
	Macro_Set_Bit(TIM2->CR1, 0);

	while(Macro_Check_Bit_Clear(TIM2->SR, 0));
	Macro_Clear_Bit(TIM2->CR1, 0);
}

#define TIM4_TICK	  		(20) 					// usec
#define TIM4_FREQ 	  		(1000000./TIM4_TICK) 	// Hz
#define TIME4_PLS_OF_1ms  	(1000./TIM4_TICK)
#define TIM4_MAX	  		(0xffffu)

void TIM4_Repeat_Interrupt_Enable(int en, int time)
{
	if(en)
	{
		Macro_Set_Bit(RCC->APB1ENR, 2);

		TIM4->CR1 = (1<<4)|(0<<3);
		TIM4->PSC = (unsigned int)(TIMXCLK/TIM4_FREQ + 0.5)-1;
		TIM4->ARR = TIME4_PLS_OF_1ms * time;
		Macro_Set_Bit(TIM4->EGR,0);

		Macro_Clear_Bit(TIM4->SR, 0);
		NVIC_ClearPendingIRQ(30);

		Macro_Set_Bit(TIM4->DIER, 0);
		NVIC_EnableIRQ(30);

		Macro_Set_Bit(TIM4->CR1, 0);
	}

	else
	{
		NVIC_DisableIRQ(30);
		Macro_Clear_Bit(TIM4->CR1, 0);
		Macro_Clear_Bit(TIM4->DIER, 0);
	}
}

void TIM2_Out_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 0);
	Macro_Set_Bit(RCC->APB1ENR, 0);

	Macro_Write_Block(GPIOA->MODER, 0x3, 0x2, 10);  	// PA5 => ALT
	Macro_Write_Block(GPIOA->AFR[0], 0xf, 0x1, 20); 	// PA5 => AF01

	Macro_Write_Block(TIM2->CCMR1,0xff, 0x60, 0);
	TIM2->CCER = (0<<1)|(1<<0);
}

void TIM2_Out_PWM_Generation(unsigned short freq, int duty)
{
	// Timer 주파수가 TIM2_FREQ가 되도록 PSC 설정
	TIM2->PSC = (unsigned int)((double)TIMXCLK/TIM2_FREQ + 0.5) - 1; 
	// 요청한 주파수가 되도록 ARR 설정
	TIM2->ARR = (unsigned int)((double)TIM2_FREQ / freq + 0.5) - 1;
	// Duty Rate 50%가 되도록 CCR1 설정
	TIM2->CCR1 = (unsigned int)(TIM2->ARR * (double)((double)duty / 100.) + 0.5);
	// Manual Update(UG 발생)
	Macro_Set_Bit(TIM2->EGR,0);
	// Down Counter, Repeat Mode, Timer Start
	TIM2->CR1 = (1<<4)|(0<<3)|(1<<0);
}


void TIM4_Repeat(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);

	TIM4->CR1 = (1<<4)|(0<<3);
	TIM4->PSC = (unsigned int)(TIMXCLK/TIM4_FREQ + 0.5)-1;
	TIM4->ARR = TIME4_PLS_OF_1ms * time - 1;

	Macro_Set_Bit(TIM4->EGR,0);
	Macro_Clear_Bit(TIM4->SR, 0);
	Macro_Set_Bit(TIM4->CR1, 0);
}

int TIM4_Check_Timeout(void)
{
	if(Macro_Check_Bit_Set(TIM4->SR, 0))
	{
		Macro_Clear_Bit(TIM4->SR, 0);
		return 1;
	}
	else
	{
		return 0;
	}
}

void TIM4_Stop(void)
{
	Macro_Clear_Bit(TIM4->CR1, 0);
}

void TIM4_Change_Value(int time)
{
	TIM4->ARR = TIME4_PLS_OF_1ms * time;
}

#define TIM3_FREQ 	  			(8000000.) 	      		// Hz
#define TIM3_TICK	  			(1000000./TIM3_FREQ)	// usec
#define TIME3_PLS_OF_1ms  		(1000./TIM3_TICK)

void TIM3_Out_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 1);
	Macro_Set_Bit(RCC->APB1ENR, 1);

	Macro_Write_Block(GPIOB->MODER, 0x3, 0x2, 0);
	Macro_Write_Block(GPIOB->AFR[0], 0xf, 0x2, 0);

	Macro_Write_Block(TIM3->CCMR2,0xff, 0x68, 0);
	TIM3->CCER = (0<<9)|(1<<8);
}

void TIM3_Out_Freq_Generation(unsigned short freq)
{
	TIM3->PSC = (unsigned int)(TIMXCLK/TIM3_FREQ + 0.5)-1;
	TIM3->ARR = TIM3_FREQ/freq-1;
	TIM3->CCR3 = TIM3->ARR/2;

	Macro_Set_Bit(TIM3->EGR,0);
	TIM3->CR1 = (1<<4)|(0<<3)|(0<<1)|(1<<0);
}

void TIM3_Out_Stop(void)
{
	Macro_Clear_Bit(TIM3->CR1, 0);
}

void TIM3_Delay_ms(uint32_t ms)
{
    Macro_Set_Bit(RCC->APB1ENR, 1); // TIM3 Clock Enable (Bit 1)

    // 1ms 주기로 카운팅되도록 설정 (TIMXCLK = 96MHz 기준)
    TIM3->PSC = 9600 - 1;           // 96MHz / 9600 = 10kHz (0.1ms 단위)
    TIM3->ARR = (10 * ms) - 1;      // 요청한 ms만큼 ARR 설정

    TIM3->EGR |= (1 << 0);          // UG = 1 (Re-initialize)
    TIM3->SR &= ~(1 << 0);          // UIF 클리어
    TIM3->CR1 |= (1 << 0) | (1 << 3); // CEN = 1 (Start), OPM = 1 (One-pulse mode)

    while (!(TIM3->SR & (1 << 0))); // 딜레이 완료 대기
}
