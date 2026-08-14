#include "stm32f4xx.h"
#include "option.h"
#include "macro.h"
#include "malloc.h"

// Uart.c

extern void Uart2_Init(int baud);
extern void Uart2_Send_Byte(char data);
extern void Uart2_RX_Interrupt_Enable(int en);

extern void Uart1_Init(int baud);
extern void Uart1_Send_Byte(char data);
extern void Uart1_Send_String(char *pt);
extern void Uart1_Printf(char *fmt,...);
extern char Uart1_Get_Char(void);
extern char Uart1_Get_Pressed(void);

// SysTick.c

extern void SysTick_Run(unsigned int msec);
extern int SysTick_Check_Timeout(void);
extern unsigned int SysTick_Get_Time(void);
extern unsigned int SysTick_Get_Load_Time(void);
extern void SysTick_Stop(void);

// Led.c

extern void LED_Init(void);
extern void LED_On(void);
extern void LED_Off(void);
extern void LED_Toggle(void);
extern int led_toggle_flag;
// Clock.c

extern void Clock_Init(void);

// Key.c

extern void Key_Poll_Init(void);
extern int Key_Get_Pressed(void);
extern void Key_Wait_Key_Released(void);
extern void Key_Wait_Key_Pressed(void);
extern void Key_ISR_Enable(int en);

// Timer.c

extern void TIM2_Delay(int time);
extern void TIM2_Stopwatch_Start(void);
extern unsigned int TIM2_Stopwatch_Stop(void);
extern void TIM4_Repeat(int time);
extern int TIM4_Check_Timeout(void);
extern void TIM4_Stop(void);
extern void TIM4_Change_Value(int time);
extern void TIM4_Repeat_Interrupt_Enable(int en, int time);
extern void TIM3_Out_Init(void);
extern void TIM3_Out_Freq_Generation(unsigned short freq);
extern void TIM3_Out_Stop(void);

// que.c

#define RING_BUF_SIZE 128 // 원형 큐 크기

extern void RingBuf_Init(void);
extern int RingBuf_IsFull(void);
extern int RingBuf_IsEmpty(void);
extern int RingBuf_Put(char c);
extern int RingBuf_Get(char *c);

// cli.c
#define LINE_BUF_SIZE 64  // 명령어 1줄 버퍼 크기
#define MAX_ARGC      8   // 인자 최대 개수

extern void Process_CLI_Line(char *line);

// servo.c
extern void Servo_Init(void);
extern void Servo_SetAngle(int angle);
extern void Servo_Set90(void);
extern void Servo_Set0(void);

//color_check.c
extern void Check_Blue(void);
extern void Check_Red(void);
extern void Check_Stop(void);