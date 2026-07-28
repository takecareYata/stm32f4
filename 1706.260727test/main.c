/*
보드의 KEY를 누르고 떼면 그 간격 시간을 UART로 인쇄하는 코드를 구현하라
타이머는 반드시 TIM3을 이용해야한다(다른 타이머 사용시 0점 처리)
시간은 100msec 단위(즉, 0.1초 단위로 측정하며 반올림하거나 버림하는 것은 무방하다)로 측정한다. (예를 들어 실제 시간이 3.45초 일 경우 인쇄 결과가 3.4 또는 3.5 어떤 값이든 다 상관없다)
최대 측정 가능 시간은 10초로 하며 그 이상 넘어갈 경우 측정 결과는 오류가 나거나 측정되거나 무방하다 (즉, 검사할 때 10초 이상 누르지는 않음)
로직아날라이져를 KEY를 스위치 눌린 사이 시간을 확인할 수 있도록 연결한다.
(주의) 사용하는 주변장치의 Clock의 ON이 필요할 수도 있음
*/

#include "device_driver.h"
#include <stdio.h>
#define TIM_MAX	0xffffu
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
	printf("TIM2 stopwatch test\n");

	Key_ISR_Enable(1);
	
	for(;;)
	{
		
	}
}
