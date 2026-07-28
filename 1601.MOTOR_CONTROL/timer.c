#include "device_driver.h"

#define TIM2_TICK         	(20) 				// usec
#define TIM2_FREQ 	  		(1000000/TIM2_TICK)	// Hz
#define TIME2_PLS_OF_1ms  	(1000/TIM2_TICK)
#define TIM2_MAX	  		(0xffffu)

#define TIM4_TICK	  		(20) 				// usec
#define TIM4_FREQ 	  		(1000000/TIM4_TICK) // Hz
#define TIME4_PLS_OF_1ms  	(1000/TIM4_TICK)
#define TIM4_MAX	  		(0xffffu)

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

/* Delay Time Max = 65536 * 20use = 1.3sec */

#if 0

void TIM2_Delay(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->PSC = (unsigned int)(TIMXCLK/(double)TIM2_FREQ + 0.5)-1;
	TIM2->ARR = TIME2_PLS_OF_1ms * time;

	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Clear_Bit(TIM2->SR, 0);
	Macro_Set_Bit(TIM2->CR1, 0);

	while(Macro_Check_Bit_Clear(TIM2->SR, 0));

	Macro_Clear_Bit(TIM2->CR1, 0);
}

#else

/* Delay Time Extended */

void TIM2_Delay(int time)
{
	int i;
	unsigned int t = TIME2_PLS_OF_1ms * time;

	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->PSC = (unsigned int)(TIMXCLK/(double)TIM2_FREQ + 0.5)-1;
	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->ARR = 0xffff;
	Macro_Set_Bit(TIM2->EGR,0);

	for(i=0; i<(t/0xffffu); i++)
	{
		Macro_Set_Bit(TIM2->EGR,0);
		Macro_Clear_Bit(TIM2->SR, 0);
		Macro_Set_Bit(TIM2->CR1, 0);
		while(Macro_Check_Bit_Clear(TIM2->SR, 0));
	}

	TIM2->ARR = t % 0xffffu;
	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Clear_Bit(TIM2->SR, 0);
	Macro_Set_Bit(TIM2->CR1, 0);
	while (Macro_Check_Bit_Clear(TIM2->SR, 0));

	Macro_Clear_Bit(TIM2->CR1, 0);
}

#endif

void TIM4_Repeat(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);

	TIM4->CR1 = (1<<4)|(0<<3);
	TIM4->PSC = (unsigned int)(TIMXCLK/(double)TIM4_FREQ + 0.5)-1;
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

#define TIM3_FREQ					(8000000)			// Hz
#define TIM3_TICK					(1000000/TIM3_FREQ)	// usec
#define TIME3_PLS_OF_1ms			(1000/TIM3_TICK)

void TIM3_Out_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 1);
	Macro_Set_Bit(RCC->APB1ENR, 1);

	Macro_Write_Block(GPIOB->MODER, 0x3, 0x2, 0);  	// PB0 => ALT
	Macro_Write_Block(GPIOB->AFR[0], 0xf, 0x2, 0); 	// PB0 => AF02

	Macro_Write_Block(TIM3->CCMR2,0xff, 0x60, 0);
	TIM3->CCER = (0<<9)|(1<<8);
}

void TIM3_Out_Freq_Generation(unsigned short freq)
{
	// Timer 주파수가 TIM3_FREQ가 되도록 PSC 설정
	TIM3->PSC = (unsigned int)((double)TIMXCLK/TIM3_FREQ + 0.5) - 1; 
	// 요청한 주파수가 되도록 ARR 설정
	TIM3->ARR = (unsigned int)((double)TIM3_FREQ/freq + 0.5) - 1;
	// Duty Rate 50%가 되도록 CCR3 설정
	TIM3->CCR3 = TIM3->ARR / 2;
	// Manual Update(UG 발생)
	Macro_Set_Bit(TIM3->EGR,0);
	// Down Counter, Repeat Mode, Timer Start
	TIM3->CR1 = (1<<4)|(0<<3)|(1<<0);
}

