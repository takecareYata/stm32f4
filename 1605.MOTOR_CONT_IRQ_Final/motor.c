#if 1
#include "device_driver.h"
#include <stdio.h>

#define TIM5_FREQ                   (1000000)            
#define PWM_FREQ                    (10000)              // 10kHz (모터 소음 최소화 주파수)

static int current_speed_duty = 0; // 현재 설정된 속도 (0 ~ 100%)

void Motor_Init(void)
{
    // GPIOA (AHB1 bit 0) & TIM5 (APB1 bit 3) 클럭 활성화
    Macro_Set_Bit(RCC->AHB1ENR, 0);
    Macro_Set_Bit(RCC->APB1ENR, 3);

    // PA0, PA1 Push-Pull 설정
    Macro_Write_Block(GPIOA->OTYPER, 0x3, 0x0, 0);

    // AF2 (TIM5) 매핑 미리 설정 (PA0 -> CH1, PA1 -> CH2)
    Macro_Write_Block(GPIOA->AFR[0], 0xF, 0x2, 0);  // PA0 = AF2
    Macro_Write_Block(GPIOA->AFR[0], 0xF, 0x2, 4);  // PA1 = AF2

    // TIM5 CH1 & CH2 PWM Mode 1 설정
    Macro_Write_Block(TIM5->CCMR1, 0xFF, 0x60, 0);  // CH1 PWM Mode 1
    Macro_Write_Block(TIM5->CCMR1, 0xFF, 0x60, 8);  // CH2 PWM Mode 1

    // CC1E, CC2E Enable
    TIM5->CCER |= (1 << 0) | (1 << 4);

    // 타이머 기본 주파수 설정 (PSC, ARR)
    TIM5->PSC = (unsigned int)((double)TIMXCLK / TIM5_FREQ + 0.5) - 1;
    TIM5->ARR = (unsigned int)((double)TIM5_FREQ / PWM_FREQ  + 0.5) - 1;; // ARR을 99로 두면 CCR값(0~100)이 곧 Duty %가 되어 계산이 편리
    
    // UG bit 발생으로 레지스터 업데이트
    Macro_Set_Bit(TIM5->EGR, 0);

    // Timer Start
    TIM5->CR1 = (1 << 4) | (1 << 0); // Down-counter, Counter Enable
    
    // 초기 상태: 정지
    Motor_Stop();
}

void Motor_Stop(void)
{
    // GPIO Mode -> (PA0: bit 1:0 = 01, PA1: bit 3:2 = 01)
    Macro_Write_Block(GPIOA->MODER, 0xF, 0x5, 0);

    // PA0, PA1 0V 출력
    Macro_Clear_Bit(GPIOA->ODR, 0);
    Macro_Clear_Bit(GPIOA->ODR, 1);

    TIM5->CCR1 = 0;
    TIM5->CCR2 = 0;
}

void Motor_Forward(int duty)
{
    // 1. PA0 -> General Output (0V 고정), PA1 -> AF Mode (TIM5 PWM)
    // MODER: PA0 = 0x1 (Output), PA1 = 0x2 (AF) => Binary: 1001 (0x9)
    Macro_Write_Block(GPIOA->MODER, 0xF, 0x9, 0);

    // 2. PA0 핀 0V 출력
    Macro_Clear_Bit(GPIOA->ODR, 0);

    // 3. PA1(CH2) PWM Duty 설정 (PA0(CH1)은 0)
    TIM5->CCR1 = 0;
    TIM5->CCR2 = (int)((TIM5->ARR + 1) * (duty / 100.));

    // UG bit 발생으로 레지스터 업데이트
    Macro_Set_Bit(TIM5->EGR, 0);
    //printf("Duty : %d \%\n", duty);
}

void Motor_Reverse(int duty)
{
    // 1. PA0 -> AF Mode (TIM5 PWM), PA1 -> General Output (0V 고정)
    // MODER: PA0 = 0x2 (AF), PA1 = 0x1 (Output) => Binary: 0110 (0x6)
    Macro_Write_Block(GPIOA->MODER, 0xF, 0x6, 0);

    // 2. PA1 핀 0V 출력
    Macro_Clear_Bit(GPIOA->ODR, 1);

    // 3. PA0(CH1) PWM Duty 설정 (PA1(CH2)는 0)
    TIM5->CCR1 = (int)((TIM5->ARR + 1) * (duty / 100.));
    TIM5->CCR2 = 0;

    // UG bit 발생으로 레지스터 업데이트
    Macro_Set_Bit(TIM5->EGR, 0);
}
#endif