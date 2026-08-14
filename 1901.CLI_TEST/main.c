#include "device_driver.h"
#include <stdio.h>

volatile int Key_Pressed = 0;
volatile int Uart_Data_In = 0;
volatile unsigned char Uart_Data = 0;
volatile int TIM4_Expired = 0;

static void Sys_Init(int baud) 
{
    SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
    Clock_Init();
    Uart2_Init(baud);
    setvbuf(stdout, NULL, _IONBF, 0); // printf 버퍼링 해제
    LED_Init();
    Servo_Init();
    RingBuf_Init();                   // 원형 큐 초기화
}

void Main(void) 
{
    static char line_buf[LINE_BUF_SIZE];
    static int line_idx = 0;
    char ch;

    Sys_Init(115200);

    printf("MCU> ");

    Key_ISR_Enable(1);
    Uart2_RX_Interrupt_Enable(1);
    TIM4_Repeat_Interrupt_Enable(1, 200);

    for(;;) 
    {
        // [1] ISR에서 전달받은 UART 수신 문자를 원형 큐에 입력
        if (Uart_Data_In) 
        {
            if (!RingBuf_Put((char)Uart_Data)) 
            {
                printf("\n[Error] Ring Buffer Overflow!\nMCU> ");
            }
            Uart_Data_In = 0;
        }

        // [2] 원형 큐에서 1바이트씩 꺼내어 문자열 조립 및 CLI 처리
        while (RingBuf_Get(&ch)) 
        {
            printf("%c", ch); // Echo-back

            if (ch == '\r' || ch == '\n') 
            {
                printf("\n");
                line_buf[line_idx] = '\0'; // 문자열 종단

                Process_CLI_Line(line_buf); // CLI 처리

                line_idx = 0;
                printf("MCU> ");
            } 
            else if (ch == '\b' || ch == 0x7F) // 백스페이스
            { 
                if (line_idx > 0) {
                    line_idx--;
                    printf(" \b");
                }
            } 
            else 
            {
                if (line_idx < LINE_BUF_SIZE - 1) 
                {
                    line_buf[line_idx++] = ch;
                }
            }
        }

        if (Key_Pressed) 
        {
            printf("\n[INT] KEY Pressed!!!\nMCU> ");
            led_toggle_flag = 1;
            Servo_SetAngle(0);
            Key_Pressed = 0;
        }

        if (TIM4_Expired) 
        {
			if(led_toggle_flag)
			{
				LED_Toggle();
				TIM4_Expired = 0;
			}
        }
    }
}