void TIM3_Out_PWM_Generation(unsigned short freq, int duty)
{
	// Timer 주파수가 TIM3_FREQ가 되도록 PSC 설정
	TIM3->PSC = (unsigned int)((double)TIMXCLK/TIM3_FREQ + 0.5) - 1; 
	// 요청한 주파수가 되도록 ARR 설정
	TIM3->ARR = (unsigned int)((double)TIM3_FREQ / freq + 0.5) - 1;
	// Duty Rate 50%가 되도록 CCR3 설정
	TIM3->CCR3 = (unsigned int)(TIM3->ARR * (double)((double)duty / 100.) + 0.5);
	// Manual Update(UG 발생)
	Macro_Set_Bit(TIM3->EGR,0);
	// Down Counter, Repeat Mode, Timer Start
	TIM3->CR1 = (1<<4)|(0<<3)|(1<<0);
}

void TIM3_Out_Stop(void)
{
	Macro_Clear_Bit(TIM3->CR1, 0);
}

#define TIM5_FREQ                   (8000000)           // Hz
#define TIM5_TICK                   (1000000/TIM5_FREQ) // usec
#define TIM5_PLS_OF_1ms             (1000/TIM5_TICK)

void TIM5_Out_Init(void)
{
    // 1. GPIOA 클럭 (AHB1 Bit 0) 및 TIM5 클럭 (APB1 Bit 3) 활성화
    Macro_Set_Bit(RCC->AHB1ENR, 0); // GPIOA
    Macro_Set_Bit(RCC->APB1ENR, 3); // TIM5

    // 2. PA0 핀을 Alternate Function(0x2) 모드로 설정
    // TIM5_CH1은 보통 PA0 핀에 배정됩니다.
    Macro_Write_Block(GPIOA->MODER, 0x3, 0x2, 0);   // PA0 => Alternate Function
    Macro_Write_Block(GPIOA->AFR[0], 0xF, 0x2, 0);  // PA0 => AF02 (TIM5)

    // 3. TIM5 Channel 1 (CCMR1) PWM Mode 1 설정 (0x60)
    Macro_Write_Block(TIM5->CCMR1, 0xFF, 0x60, 0);  // OC1M = 110 (PWM Mode 1)
    
    // 4. CCER 설정 (Channel 1 Output Enable)
    TIM5->CCER = (0 << 1) | (1 << 0);              // CC1E = 1 (Channel 1 출력 활성화)
}

void TIM5_Out_Freq_Generation(unsigned short freq)
{
    // Timer 주파수가 TIM5_FREQ가 되도록 PSC 설정
    TIM5->PSC = (unsigned int)((double)TIMXCLK / TIM5_FREQ + 0.5) - 1; 
    
    // 요청한 주파수가 되도록 ARR 설정 (TIM5는 32비트 타이머 지원)
    TIM5->ARR = (unsigned int)((double)TIM5_FREQ / freq + 0.5) - 1;
    
    // Duty Rate 50%가 되도록 CCR1 설정 (PA0 = CH1)
    TIM5->CCR1 = TIM5->ARR / 2;
    
    // Manual Update (UG 발생: 레지스터 설정값 즉시 반영)
    Macro_Set_Bit(TIM5->EGR, 0);
    
    // Down Counter, Repeat Mode, Timer Start
    TIM5->CR1 = (1 << 4) | (0 << 3) | (1 << 0);
}

void TIM5_Out_PWM_Generation(unsigned short freq, int duty)
{
    // Timer 주파수가 TIM5_FREQ가 되도록 PSC 설정
    TIM5->PSC = (unsigned int)((double)TIMXCLK / TIM5_FREQ + 0.5) - 1; 
    
    // 요청한 주파수가 되도록 ARR 설정
    TIM5->ARR = (unsigned int)((double)TIM5_FREQ / freq + 0.5) - 1;
    
    // 지정한 Duty Rate가 되도록 CCR1 설정 (PA0 = CH1)
    TIM5->CCR1 = (unsigned int)(TIM5->ARR * (double)((double)duty / 100.0) + 0.5);
    
    // Manual Update (UG 발생)
    Macro_Set_Bit(TIM5->EGR, 0);
    
    // Down Counter, Repeat Mode, Timer Start
    TIM5->CR1 = (1 << 4) | (0 << 3) | (1 << 0);
}

void TIM5_Out_Stop(void)
{
    Macro_Clear_Bit(TIM5->CR1, 0);
}