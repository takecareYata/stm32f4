#include "device_driver.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void MOTOR_Init(void)
{
    Macro_Set_Bit(RCC->AHB1ENR, 0);
    //PA0,PA1 init
    Macro_Write_Block(GPIOA->MODER, 0xf, 0x5, 0);
    Macro_Clear_Bit(GPIOA->OTYPER,0);
    Macro_Clear_Bit(GPIOA->OTYPER,1);

    // 초깃값 stop
	Macro_Clear_Bit(GPIOA->ODR, 0);
    Macro_Clear_Bit(GPIOA->ODR, 1); 
}

void MOTOR_Turn_cw(void)
{
    //PA0 High,PA1 Low
    Macro_Set_Bit(GPIOA->ODR, 0);
    Macro_Clear_Bit(GPIOA->ODR, 1);
}

void MOTOR_Turn_ccw(void)
{
    //PA0 Low,PA1 High
    Macro_Clear_Bit(GPIOA->ODR, 0);
    Macro_Set_Bit(GPIOA->ODR, 1);
}

void MOTOR_Stop(void)
{
    //PA0 Low,PA1 Low
    Macro_Clear_Bit(GPIOA->ODR, 0);
    Macro_Clear_Bit(GPIOA->ODR, 1); 
}