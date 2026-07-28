#if 0
// 여기에 사용자 임의의 define을 작성하시오
#define GPIOA_MODER		(*(volatile unsigned long *)0x40020000)
#define GPIOA_OTYPER	(*(volatile unsigned long *)0x40020004)
#define GPIOA_ODR		(*(volatile unsigned long *)0x40020014)

void Main(void)
{
	// LED GPA[5]를 출력(General Push Pull) 모드로 설정하시오

	GPIOA_MODER = 0x400;
	GPIOA_OTYPER = 0x0;

	// GPA[5] LED를 ON 시키도록 설정하시오

	GPIOA_ODR = 0x20; // 0x00000020 ==> 0b00~~~010000 (5번 bit High)
}

#else // 과제. 외부 LED 연결하고 ON-OFF 제어하기

#define GPIOA_MODER   (*(volatile unsigned long *)0x40020000) // GPIOA Mode Register
#define GPIOA_OTYPER  (*(volatile unsigned long *)(GPIOA_MODER + 0x4)) // GPIOA Output Type Register
#define GPIOA_ODR    (*(volatile unsigned long *)(GPIOA_MODER + 0x14)) //GPIOA Output Data Register
#define PIN_7   7

int Main(void)
{

    // 1. PA7 핀을 Output 모드로 설정 (14, 15번 비트 = 0b01)
    GPIOA_MODER &= ~(3 << (PIN_7 * 2)); // 두개 비트 동시 초기화
    GPIOA_MODER |= (1 << (PIN_7 * 2)); // 14번 비트에 1 대입 Output)

    // 2. Output Type을 Open-Drain으로 설정 (7번 비트 = 0b01)
    GPIOA_OTYPER |= (1 << PIN_7);      // 7번 비트 1 (Open-Drain)

    // 3. ODR 레지스터의 7번 비트에 0을 써서 PA7을 LOW(0V)로 만듦 -> LED ON
    GPIOA_ODR &= ~(1 << PIN_7);

}
#endif