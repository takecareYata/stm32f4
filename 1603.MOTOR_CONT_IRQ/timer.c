#include "device_driver.h"
#define TIM2_TICK 20					// usec
#define TIM2_FREQ (1000000 / TIM2_TICK) // Hz
#define TIM2_PULSE_MSEC (1000 / TIM2_TICK)
#define TIM2_MAX (0xffff)

#define TIM2_TIMER_TIME (3000) // msec

#define TIM4_TICK 20					// usec
#define TIM4_FREQ (1000000 / TIM4_TICK) // Hz
#define TIM4_PULSE_MSEC (1000 / TIM4_TICK)

void TIM2_Timer_Start()
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->CR1 = (0x1 << 4) | (0x1 << 3);
	TIM2->PSC = (unsigned int)((PCLK1 * 2) / (double)TIM2_FREQ + 0.5) - 1;
	TIM2->ARR = TIM2_TIMER_TIME * TIM2_PULSE_MSEC;

	Macro_Set_Bit(TIM2->EGR, 0);
	Macro_Clear_Bit(TIM2->SR, 0);
	// TIM2 start
	Macro_Set_Bit(TIM2->CR1, 0);
}

unsigned int TIM2_Timer_Stop()
{
	Macro_Clear_Bit(TIM2->CR1, 0);

	return TIM2_TIMER_TIME - TIM2->CNT / TIM2_PULSE_MSEC;
}

int TIM2_Check_Timeout(void)
{
	// 타이머가 timeout 이면 1 리턴, 아니면 0 리턴
	if (Macro_Check_Bit_Set(TIM2->SR, 0))
	{
		Macro_Clear_Bit(TIM2->SR, 0);
		return 1;
	}
	return 0;
}

void TIM2_Stopwatch_Start(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 = (0x1 << 4) | (0x1 << 3);
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (unsigned int)((PCLK1 * 2) / (double)TIM2_FREQ + 0.5) - 1;
	// ARR 초기값 설정 => 최대값 0xFFFF 설정
	TIM2->ARR = TIM2_MAX;
	// UG 이벤트 발생
	Macro_Set_Bit(TIM2->EGR, 0);
	// TIM2 start
	Macro_Set_Bit(TIM2->CR1, 0);
}

unsigned int TIM2_Stopwatch_Stop(void)
{
	unsigned int time;

	// TIM2 stop
	Macro_Clear_Bit(TIM2->CR1, 0);
	// CNT 초기 설정값 (0xffff)와 현재 CNT의 펄스수 차이를 구하고
	// 그 펄스수 하나가 20usec이므로 20을 곱한값을 time에 저장
	time = (TIM2_MAX - TIM2->CNT) * TIM2_TICK;
	// 계산된 time 값을 리턴(단위는 usec)
	return time;
}

void TIM2_Delay(int time) // msec
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 = (1 << 4) | (1 << 3);
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (unsigned int)((PCLK1 * 2) / (double)TIM2_FREQ + 0.5) - 1;
	// ARR 초기값 설정 => 요청한 time msec에 해당하는 초기값 설정
	TIM2->ARR = time * TIM2_PULSE_MSEC - 1;
	// UG 이벤트 발생
	Macro_Set_Bit(TIM2->EGR, 0);

	// UIF(Update Interrupt Pending) Clear
	Macro_Clear_Bit(TIM2->SR, 0); // 중요
	// TIM2 start
	Macro_Set_Bit(TIM2->CR1, 0);
	// Wait timeout
	while (!Macro_Check_Bit_Set(TIM2->SR, 0))
		;
	// TIM2 Stop
	Macro_Clear_Bit(TIM2->CR1, 0);
}

void TIM4_Delay(int time) // msec
{
	Macro_Set_Bit(RCC->APB1ENR, 2);

	// TIM2 CR1 설정: down count, one pulse
	TIM4->CR1 = (1 << 4) | (1 << 3);
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM4->PSC = (unsigned int)((PCLK1 * 2) / (double)TIM4_FREQ + 0.5) - 1;
	// ARR 초기값 설정 => 요청한 time msec에 해당하는 초기값 설정
	TIM4->ARR = time * TIM4_PULSE_MSEC - 1;
	// UG 이벤트 발생
	Macro_Set_Bit(TIM4->EGR, 0);

	// UIF(Update Interrupt Pending) Clear
	Macro_Clear_Bit(TIM4->SR, 0); // 중요
	// TIM2 start
	Macro_Set_Bit(TIM4->CR1, 0);
	// Wait timeout
	while (!Macro_Check_Bit_Set(TIM4->SR, 0))
		;
	// TIM2 Stop
	Macro_Clear_Bit(TIM4->CR1, 0);
}

