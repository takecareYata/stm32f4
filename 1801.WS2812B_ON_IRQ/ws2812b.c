#include "device_driver.h"

#define WS2812_FREQ         (800000UL)   // WS2812B 신호 주파수 (800kHz = 1.25us 주기)
#define WS2812_T0H_NS       (400UL)      // Bit 0 High 시간 (400ns = 0.40us)
#define WS2812_T1H_NS       (830UL)      // Bit 1 High 시간 (830ns = 0.83us)

// ARR = (TIMXCLK / FREQ) - 1
#define WS2812_ARR          (((TIMXCLK + (WS2812_FREQ / 2)) / WS2812_FREQ) - 1)

// 1us(1000ns)당 타이머 카운트 수 = TIMXCLK / 1,000,000
// CCR = (T_NS * (TIMXCLK / 1000) + 500,000) / 1,000,000 (반올림)
#define WS2812_T0H          ((uint16_t)(((WS2812_T0H_NS * (TIMXCLK / 1000UL)) + 500000UL) / 1000000UL))
#define WS2812_T1H          ((uint16_t)(((WS2812_T1H_NS * (TIMXCLK / 1000UL)) + 500000UL) / 1000000UL))

#define LED_COUNT     4    // 제어할 LED 개수
#define BIT_BUFF_SIZE (LED_COUNT * 24 + 50) // 24비트/LED + Reset 구간(50비트 0)

static uint16_t bit_buffer[BIT_BUFF_SIZE];
static volatile uint16_t tx_index = 0;
static volatile uint8_t is_transmitting = 0;

void TIM2_WS2812_Init(void)
{
    // 1. Clock Enable (GPIOA, TIM2)
    Macro_Set_Bit(RCC->AHB1ENR, 0); // GPIOA
    Macro_Set_Bit(RCC->APB1ENR, 0); // TIM2

    // 2. PA5 => AF01 (TIM2_CH1)
    Macro_Write_Block(GPIOA->MODER, 0x3, 0x2, 10);     // PA5 AF Mode
    Macro_Write_Block(GPIOA->AFR[0], 0xF, 0x1, 20);    // PA5 AF1 (TIM2)

    // 3. TIM2 레지스터 설정 (96MHz 기준)
    TIM2->PSC = 0;                       // Prescaler = 0
    TIM2->ARR = WS2812_ARR;              // 119 (800kHz)
    TIM2->CCR1 = 0;
    
    // PWM Mode 1 (OC1M = 110), Preload Enable (OC1PE = 1)
    Macro_Write_Block(TIM2->CCMR1, 0xFF, (6 << 4) | (1 << 3), 0);
    TIM2->CCER |= (1 << 0);              // CC1E = 1 (Output Enable)

    // 4. NVIC 인터럽트 설정
    NVIC_EnableIRQ(28);
}

void WS2812_Set_LED_Color(uint8_t led_index, uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t color = (green << 16) | (red << 8) | blue ;
    uint16_t buff_offset = led_index * 24;

    for (int i = 23; i >= 0; i--)
    {
        if ((color >> i) & 0x01) 
        {
            bit_buffer[buff_offset++] = WS2812_T1H;
        } 
        else 
        {
            bit_buffer[buff_offset++] = WS2812_T0H;
        }
    }
}

void WS2812_Clear_Buffer(void)
{
    for (int i = 0; i < BIT_BUFF_SIZE; i++) 
    {
        bit_buffer[i] = 0;
    }
}

void WS2812_Send(void)
{
    // 이전 전송 중이면 완료될 때까지 대기
    while (is_transmitting);

    tx_index = 0;
    is_transmitting = 1;

    // 첫 번째 비트 데이터 사전 로드
    TIM2->CCR1 = bit_buffer[tx_index++];

    // TIM2 카운터 리셋 및 Update Interrupt Enable (UIE = 1)
    TIM2->CNT = 0;
    TIM2->SR &= ~(1 << 0);              // UIF 클리어
    TIM2->DIER |= (1 << 0);              // Update Interrupt Enable
    TIM2->CR1 |= (1 << 0);               // Start Timer
}

void TIM2_IRQHandler(void)
{
    // Update Interrupt Flag 확인
    if (TIM2->SR & (1 << 0))
    {
        TIM2->SR &= ~(1 << 0); // UIF 인터럽트 플래그 클리어

        if (tx_index < BIT_BUFF_SIZE)
        {
            // 다음 비트의 High 유지 시간(CCR1) 로드
            TIM2->CCR1 = bit_buffer[tx_index++];
        }
        else
        {
            // 전송 완료 처리
            TIM2->DIER &= ~(1 << 0);     // Update Interrupt Disable
            TIM2->CR1 &= ~(1 << 0);      // Stop Timer
            TIM2->CCR1 = 0;              // 라인 Low 유지
            is_transmitting = 0;
        }
    }
}