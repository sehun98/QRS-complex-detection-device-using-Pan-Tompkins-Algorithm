#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <util/delay.h>

#include "uart.h"

#define LED_ON (PORTA |= (1 << PA0))
#define LED_OFF (PORTA &= ~(1 << PA0))
#define LED_TOGGLE (PORTA ^= (1 << PA0))

void Timer_Init(void);
void GPIO_Init(void);

int main(void)
{
	GPIO_Init();
	Timer_Init();
	UART_Init(0,115200);
	
	sei();
	
    while (1) 
    {
		
    }
}

void GPIO_Init(void)
{
	DDRA |= (1 << PA0);      // PA0 출력
	LED_OFF;    // LED OFF
}

// 16000000 / 250 / 64 = 1000Hz 
void Timer_Init(void)
{
	// CTC 모드 (Clear Timer on Compare Match)
	TCCR0 = (1 << WGM01);       // CTC 모드
	TCCR0 |= (1 << CS01) | (1 << CS00); // Prescaler = 64

	OCR0 = 249; // 1ms 만들어주는 값

	TIMSK |= (1 << OCIE0); // Output Compare Match Interrupt Enable
}

ISR(TIMER0_COMP_vect)
{
	static uint16_t ms_count = 0;
	ms_count++;   // 1ms 증가
	
	if(ms_count >= 1000)   // 1000ms = 1초
	{
		LED_TOGGLE;   // LED toggle
		ms_count = 0;
	}
}