#include "main.h"

void Delay(uint32_t time)
{
    while (time--)
    {
        volatile uint32_t count = 1000;
        while (count--) {}
    }
}

int main(void)
{
	Led_Init();
	while(1)
	{
		LED_ON();
		Delay(1000);
		LED_OFF();
		Delay(1000);
	}
}
