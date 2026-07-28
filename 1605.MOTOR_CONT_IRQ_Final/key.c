#include "device_driver.h"

void Key_Poll_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 2);
	Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 26);
	Macro_Write_Block(GPIOC->PUPDR, 0x3, 0x1, 26);
}

int Key_Is_Pressed(void)
{
	return Macro_Check_Bit_Clear(GPIOC->IDR, 13);
}
#if 0
int Key_Get_Pressed(void)
{
	return Macro_Check_Bit_Clear(GPIOC->IDR, 13);	
}
#endif

#if 1
int Key_Get_Pressed(void)
{
	static int prev = 0;
	int curr = Key_Is_Pressed();

	if (!prev && curr)
	{
		TIM4_Delay(10);

		if (Key_Is_Pressed())
		{
			prev = 1;
			return 1;
		}
	}

	if (!Key_Is_Pressed())
		prev = 0;

	return 0;
}
#endif

void Key_Wait_Key_Pressed(void)
{
	while (!Macro_Check_Bit_Clear(GPIOC->IDR, 13))
		;
}

void Key_Wait_Key_Released(void)
{
	while (!Macro_Check_Bit_Set(GPIOC->IDR, 13))
		;
}
