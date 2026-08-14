#include "device_driver.h"

void Check_Blue()
{
    LED_On();
    Servo_Set90();
}

void Check_Red()
{
    LED_Off();
    Servo_Set0();
}

void Check_Stop()
{
    LED_Toggle();
    Servo_Set0();
}