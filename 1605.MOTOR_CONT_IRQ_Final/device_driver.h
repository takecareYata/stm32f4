#include "stm32f4xx.h"
#include "option.h"
#include "macro.h"
#include "malloc.h"

// Uart.c

extern void Uart2_Init(int baud);
extern void Uart2_Send_Byte(char data);
extern char Uart2_Get_Pressed(void);
extern char Uart2_Get_Char(void);

extern void Uart1_Init(int baud);
extern void Uart1_Send_Byte(char data);
extern void Uart1_Send_String(char *pt);
extern char Uart1_Get_Char(void);
extern char Uart1_Get_Pressed(void);

// SysTick.c

extern void SysTick_Run(void);

// Led.c

extern void LED_Init(void);
extern void LED_On(void);
extern void LED_Off(void);

// Clock.c

extern void Clock_Init(void);

// Key.c

extern void Key_Poll_Init(void);
extern int Key_Is_Pressed(void);
extern int Key_Get_Pressed(void);
extern void Key_Wait_Key_Released(void);
extern void Key_Wait_Key_Pressed(void);

//uart.c
extern void Uart2_RX_Interrupt_Enable(int en);

// Timer.c
extern void TIM2_Timer_Start();
extern unsigned int TIM2_Timer_Stop();
extern int TIM2_Check_Timeout(void);
extern void TIM2_Delay(int time);
extern void TIM2_Stopwatch_Start(void);
extern unsigned int TIM2_Stopwatch_Stop(void);
extern void TIM4_Delay(int time);
extern void TIM4_Repeat(int time);
extern int TIM4_Check_Timeout(void);
extern void TIM4_Stop(void);
extern void TIM4_Change_Value(int time);
extern void TIM5_Out_Init(void);
extern void TIM5_Out_Freq_Generation(unsigned short freq);
extern void TIM5_Out_PWM_Generation(unsigned short freq, int duty);
extern void TIM5_Out_Stop(void);
extern void TIM5_CH2_Out_PWM_Generation(unsigned short freq, int duty);
extern void TIM5_CH2_Out_Stop(void);

// Motor.c
extern void Motor_Init(void);
extern void Motor_Stop(void);
extern void Motor_Forward(int duty);
extern void Motor_Reverse(int duty);

#if 0
extern void Motor_Init(void);
extern void Motor_Stop(void);
extern void Motor_Forward(void);
extern void Motor_Reverse(void);
#endif