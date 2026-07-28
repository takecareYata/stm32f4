#include "stm32f4xx.h"
#include "option.h"
#include "macro.h"
#include "malloc.h"

// Uart.c

extern void Uart2_Init(int baud);
extern void Uart2_Send_Byte(char data);
extern char Uart2_Get_Pressed(void);

extern void Uart1_Init(int baud);
extern void Uart1_Send_Byte(char data);
extern void Uart1_Send_String(char *pt);
extern void Uart1_Printf(char *fmt,...);
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
extern int Key_Get_Pressed(void);
extern void Key_Wait_Key_Released(void);
extern void Key_Wait_Key_Pressed(void);

// motor.c
extern void MOTOR_Init(void);
extern void MOTOR_Turn_cw(void);
extern void MOTOR_Turn_ccw(void);
extern void MOTOR_Stop(void);

// timer.c
extern void TIM2_Delay(int time);
extern void TIM2_Stopwatch_Start(void);
extern unsigned int TIM2_Stopwatch_Stop(void);
extern void TIM4_Repeat(int time);
extern int TIM4_Check_Timeout(void);
extern void TIM4_Stop(void);
extern void TIM4_Change_Value(int time);
extern void TIM3_Out_Init(void);
extern void TIM3_Out_Freq_Generation(unsigned short freq);
extern void TIM3_Out_Stop(void);
extern void TIM3_Out_PWM_Generation(unsigned short freq, int duty);
extern void TIM5_Out_Init(void);
extern void TIM5_Out_Freq_Generation(unsigned short freq);
extern void TIM5_Out_PWM_Generation(unsigned short freq, int duty);
extern void TIM5_Out_Stop(void);