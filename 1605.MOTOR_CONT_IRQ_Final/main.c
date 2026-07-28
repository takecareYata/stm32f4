#include "device_driver.h"
#include <stdio.h>

static void Sys_Init(int baud)
{
	SCB->CPACR |= (0x3 << 10 * 2) | (0x3 << 11 * 2);
	Clock_Init();
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
}

#if 1
typedef enum {
    MOTOR_STOP ,
    MOTOR_FORWARD,
    MOTOR_REVERSE
} MotorState_t;

typedef enum {
    KEY_IDLE ,
    KEY_PRESSED,
    KEY_WAIT_RELEASE
} KeyState_t;

// 전역 변수
static MotorState_t motor_state = MOTOR_STOP;
static KeyState_t key_state = KEY_IDLE;
static volatile int long_pressed = 0;
static volatile int short_pressed = 0;

// UART2 속도 제어용 변수 (기본값: 55%)
static int current_speed_duty = 50;

// 함수 선언
static void OP_Handler(void);
static void Event_Handler(void);
static void Uart_Handler(void);

void Main(void)
{
    Sys_Init(115200);
    printf("\nDC MOTOR Control (Switch + UART IRQ Speed)\n");
    printf(" - Switch Short: Direction Toggle\n");
    printf(" - Switch Long : Stop\n");
    printf(" - UART '1'~'9': Speed Control\n");

    Uart2_RX_Interrupt_Enable(1);
    Motor_Init();
    Key_Poll_Init();

    for (;;)
    {
        Uart_Handler();   // UART2 속도 입력 처리
        OP_Handler();     // 스위치 기반 모터 FSM
        Event_Handler();  // 스위치 디바운싱 및 길게/짧게 누름 판별
    }
}

extern volatile int Uart_Data_In;

static void Uart_Handler(void)
{
    char rx_char = Uart2_Get_Pressed();

    if (rx_char == 0)
    {
        return;
    }
   
    if (rx_char >= '1' && rx_char <= '9' && (motor_state != MOTOR_STOP))
    {
        // [60%,65%,70%,75%,80%,85%,90%,95%,100%]
        current_speed_duty = (rx_char - '1') * 5 + 60;
        printf("current_speed_duty : %d\n",current_speed_duty);

        // 현재 모터가 구동 중이라면 실시간으로 PWM 속도 반영
        if (motor_state == MOTOR_FORWARD)
        {
            Motor_Forward(current_speed_duty);
        }
        else if (motor_state == MOTOR_REVERSE)
        {
            Motor_Reverse(current_speed_duty);
        }
    }
    else if (rx_char == 's' || rx_char == 'S')
    {   
        Motor_Stop();
        motor_state = MOTOR_STOP;
    }
    else if (rx_char == 'f' || rx_char == 'F')
    {
        Motor_Stop();
        TIM4_Delay(10);
        motor_state = MOTOR_FORWARD;
        Motor_Forward(100);
    }
    else if (rx_char == 'r' || rx_char == 'R')
    {
        Motor_Stop();
        TIM4_Delay(10);
        motor_state = MOTOR_REVERSE;
        Motor_Reverse(100);
    }

}

static void OP_Handler(void)
{
    if (long_pressed)
    {
        Motor_Stop();
        motor_state = MOTOR_STOP;

        long_pressed = 0; // flag clear
        return;
    }

    if (!short_pressed) // long, short pressed flag 둘 다 켜지지 않음
        return;

    switch (motor_state)
    {
    case MOTOR_STOP:
        // 설정된 속도(current_speed_duty)로 정회전
        Motor_Forward(current_speed_duty);
        motor_state = MOTOR_FORWARD;
        break;

    case MOTOR_FORWARD:
        Motor_Stop();
        TIM4_Delay(10);
        // 설정된 속도(current_speed_duty)로 역회전
        Motor_Reverse(current_speed_duty);
        motor_state = MOTOR_REVERSE;
        break;

    case MOTOR_REVERSE:
        Motor_Stop();
        TIM4_Delay(10);
        // 설정된 속도(current_speed_duty)로 정회전
        Motor_Forward(current_speed_duty);
        motor_state = MOTOR_FORWARD;
        break;

    default:
        Motor_Stop();
        motor_state = MOTOR_STOP;
        break;
    }

    short_pressed = 0;
}

static void Event_Handler(void)
{
    switch (key_state)
    {
    case KEY_IDLE:
        if (Key_Get_Pressed())
        {
            TIM2_Timer_Start();
            key_state = KEY_PRESSED;
        }
        break;

    case KEY_PRESSED:
        if (TIM2_Check_Timeout())
        {
            long_pressed = 1;
            printf("long pressed!!\n");
            key_state = KEY_WAIT_RELEASE;
        }
        else if (!Key_Is_Pressed())
        {
            TIM4_Delay(10);

            if (!Key_Is_Pressed())
            {
                TIM2_Timer_Stop();
                short_pressed = 1;
                printf("short pressed!!\n");
                key_state = KEY_IDLE;
            }
        }
        break;

    case KEY_WAIT_RELEASE:
        if (!Key_Is_Pressed())
        {
            TIM4_Delay(10);

            if (!Key_Is_Pressed())
                key_state = KEY_IDLE;
        }
        break;

    default:
        key_state = KEY_IDLE;
        break;
    }
}
#endif
