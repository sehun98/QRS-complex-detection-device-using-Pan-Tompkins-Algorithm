/* 
 * @Device : Atmega128
 * @Autor : oh sehun
 * @Title : A Pan Tompkins Algorithm based ECG QRS Detector
 * @Overview of the Pan Tompkins Algorithm : 
 * 1. Bandpass filtering
 * 2. Differentiation
 * 3. Squaring
 * 4. Moving average filter
 * 5. Detection of QRS Complex
 */
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>

#include "uart.h"
#include "ili9341.h"
#include "ili9341gfx.h"
#include "ecg.h"

#define LCD_WIDTH 320
#define LCD_HEIGHT 240

#define RESAMPLED_X_POINTS LCD_WIDTH

void Timer_Init(void);
void ADC_Init(void);

void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ili9341_start(void);
void ili9341_graphDisplay(void);
void draw_graph_point(uint16_t lowPassFilteredData);

static FILE mydata = FDEV_SETUP_STREAM(ili9341_putchar_printf, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	stdout = &mydata;
		
	Timer_Init();
	ADC_Init();
	UART_Init(0,115200);
	
	ili9341_init();
	ili9341_clear(BLACK);
		
	ili9341_setRotation(1);
	ili9341_start();
	_delay_ms(1000);
	ili9341_graphDisplay();
	
	sei();

	while (1)
	{
		if(ECG_Flag)
		{
			ecg.raw_data = read_adc(0);
			
			ecg.highPassFiltered_data = highPass_filter((double)ecg.raw_data);
			ecg.differented_data = differentiator(ecg.highPassFiltered_data);
			ecg.squared_data = (uint32_t) ecg.differented_data * (uint32_t) ecg.differented_data;
			ecg.moving_average_filtered_data = moving_average_filter(ecg.squared_data);
			ecg.sub_sample_data = sub_sampling(ecg.highPassFiltered_data);
			
			ecg.bpm = peak_detection(ecg.moving_average_filtered_data);
			
			UART_Printf(0, "%d %d\n\r", ecg.raw_data, ecg.sub_sample_data);
			//draw_graph_point(ecg.sub_sample_data);

			ECG_Flag = false;
		}
	}
}

/*
 * @Description : 
 * TIMSK(timer/interrupt mask register) : 타이머/카운터의 인터럽트 요청을 활성화 하거나 비활성화하는데 사용
 * TCCR0(timer/interrupt0 controll register) : 타이머의 동작 모드, 프리스케일러 값 등을 설정
 * TCNT0(timer/counter0 register) : 현재 카운트 값을 저장
 * 
 * TIMSK 레지스터의 TOIE0 비트를 1로 활성화 하여 타이머를 사용하겠다고 선언하고
 * TCCR0 레지스터에서 일반모드 설정과 프리스케일을 1024로 분주비를 설정하여 16000000/1024 = 15625Hz로 타이머 클럭을 인가한다.
 * TCNT0 레지스터 값을 241로 설정하여 241부터 256까지 15번 타이머가 동작하고 오버플로우 되는 동작을 한다.
 * 오버플로우가 동작하면 ISR(TIMER0_OVF_vect)동작을 한다.
 * ISR(TIMER0_OVF_vect)이 끝나기 전에 TCNT0를 241로 초기화 시켜줘야 정상적으로 15번 타이머가 동작한다.
 *
 * ECG 신호는 초당 2Hz 정도로 낮은 주파수에 해당하지만 
 * Nyquist frequency에 의해 샘플링 레이트는 최대 주파수의 2배 이상을 해야 하지만 
 * 일반적인 생체신호 측정시 샘플링 레이트를 1000Hz로 설정하므로 위와 같은 설정을 진행하였다. 
 * 16000000Hz/1024/31 = 504Hz, (256-225) = 31
 * 16000000Hz/1024/15 = 1041Hz, (256-241) = 15
 */
void Timer_Init(void)
{
	TIMSK = 0x01;    // TOIE0 = 1;
	TCCR0 = 0x07;    // 일반모드, 프리스케일 = CK/1024
	TCNT0 = 241;    // 타이머/카운터0 레지스터 초기값
}

/* 
 * @Description: 
 * when timer0 overflows, the ISR is triggered.
 * 
 * @Warning!!! :
 * You need to initialize TCNT0 before the ISR ends.
 */
ISR(TIMER0_OVF_vect)
{
	ECG_Flag = true;
	TCNT0 = 241;
}


void ili9341_start(void)
{
	ili9341_setcursor(40,40);
	ili9341_settextcolour(YELLOW, BLACK);
	ili9341_settextsize(3);
	printf("QRS complex");
	ili9341_settextsize(2);
	ili9341_setcursor(40,90);
	printf("Oh. sehun");
}

void ili9341_graphDisplay(void)
{
	ili9341_clear(BLACK);
	draw_line(5, 0, 5,235, WHITE);
	draw_line(5, 235, 320,235, WHITE);

	for(int i = 1; i < 8; i++)
	{
		draw_line(0, i * 30, 10, i * 30, WHITE); // 점선 그리기
		draw_line(i * 40, 230, i * 40, 240, WHITE);
	}
	ili9341_setcursor(90,5);
}

void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t dx, dy, steps, i;
	float x_inc, y_inc, x, y;

	dx = x2 - x1;
	dy = y2 - y1;

	// 두 점 사이의 거리 계산
	if (abs(dx) > abs(dy)) {
		steps = abs(dx);
		} else {
		steps = abs(dy);
	}

	// 한 점에서 다음 점으로 그리는 라인의 간격 계산
	x_inc = dx / (float)steps;
	y_inc = dy / (float)steps;

	x = x1;
	y = y1;

	// 라인 그리기
	for (i = 0; i < steps; i++) {
		ili9341_drawpixel((uint16_t)x, (uint16_t)y, color);
		x += x_inc;
		y += y_inc;
	}
}

/*
 * @Description :
 * 이전 점과 현재 점을 연결해 노란색 선으로 그래프를 그린다.
 * X축이 끝나면(320개의 데이터를 나열) 화면을 초기화합니다.
 * 이때, 1041Hz의 샘플링 레이트로 리샘플링을 진행한 데이터 값을 화면에 텍스트로 표시한다.
 */
void draw_graph_point(uint16_t scaledValue)
{
	static uint16_t x = 0;
	static uint16_t prevValue = LCD_HEIGHT / 2;  // 초기 중간값

	// 화면 범위 보호
	if (scaledValue >= LCD_HEIGHT)
	scaledValue = LCD_HEIGHT - 1;

	// 이전 점 지우기 (선택사항 - 덮어그리기 방식 원하면 제거)
	ili9341_drawpixel(x, prevValue, BLACK);

	// 새로운 점 그리기
	ili9341_drawpixel(x, scaledValue, YELLOW);

	prevValue = scaledValue;

	// X좌표 증가
	x++;
	if (x >= LCD_WIDTH)
	{
		x = 0;
		// 화면 전체 지우지 말고 첫번째 열만 지우고 다시 시작하면 끊김 없음
		for (uint16_t y = 0; y < LCD_HEIGHT; y++)
		ili9341_drawpixel(0, y, BLACK);
	}
}
