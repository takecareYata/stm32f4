#include "device_driver.h"

void Key_Poll_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 2); 
	Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 26);
}

int Key_Get_Pressed(void)
{
	// Key가 눌렸으면 1, 안 눌렸으면 0 리턴
#if	0
	int rtn = 0;
	static int flag;

	if((flag == 0)&&Macro_Check_Bit_Set(GPIOC->IDR,13))
	{
		rtn = 0;
		flag = 1;
	}
	else if((flag == 1)&&Macro_Check_Bit_Clear(GPIOC->IDR,13))
	{
		rtn = 1;
		flag = 0;
	}

	return rtn;

#else
	return Macro_Check_Bit_Clear(GPIOC->IDR, 13);	
#endif
}

void Key_Wait_Key_Pressed(void)
{
	// 키가 눌릴때까지 대기하고 눌리면 리턴
	while(!(Macro_Check_Bit_Clear(GPIOC->IDR,13)))
	{
		;
	}
}

void Key_Wait_Key_Released(void)
{
	// 키가 안 눌릴때까지 대기하고 안 눌리면 리턴
	while(!(Macro_Check_Bit_Set(GPIOC->IDR,13)))
	{
		;
	}
}