void TIM4_Repeat(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);

	// TIM4 CR1: ARPE=0, down counter, repeat mode
	TIM4->CR1 = (1 << 4);
	// PSC(50KHz),  ARR(reload시 값) 설정
	TIM4->PSC = (unsigned int)(PCLK1 * 2 / (double)TIM4_FREQ + 0.5) - 1;
	TIM4->ARR = time * TIM4_PULSE_MSEC - 1;
	// UG 이벤트 발생
	Macro_Set_Bit(TIM4->EGR, 0);
	// Update Interrupt Pending Clear
	Macro_Clear_Bit(TIM4->SR, 0);
	// TIM4 start
	Macro_Set_Bit(TIM4->CR1, 0);
}

int TIM4_Check_Timeout(void)
{
	// 타이머가 timeout 이면 1 리턴, 아니면 0 리턴
	if (Macro_Check_Bit_Set(TIM4->SR, 0))
	{
		Macro_Clear_Bit(TIM4->SR, 0);
		return 1;
	}
	return 0;
}

void TIM4_Stop(void)
{
	Macro_Clear_Bit(TIM4->CR1, 0);
}

void TIM4_Change_Value(int time)
{
	TIM4->ARR = 50 * time;
}

#define TIM5_FREQ                   (8000000)           // Hz
#define TIM5_TICK                   (1000000/TIM5_FREQ) // usec
#define TIME5_PLS_OF_1ms            (1000/TIM5_TICK)

void TIM5_Out_Init(void)
{
    Macro_Set_Bit(RCC->AHB1ENR, 0); // GPIOA Clock (AHB1 bit 0)
    Macro_Set_Bit(RCC->APB1ENR, 3); // TIM5 Clock  (APB1 bit 3)

    // MODER: PA0(bit 1:0) = 0x2, PA1(bit 3:2) = 0x2
    Macro_Write_Block(GPIOA->MODER, 0x3, 0x2, 0);   // PA0 => ALT
    Macro_Write_Block(GPIOA->MODER, 0x3, 0x2, 2);   // PA1 => ALT
    // AFR[0]: PA0(bit 3:0) = AF2, PA1(bit 7:4) = AF2
    Macro_Write_Block(GPIOA->AFR[0], 0xF, 0x2, 0);  // PA0 => AF02
    Macro_Write_Block(GPIOA->AFR[0], 0xF, 0x2, 4);  // PA1 => AF02

    // CCMR1 하위 8비트는 CH1, 상위 8비트는 CH2를 담당합니다.
    Macro_Write_Block(TIM5->CCMR1, 0xFF, 0x60, 0);  // CH1 PWM Mode 1
    Macro_Write_Block(TIM5->CCMR1, 0xFF, 0x60, 8);  // CH2 PWM Mode 1

    // CC1E = bit 0, CC2E = bit 4
    TIM5->CCER |= (1 << 0) | (1 << 4);
}

void TIM5_Out_Freq_Generation(unsigned short freq)
{
    // Timer 주파수가 TIM5_FREQ가 되도록 PSC 설정
    TIM5->PSC = (unsigned int)((double)TIMXCLK / TIM5_FREQ + 0.5) - 1; 
    // 요청한 주파수가 되도록 ARR 설정
    TIM5->ARR = (unsigned int)((double)TIM5_FREQ / freq + 0.5) - 1;
    // Duty Rate 50%가 되도록 CCR1 설정
    TIM5->CCR1 = TIM5->ARR / 2;
    // Manual Update (UG 발생)
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
    // 요청한 Duty Rate가 되도록 CCR1 설정
    TIM5->CCR1 = (unsigned int)(TIM5->ARR * (double)((double)duty / 100.) + 0.5);
    // Manual Update (UG 발생)
    Macro_Set_Bit(TIM5->EGR, 0);
    // Down Counter, Repeat Mode, Timer Start
    TIM5->CR1 = (1 << 4) | (0 << 3) | (1 << 0);
}

void TIM5_Out_Stop(void)
{
    Macro_Clear_Bit(TIM5->CR1, 0);
}

void TIM5_CH2_Out_PWM_Generation(unsigned short freq, int duty)
{
    // Timer 주파수가 TIM5_FREQ가 되도록 PSC 설정
    TIM5->PSC = (unsigned int)((double)TIMXCLK / TIM5_FREQ + 0.5) - 1; 
    // 요청한 주파수가 되도록 ARR 설정
    TIM5->ARR = (unsigned int)((double)TIM5_FREQ / freq + 0.5) - 1;
    
    // CH2 전용 레지스터인 CCR2에 Duty Rate 설정
    TIM5->CCR2 = (unsigned int)(TIM5->ARR * (double)((double)duty / 100.) + 0.5);
    
    // Manual Update (UG 발생)
    Macro_Set_Bit(TIM5->EGR, 0);
    // Down Counter, Repeat Mode, Timer Start
    TIM5->CR1 = (1 << 4) | (0 << 3) | (1 << 0);
}

void TIM5_CH2_Out_Stop(void)
{
    // CC2E (bit 4) 비트만 Clear하여 CH2 출력만 끕니다. (타이머 전체를 끄지 않음)
    Macro_Clear_Bit(TIM5->CCER, 4);
}
