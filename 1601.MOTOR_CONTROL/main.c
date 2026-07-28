#include "device_driver.h"
#include <stdio.h>

static void Sys_Init(int baud) 
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Clock_Init();
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
	MOTOR_Init();
	Key_Poll_Init();
}

void Main(void)
{
	Sys_Init(115200);
	printf("\nMotor Control Test\n");

	int mode = 2;

	int key_state = 0;
    int prev_key_state = 0;

	unsigned int time = 0;

	unsigned int cnt;
	unsigned int long_flag = 0;
	unsigned int short_flag = 0;
	
	for(;;)
	{

// 단점 : 3초 이후에도 바로 멈추지 않는다.
        Key_Wait_Key_Pressed(); // key입력 Falling 시작 대기
		//printf("Key Pressed! \r\n");
		TIM2_Stopwatch_Start(); 
		//LED_On();
		Key_Wait_Key_Released(); // rising 시점 체크
		//LED_Off();
		time = (unsigned int)(TIM2_Stopwatch_Stop());

		//printf("time = %u\r\n",time);

		if (time >= 3000000)
		{
			MOTOR_Stop();
			mode = 2;
		}
		else
		{
			mode = (mode + 1) % 2;
		}

		TIM2_Delay(1);

		//1. button 1 cw
		if(mode == 0)
		{
			MOTOR_Stop();
			MOTOR_Turn_cw();
		}
		//2. button 2 ccw
		else if(mode == 1)
		{
			MOTOR_Stop();
			MOTOR_Turn_ccw();
		}

#if 0
		// motor control
		key_state = Key_Get_Pressed();

        if (key_state == 1 && prev_key_state == 0)
        {
            mode = (mode + 1) % 2; 
            TIM2_Delay(1);
        }
		prev_key_state = key_state;

		TIM2_Delay(1);

		//1. button 2 cw
		if(mode == 0)
		{
			MOTOR_Stop();
			MOTOR_Turn_cw();
		}
		//2. button 3 ccw
		else if(mode == 1)
		{
			MOTOR_Stop();
			MOTOR_Turn_ccw();
		}
#endif
	}
}
