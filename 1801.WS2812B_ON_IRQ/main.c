#include "device_driver.h"
#include <stdio.h>
#include <string.h>

static void Sys_Init(int baud) 
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Clock_Init();
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
}

extern volatile int TIM4_Expired;
extern volatile int DMA1_STREAM6_DONE;
extern volatile int Key_Pressed;

void Start_Sign(void)
{
    WS2812_Clear_Buffer();

    WS2812_Set_LED_Color(0, 0, 0, 0);
    WS2812_Set_LED_Color(1, 0, 0, 0);
    WS2812_Set_LED_Color(2, 0, 0, 0);
    WS2812_Set_LED_Color(3, 0, 0, 0);
    WS2812_Send();
    TIM3_Delay_ms(1000);
    
    WS2812_Set_LED_Color(0, 255, 0, 0);
    WS2812_Send();
    TIM3_Delay_ms(1000);

    WS2812_Set_LED_Color(1, 255, 0, 0);
    WS2812_Send();
    TIM3_Delay_ms(1000);

    WS2812_Set_LED_Color(2, 255, 0, 0);
    WS2812_Send();
    TIM3_Delay_ms(1000);

    WS2812_Set_LED_Color(3, 255, 0, 0);
    WS2812_Send();
    TIM3_Delay_ms(1000);

    WS2812_Set_LED_Color(0, 0, 255, 0);
    WS2812_Set_LED_Color(1, 0, 255, 0);
    WS2812_Set_LED_Color(2, 0, 255, 0);
    WS2812_Set_LED_Color(3, 0, 255, 0);
    WS2812_Send();
    TIM3_Delay_ms(1000);
}

void Main(void)
{
    Sys_Init(115200);

    Key_ISR_Enable(1);
    TIM2_WS2812_Init();
    WS2812_Clear_Buffer();

    printf("WS2812B ON TEST\n\n");    

    for (;;)
    {
        if(Key_Pressed == 0)
        {
            WS2812_Set_LED_Color(0, 255, 255, 0);
            WS2812_Set_LED_Color(1, 255, 255, 0);
            WS2812_Set_LED_Color(2, 255, 255, 0);
            WS2812_Set_LED_Color(3, 255, 255, 0);
            WS2812_Send();
        }
        else if(Key_Pressed == 1)
		{
            Start_Sign();
		}
        else if (Key_Pressed == 2)
        {
            //??????    
        }
    }
}
