#include "device_driver.h"

#define TIM2_TICK	20
#define TIM2_FREQ	(1000000./TIM2_TICK) // Hz
#define TM2_1ms_Pls	(TIM2_FREQ/1000.) // EA (펄스 갯수)
#define TIM2_MAX	(0xFFFFFFFFu)

#define TIM4_TICK	20
#define TIM4_FREQ	(1000000./TIM4_TICK) // Hz
#define TM4_1ms_Pls	(TIM4_FREQ/1000.) // EA (펄스 갯수)
#define TIM4_MAX	(0xFFFFu)

void TIM2_Stopwatch_Start(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one shot
	TIM2->CR1 = (1 << 4) | (1 << 3);
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (unsigned int)(TIMXCLK/TIM2_FREQ + 0.5) - 1;
	// ARR 초기값 설정 => 최대값 0xFFFF 설정
	TIM2->ARR = TIM2_MAX;
	// UG 이벤트 발생
	TIM2->EGR |= (0x1 << 0);
	// TIM2 start
	TIM2->CR1 |= (0x1 << 0);
}

unsigned int TIM2_Stopwatch_Stop(void)
{
	unsigned int time;

	// TIM2 stop
	TIM2->CR1 &= ~(0x1 << 0);
	// CNT 초기 설정값 (0xffff)와 현재 CNT의 펄스수 차이를 구하고
	// 그 펄스수 하나가 20usec이므로 20을 곱한값을 time에 저장
	time = (TIM2->ARR - TIM2->CNT) * TIM2_TICK;
	// 계산된 time 값을 리턴(단위는 usec
	return time;
}

#if 0
void TIM2_Delay(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one shot
	TIM2->CR1 = (0x1 << 4) | (0x1 << 3);
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (unsigned int)(TIMXCLK/TIM2_FREQ + 0.5) - 1;
	// ARR 초기값 설정 => 요청한 time msec에 해당하는 초기값 설정
	unsigned int pls = TM2_1ms_Pls * time;
	int n = pls / TIM2_MAX;
	int m = pls % TIM2_MAX;
	int i;

	for(i = 0; i < n; i ++)
	{
		TIM2->ARR = TIM2_MAX;
		// UG 이벤트 발생
		TIM2->EGR |= (0x1 << 0);
		// UIF(Update Interrupt Pending) Clear
		TIM2->SR &= ~(0x1 << 0);
		// TIM2 start
		TIM2->CR1 |= (0x1 << 0);
		// Wait timeout
		while(!Macro_Check_Bit_Set(TIM2->SR,0));
	}

	TIM2->ARR = m;
	TIM2->EGR |= (0x1 << 0);
	// UIF(Update Interrupt Pending) Clear
	TIM2->SR &= ~(0x1 << 0);
	// TIM2 start
	TIM2->CR1 |= (0x1 << 0);
	// Wait timeout
	while(!Macro_Check_Bit_Set(TIM2->SR,0));

	// TIM2 Stop
	Macro_Clear_Bit(TIM2->CR1, 0);
}
#else
void TIM2_Delay(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one shot
	TIM2->CR1 = (0x1 << 4) | (0x1 << 3);
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (unsigned int)(TIMXCLK/TIM2_FREQ + 0.5) - 1;
	// ARR 초기값 설정 => 요청한 time msec에 해당하는 초기값 설정
	TIM2->ARR = TM2_1ms_Pls * time;

	// UG 이벤트 발생
	TIM2->EGR |= (0x1 << 0);

	// UIF(Update Interrupt Pending) Clear
	TIM2->SR &= ~(0x1 << 0);
	// TIM2 start
	TIM2->CR1 |= (0x1 << 0);
	// Wait timeout
	while(!Macro_Check_Bit_Set(TIM2->SR,0));

	// TIM2 Stop
	Macro_Clear_Bit(TIM2->CR1, 0);
}
#endif

void TIM4_Repeat(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);

	// TIM4 CR1: ARPE=0, down counter, repeat mode
	TIM4->CR1 = (0x1 << 4) & ~(0x1 << 3);
	// PSC(50KHz),  ARR(reload시 값) 설정
	TIM4->PSC = (unsigned int)(TIMXCLK/TIM4_FREQ + 0.5) - 1;
	TIM4->ARR = time * TM4_1ms_Pls;
	// UG 이벤트 발생
	TIM4->EGR |= (0x1 << 0);
	// Update Interrupt Pending Clear
	TIM4->SR &= ~(0x1 << 0);
	// TIM4 start
	TIM4->CR1 |= (0x1 << 0);
}

int TIM4_Check_Timeout(void)
{
	int rtn = 0;
	// 타이머가 timeout 이면 1 리턴, 아니면 0 리턴
	if(Macro_Check_Bit_Set(TIM4->SR,0))
	{
		rtn = 1;
		TIM4->SR &= ~(0x1 << 0);
	}

	return rtn;
}

void TIM4_Stop(void)
{
	Macro_Clear_Bit(TIM4->CR1, 0);
}

void TIM4_Change_Value(int time)
{
	TIM4->ARR = 50 * time;
}
