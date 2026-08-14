#include "device_driver.h"

// 서보모터 초기화 (GPIOC PC6 -> TIM3_CH1 AF2 매핑)
void Servo_Init(void)
{
    // 1. GPIOC (Bit 2) 및 TIM3 (Bit 1) 클록 활성화
    RCC->AHB1ENR |= (1 << 2);  // GPIOC Enable
    RCC->APB1ENR |= (1 << 1);  // TIM3 Enable

    // 2. PC6 핀을 Alternate Function(AF2 - TIM3) 모드로 설정
    GPIOC->MODER &= ~(3 << (6 * 2));
    GPIOC->MODER |=  (2 << (6 * 2)); // Alternate Function Mode
    
    // PC6 핀은 AFRL(AFR[0])의 6번 비트 필드에 위치
    GPIOC->AFR[0] &= ~(0xF << (6 * 4));
    GPIOC->AFR[0] |=  (2 << (6 * 4));  // AF2 (TIM3_CH1)

    // 3. TIM3 PWM 설정 (50Hz = 20ms 주기)
    // APB1 Timer Clock = 84MHz 기준
    TIM3->PSC = 84 - 1;          // 1MHz 카운터 클록 (1us 틱)
    TIM3->ARR = 20000 - 1;       // 20,000us = 20ms (50Hz 주기)

    // 4. TIM3 Channel 1 PWM Mode 1 설정
    TIM3->CCMR1 &= ~(0x7 << 4);
    TIM3->CCMR1 |=  (6 << 4);    // PWM Mode 1 (CNT < CCR1 일 때 High)
    TIM3->CCMR1 |=  (1 << 3);    // Preload Enable

    TIM3->CCER |= (1 << 0);      // CH1 Output Enable
    TIM3->CR1  |= (1 << 7);      // ARPE Enable
    TIM3->CR1  |= (1 << 0);      // TIM3 Counter Enable

    // 초기 각도 0도로 설정
    Servo_SetAngle(0);
}

// 서보모터 각도 제어 함수 (0도 ~ 180도)
void Servo_SetAngle(int angle)
{
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    // SG90 서보모터 기준: 0도(500us) ~ 180도(2500us)
    // CCR1 값 = 500 + (angle * 2000 / 180)
    uint32_t ccr_val = 500 + ((uint32_t)angle * 2000 / 180);
    
    TIM3->CCR1 = ccr_val;
}

void Servo_Set0(void)
{
    Servo_SetAngle(0);
}

void Servo_Set90(void)
{
    Servo_SetAngle(90);
}