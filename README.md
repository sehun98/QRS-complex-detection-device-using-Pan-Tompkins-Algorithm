

심전도 측정 기기를 설계하기에 앞서, 인체의 신경 세포와 근육 세포가 전기적 신호를 생성하는 과정인 활동 전위(Action Potential)를 이해하는 것이 중요하다. 

이러한 활동 전위를 통해 발생하는 심전도 파형 중 하나인 QRS complex는 심장의 전기적 활동을 반영하며, 심부전, 심근경색, 심장비대와 같은 다양한 심장 질환을 진단하는 데 중요한 역할을 한다.

또한, 산소포화도(SPO2)와 함께 측정하면 심장 건강과 산소 공급 능력을 종합적으로 평가할 수 있어, 여러 질병을 조기에 발견하는 데 큰 도움이 된다.

## Active Potential

------

![action_potential.png](./4. images/action_potential.png)

1. Rest membrane potential : Extracellular fluid에 Na⁺ > Cl ⁻ > K ⁺  순으로 함유되어있다. 이때 extra에 있는 3개의 Sodium(Na⁺ )와 intra에 있는 2개의 Potassium(K ⁺ )이 Na⁺/K⁺ ATPase에 의해 교환이 이루어진다. 이를 -80mV의 음전하를 띄는 Rest membrane potential이라 한다.
2. Depolarization : 여러 자극이 존재하지만 근육에 붙어있는 Tendon에서 신호가 발생한 근 신호를 예를 들어 자극이 임계값(threshold)이상 발생하면 Na⁺  channel이 열리게 되어 Na⁺ 가 농도 차이에 의해 빠르게 유입되어 전위가 약 +30mV까지 상승하게 된다. 이를 Depolarization 이라 한다.
3. Repolarization : Depolarization 직후 Na⁺  channel이 닫히고 K⁺  channel이 열리게 되어 Potassium(K ⁺ )이 세포 밖으로 빠져나가 음전하로 떨어지게 된다.
4. After potential : Na⁺  channel이 원상태로 돌아갈때까지 K⁺  channel이 닫히지 않아 Potassium(K ⁺ )이 과도하게 세포 밖으로 빠져나가 과분극(Hyperpolarization) 이 발생한다. 이를 after potential이라고도 한다.

이러한 과정에 신호 전달 과정이 신경 세포에서 이루어진다.

## QRS파

------

### 심장의 전기신호 전달과정

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/fdb9b0c9-cc79-45e5-8023-74951f83bd69/image.png)

1. SA Node에서 시작된 전기 신호 : 심방을 따라 신호가 전달되어 심방을 수축시킨다. 이때 심방이 탈분극이 되지만 QRS파에는 나타나지 않고 QRS파는 심실의 탈분극을 나타내기 때문에, 심방 수축 시에는 P파가 나타난다.
2. AV Node에서 신호 지연 : 심방이 수축하고 심실로 혈액을 충분히 보낼 수 있도록 확보한다.
3. AV node를 통해 His bundle로 심실 수축 관여 : His bundle을 통해 전달된 신호는 심실 사이의 벽 수축을 일으켜 Q파가 발생한다.
4. Purkinje fibers를 통해 심실 전체 수축 : 가장 빠르고 강하게 수축하게 하는 Purkinje fibers를 통해 심실 전체가 수축하여 R파가 크게 발생하고 심실벽의 마지막 수축으로 S파가 발생한다.
5. 심실의 이완으로 T파가 발생한다.

P파 (P-wave) : 심방의 수축

**Q파 (Q-wave)** : 심실벽의 최초 수축 **R파 (R-wave)** : 심실 전체 수축 **S파 (S-wave)** : 심실벽의 마지막 수축

T파 (S-wave) :  심실의 이완

## Pan Tompkins Algorithm

------

BPM을 계산하기 위해 심전도(ECG) 신호에서 QRS 복합체를 검출하는 알고리즘

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/d9b89bff-ff69-40da-a7e4-59b26c55f44f/image.png)

### 실제 측정 데이터와 Pan Tompkins Algorithm 결과

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/d1ed92a4-bf5c-4777-aec6-0343b8a4e632/image.png)

[Atmega128 Floating 사용법](https://www.notion.so/Atmega128-Floating-140d1226d7b28065a8e5c61451371e59?pvs=21)

[Atmega128 모듈 사용법](https://www.notion.so/Atmega128-12fd1226d7b28092af94f107e7496a07?pvs=21)

[ECG_Atmega128A.zip](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/e1f39db7-1c80-4bf3-b7bf-5558c5ea8db1/ECG_Atmega128A.zip)

```c
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

#include "uart.h"
#include "ili9341.h"
#include "ili9341gfx.h"

#define MOVING_AVERAGE_SIZE 8
#define MOVING_AVERAGE_MASK (MOVING_AVERAGE_SIZE - 1)

#define ADC_VREF_TYPE 0x40
#define INITIAL_THRESHOLD 2000000

#define LCD_WIDTH 320
#define LCD_HEIGHT 240

#define TOTAL_SAMPLES (1024)
#define RESAMPLED_X_POINTS LCD_WIDTH

volatile bool ECG_Flag = false;

typedef struct ecg_t
{
	uint16_t raw_data;
	double lowPassFiltered_data;
	double differented_data;
	double squared_data;
	double moving_average_filtered_data;
	uint16_t bpm;
};

typedef struct moving_average_t 
{
	uint16_t index;
	double sum;
	double buffer[MOVING_AVERAGE_SIZE];
};

struct ecg_t ecg = { 0, 0, 0, 0, 0, 0 };
struct moving_average_t moving_average = { 0, 0, {0} };

void Timer_Init(void);
void ADC_Init(void);

uint16_t read_adc(unsigned char adc_input);
double lowPass_filter(double raw_data);
double differentiator(double lowPassFiltered_data);
double moving_average_filter(double squared_data);
uint16_t peak_detection(double filtered_data);

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
			ecg.lowPassFiltered_data = lowPass_filter((double)ecg.raw_data);
			ecg.differented_data = differentiator(ecg.lowPassFiltered_data);
			ecg.squared_data = (uint32_t) ecg.differented_data * (uint32_t) ecg.differented_data;
			ecg.moving_average_filtered_data = moving_average_filter(ecg.squared_data);
			ecg.bpm = peak_detection(ecg.moving_average_filtered_data);

			//UART_Printf(0, "%d %.2f\\n\\r", ecg.raw_data, ecg.lowPassFiltered_data);
			draw_graph_point(ecg.lowPassFiltered_data);

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
 * 16000000Hz/1024/62 = 252Hz, (256-194) = 62
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

/*
 * @Description : 
 * ADMUX(ADC Multiplexer Selection Register) : ADC 입력 채널 및 ref 전압을 설정하는 데 사용
 * ADC_VREF_TYPE : ADC의 ref 전압을 설정하는 값
 * ADCSRA(ADC Control and Status Register A) : ADC를 활성화 하고 ADC의 분주비를 결정
 *
 * ADMUX에서 상위 2비트는 ref 전압 관련 설정, 하위 6비트는 ADC 종류 관련 설정에 해당한다.
 * ADMUX 레지스터 값은 8b0100_0000로 
 * AVCC를 ref 전압으로 사용하고 Vref pin에 캐패시터를 외부에 설정하여 안정성을 높힐 수 있다.
 * ADC0를 사용한다. 즉, ADC0 핀에 ADC를 연결해야 된다.(회로도 참고)
 * 
 * ADC 활성화 및 프리스케일러를 16으로 설정
 * ADC 클럭은 16000000 / 16 = 1000000Hz가 된다.
 * 너무 빠르면 ADC 성능이 저하되고 너무 느리면 정확성이 떨어질 수 있다.
 *
 * @ADC0 feature
 * 1. 10bit resolution
 * 2. input range is 0V ~ AVCC
 * AVCC가 5V이므로 1bit당 4.88mV에 해당된다.
 */
void ADC_Init(void)
{
	ADMUX = ADC_VREF_TYPE & 0xff; // ADMUX = (1 << REFS0); Vref = AVCC, ADC0
	ADCSRA = 0x84; // ADCSRA = (1 << ADEN) | (1 << ADPS2); 
}

/*
 * @Description : 
 * @input : adc_input로 ADC0가 기본값이지만 값 설정에 따라 변경 가능
 * @output : adc 측정 데이터가 0~1023비트로 출력 1비트에 4.88mV에 해당
 * 10비트, 전압 [0V 5V] : ( 5V - 0V )/ 1024 = 0.0048828125V = 4.88mV
 *
 * ADCSRA 레지스터의 ADSC 비트를 활성화 하여 conversion을 진행한다.
 * conversion이 완료가 될 때 ADIF값이 1이 되므로 ADIF 값이 1이 들어올 때 까지 대기
 * 대기가 끝나면 값이 전달 된 것으로 ADIF 값을 1로 만들어 자동으로 0으로 초기화 되게 설정
 * 왜 1을 인가해서 초기화를 해야하는가?
 * 하드웨어가 특정 상태나 이벤트를 나타내기 위해 사용하는 비트로
 * 하드웨어가 이 비트를 1로 설정하면 소프트웨어가 비트를 확인하여 변환 완료를 감지하는 방식이다.
 * 이때 강제로 0을 인가하면 하드웨어와 소프트웨어의 미스 매칭이 이루어져 오류가 발생한다.
 * 따라서 소프트웨어에서 1을 인가하여 변환 완료가 되었음을 하드웨어에 전달하여 초기화를 진행한다.
 * 
 * ADCL과 ADCH의 값을 조합해 16비트 값으로 반환해야하나 ADCW를 사용하여 16비트로 자동으로 연산되게 할 수 있다.
 */
uint16_t read_adc(unsigned char adc_input)
{
	if(adc_input) 
	{ 
		ADMUX = adc_input | (ADC_VREF_TYPE & 0xff); // ADC0에서 변경 될 경우 동작
		_delay_us(10);
	}
	ADCSRA |= 0x40; // ADCSRA |= (1 << ADSC);
	while ((ADCSRA & 0x10) == 0); // ADIF 값이 0이면 대기
	ADCSRA |= 0x10; // reset
	return ADCW; // ADC 16bit data가 들어 있다.
}

// high pass filter, fir, window black man-harris, order 20, fs = 1041, fc = 0.05
double lowPass_filter(double raw_data)
{
	static double x[21];
	double filter_data = 0.0;
	
	double h[21] = {
		-0.000000005763680045252140562080444494966,
		-0.000000172065387586760051513178385561298,
		-0.000001054977970279373683220171080410221,
		-0.000003764719440903665501941838950061836,
		-0.000009895430705948864993047597859998632,
		-0.000020890482106453850657395987511222302,
		-0.000037069411837445175950458975400181316,
		-0.000056771692837215627402188550254535926,
		-0.000076256817198005712914524178991371173,
		-0.000090711299727415202411202888033159297,

		0.999903944283784151814131746505154296756,
		
		-0.000090711299727415229516257200170770147,
		-0.000076256817198005740019578491128982023,
		-0.000056771692837215647730979284357744064,
		-0.000037069411837445182726722553434584029,
		-0.000020890482106453867598054932597229083,
		-0.00000989543070594886668711349236859931 ,
		-0.000003764719440903670160623048848713701,
		-0.000001054977970279373894978407893985306,
		-0.000000172065387586755075194613266546806,
		-0.000000005763680045252140562080444494966
	};
	for(int i=20; i>=1;i--)
	{
		x[i] = x[i-1];
	}
	
	x[0] = raw_data;
	
	for(int i=0; i<21;i++)
	{
		filter_data += x[i] * h[i];
	}
	return filter_data;
}

/* 
 * @Description : 
 * 시간 축 delta값은 Sampling rate 1041Hz 이므로 delta = 1 / 1041 = 0.0009606147
 * 시간 축 delta값은 Sampling rate 1041Hz 이므로 delta = 1 / 252 = 0.0039682539
 * 기울기 = y축 변화율 / x축 변화율
 * 여기서 주의할 점은 데이터 타입이 변환된다는 점
 */
double differentiator(double lowPass_filtered_data)
{
	static double prev_data = 0.0;
	double delta = 0.0009606147934678194;
	// double delta = 0.00396825396825396825;

	double slope = (lowPass_filtered_data - prev_data) / delta;
	prev_data = lowPass_filtered_data;

	return slope;
}

/*
 * @Description : 
 * 현재 인덱스 위치의 데이터를 sum에서 빼서 이전 데이터의 영향을 제거한다.
 * 새로운 데이터를 sum에 추가한다.
 * 버퍼의 현재 인덱스 위치에 새로운 데이터를 저장한다.
 * 인덱스를 증가시켜 다음 데이터가 저장될 위치를 준비한다.
 * 비트 마스크를 사용하여 인덱스를 배열의 범위로 제한한다.
 * 현재까지의 데이터 합계를 배열의 크기로 나누어 평균을 계산하고 반환한다.
 */
double moving_average_filter(double squared_data)
{
	moving_average.sum -= moving_average.buffer[moving_average.index];
	moving_average.sum += squared_data;
	moving_average.buffer[moving_average.index] = squared_data;
	moving_average.index++;
	
	//if(moving_average.index >= MOVING_AVERAGE_SIZE) moving_average.index = 0;
	moving_average.index = moving_average.index & MOVING_AVERAGE_MASK;

	return moving_average.sum / MOVING_AVERAGE_SIZE;
}

/*
 * @Description :
 * 분당 bpm이 300을 넘으면 오류라 가정하여
 * 1초에 5번 피크를 감지한다고 하면
 * 1041Hz로 0.960ms * 208 = 199.68ms
 * delay가 208 이하일 경우 오류가 된다.
 * 
 * 여기서 THRESHOLD는 이동 평균으로 동적여야 한다.
 * 
 */
uint16_t peak_detection(double moving_average_filtered_data)
{
	uint16_t sampling_rate = 1041;
	static uint16_t heart_rate = 0;
	static uint16_t rr_interval = 0;
	static uint16_t delay = 0;
	static double threshold = INITIAL_THRESHOLD;

	if(moving_average_filtered_data > threshold && delay > 208) // 1041Hz 0.960ms 208개
	{
		heart_rate = 60 * sampling_rate / rr_interval; // 60 * 1041 / 925 = 67.5 bpm
		delay = 0;
		rr_interval = 0;
	}
	else
	{
		delay++;
		rr_interval++;
	}
	return heart_rate;
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
	ili9341_drawhline(0, 120, 320, WHITE);
	ili9341_drawvline(160, 0, 240, WHITE);

	for(int i = 1; i < 8; i++)
	{
		draw_line(0, i * 30, 320, i * 30, WHITE); // 점선 그리기
		draw_line(i * 40, 0, i * 40, 240, WHITE);
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
void draw_graph_point(uint16_t lowPassFilteredData)
{
	static int sample_count = 0;
	static int display_x = 0;
	static uint16_t prev_resampled_value = LCD_HEIGHT - 1;
	
	// x축 리샘플링: 1024개의 데이터를 320개의 x축으로 매핑
	// sample_count % 3 = sample_count % (1024 / 320)
	if (sample_count % (TOTAL_SAMPLES / RESAMPLED_X_POINTS) == 0)
	{
		uint16_t resampled_value = (uint16_t)(((double)lowPassFilteredData / 1024.0) * (LCD_HEIGHT - 1));
		
		// 데이터를 그리기
		draw_line(display_x - 1, prev_resampled_value, display_x,  resampled_value, YELLOW);
		prev_resampled_value = resampled_value;

		display_x++;
		if (display_x >= LCD_WIDTH)
		{
			ili9341_graphDisplay(); // 화면 초기화
			display_x = 0;
		}

		ili9341_setcursor(5,5);
		ili9341_settextcolour(WHITE, BLACK);
		ili9341_settextsize(2);
		printf("Value: %d", resampled_value); // 화면에 출력
	}
	sample_count++;
	if (sample_count >= RESAMPLED_X_POINTS)
	{
		sample_count = 0;
	}
}
```

![스크린샷 2024-11-15 170246.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/fd6b583f-1c08-4ac4-8c42-ccbdf04af81e/%EC%8A%A4%ED%81%AC%EB%A6%B0%EC%83%B7_2024-11-15_170246.png)

![스크린샷 2024-11-15 171609.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/2801e67c-07de-4d48-a276-9f042a059b97/%EC%8A%A4%ED%81%AC%EB%A6%B0%EC%83%B7_2024-11-15_171609.png)

[ECG_Serial.m](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/ba46b4e6-6684-4396-b4ba-d1cb884583c2/ECG_Serial.m)

```c
% UART 데이터 수신 및 실시간 플롯 (데이터 수신 속도 개선)
clear; clc;

%% 1. UART 포트 설정
portName = 'COM7';  % UART 포트 이름
baudRate = 115200;  % UART 통신 속도

% UART 포트 열기
serialObj = serialport(portName, baudRate);
configureTerminator(serialObj, 'LF'); % '\\n' 문자 기준 데이터 수신
serialObj.InputBufferSize = 8192; % 버퍼 크기 증가

disp('UART 포트가 열렸습니다. 데이터를 수신 중입니다...');

%% 2. 실시간 플롯 준비
figure;
grid on;
hold on;
title('UART 데이터 실시간 플롯 (첫 번째 숫자)');
xlabel('Time (s)');
ylabel('Value');
ylim([450 550]);  % Y축 데이터 범위를 400~500으로 설정
xlim([0 10]);     % 시간 범위 (필요시 수정)

% 데이터 저장용 변수
data = [];
timestamps = [];
startTime = tic;  % 시간 측정 시작

%% 3. 데이터 수신 및 플롯
while toc(startTime) < 10 % 10초 동안 데이터를 수신
    if serialObj.NumBytesAvailable > 0
        % UART 데이터 읽기
        rawData = readline(serialObj); % UART 데이터 한 줄 읽기
        disp(['Received Data: ', rawData]); % 수신된 데이터 확인
        
        % 1. 특수 문자 및 비숫자 제거
        cleanData = regexprep(rawData, '[^\\d\\s]', ''); % 숫자와 공백만 유지
        disp(['Cleaned Data: ', cleanData]); % 정리된 데이터 표시
        
        % 2. 공백 기준으로 데이터 분리
        numbers = split(strtrim(cleanData)); % 공백 제거 후 분리
        
        % 3. 첫 번째 숫자 추출
        if ~isempty(numbers) && ~isnan(str2double(numbers{1}))
            value = str2double(numbers{1}); % 첫 번째 숫자 추출
            disp(['Parsed Value: ', num2str(value)]); % 추출된 값 확인
            
            % 현재 시간 및 데이터 저장
            elapsedTime = toc(startTime);
            data = [data, value];
            timestamps = [timestamps, elapsedTime];
            
            % 플롯 업데이트 (drawnow를 최소화)
            if mod(length(data), 10) == 0 % 10번째 데이터마다 업데이트
                plot(timestamps, data, '-o', 'LineWidth', 1.5);
                drawnow limitrate; % 화면 업데이트 최적화
            end
        else
            disp('유효하지 않은 데이터입니다.'); % 유효하지 않은 데이터 디버깅
        end
    end
end

disp('데이터 수신이 완료되었습니다.');

%% 4. UART 포트 닫기
delete(serialObj);
clear serialObj;
disp('UART 포트가 닫혔습니다.');
```

# Signal Process in LabVIEW

------

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/4e7742c3-0bbf-4511-a1d6-483d26053f3e/image.png)

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/0bafe60f-3ae3-47b9-84d5-2ab0513249de/image.png)

# Objective

------

- 소프트웨어와 신호처리를 적용해보기 위한 QRS complex 감지 의료장비 설계

# Description

------

- 그동안의 프로젝트에서 학습한 기술을 활용한 심전도 프로젝트
- 노이즈 감소를 목표로 Right Leg Driver를 도입한 설계를 수행하였으며, Half-Cell Potential에 대한 경험
- Pan Tompkins Algorithm을 성공적으로 적용하여 데이터 처리와 분석에 활용

# Features

------

- The Atmel AVR 8-bit Microcontrollers Atmega128
- Instrument OP-AMP
- Right Leg Driver
- DC Offset Summing Amplifier
- 2 Gain Amplifier
- 5V to 3V3 Level Shifter
- TFT-LCD

# Circuits

------

![스크린샷 2024-11-10 215514.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/8173acca-56f8-4fad-9ef2-a8f9f5b314c2/%EC%8A%A4%ED%81%AC%EB%A6%B0%EC%83%B7_2024-11-10_215514.png)

## 1. MCU / +5V Power / -5V Power / +3V3 Power

![스크린샷 2024-11-10 215524.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/66a41c89-6493-4559-a330-e13222ea6015/%EC%8A%A4%ED%81%AC%EB%A6%B0%EC%83%B7_2024-11-10_215524.png)

### 1.1 MCU (ATmega128) :

- Description : 8-bit AVR microcontroller
- Operating Voltage : 3.3V
- Communication : SPI / 10 bit resolution ADC

### 1.2 +5V Power / +3V3 Power :

- Description : 리니어 레귤레이터 LM7805 / LM3940

### 1.3 -5V Power :

- Description : 스위칭 레귤레이터  ICL7660

## 2. Electrocardiogram

![스크린샷 2024-11-10 215533.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/c1a6871c-bf9b-4630-9c81-4897c288c2e0/%EC%8A%A4%ED%81%AC%EB%A6%B0%EC%83%B7_2024-11-10_215533.png)

### 2.1 Electrocardiogram :

- Description : Non-invert Amp + Differential Amp + Driven Right Leg + Active 1-pole band pass filter + Summing Amp
- Communication : 10 bit resolution ADC

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/0d4bd5fe-8a0c-4fb4-b1eb-b0fe4f16ae59/image.png)

- Non-invert Amp : Differential Amp만 사용하게 될 경우 Lead 저항과 합성이 되어 impedance isolation을 진행해야 한다.

  $$ \: A_V = 1 + \frac{R_1}{R_{\text{gain}/2}} = 1 + \frac{2R_1}{R_{\text{gain}}} $$

  $$ \: CMRR \approx 1 $$

- Differential Amp : CMRR이 좋은 소신호 증폭기

  $$ V_{\text{out}} = V_1 \times \left( 1 + \frac{2R_1}{R_{\text{gain}}} \right) \left( - \frac{R_3}{R_2} \right) $$

  $$ \: CMRR \approx \infty

  $$

- Driven Right Leg : 작은 전류로 CMRR을 feedback 시켜 CMRR을 효과적으로 제거하기 위해 사용되고 R4를 통해 인체에 흘러가는 전류량을 제한한다.

  $$ \: A_V = \frac{1}{2} \left( - \frac{R_3}{R_1} \right) = -100

  $$

  $$ \: V_{\text{out}} = 2 \times \frac{1}{2} \left( V_{\text{in}} + 1.24V_{\text{DC}} \right) $$

- Active 1-pole band pass filter : band pass filter를 통해 신호를 필터한다.

  $$ \: f_{CH} = \frac{1}{2\pi C_1 R_1} \approx 0.048\text{Hz}, \quad f_{CL} = \frac{1}{2\pi R_4 C_2} \approx 106.1\text{Hz}

  $$

  $$ \: A_V = 1 + \frac{R_4}{R_3} = 4.19

  $$

  $$ \: R_2 = R_1 = 3.3\text{Mohm} \quad \text{(impedance matching)}

  $$

- Summing Amp : MCU에서 인식할 수 있는 전압 범위를 위해 offset을 진행하며 증폭을 진행 시킨다.

  $$ V_{\text{out}} = 2 \times \frac{1}{2} \left( V_{\text{in}} + 1.24 V_{\text{DC}} \right) $$

## 3. 5V to 3V3 Level Shifter / TFT LCD / LEAD Connector

![스크린샷 2024-11-10 215545.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/80a8d157-27f7-457c-864f-a5867d489a0b/%EC%8A%A4%ED%81%AC%EB%A6%B0%EC%83%B7_2024-11-10_215545.png)

### 3.1 Level Shifter 5V to 3V3 :

- Description : 8-bit AVR microcontroller는 GPIO 전압이 5V이므로 LCD를 구동하기 위해서는 3V3 변환이 필요
- Resistor (R23) 10kΩ : Pull-up 시 A to B Shift 진행

### 3.2 TFT LCD :

- Description : SZH-EK096 TFT LCD
- Operating Voltage : 3.3V
- Communication : SPI

### 3.3 LEAD Connector :

- Description : 심전도 3 Lead Connector RA : 오른팔 LA : 왼팔 LL : 오른다리

### 주요 심전도 Lead

- **1-리드**: 웨어러블 기기, 간단한 심장 리듬 감지
- **3-리드**: 응급 상황에서 빠른 리듬 체크
- **5-리드**: 병원 모니터링, 실시간 심장 리듬 확인
- **6-리드**: 간이 심전도, 기본적인 심장 상태 확인
- **12-리드**: 표준 진단용, 심장의 다양한 부위를 평가 가능

# Part Layout

------

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/4c0f8206-5be8-4f73-a8f1-8cf785c6080f/image.png)

# Top Layer

------

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/b6bede03-f119-418a-9292-962c65e78ca3/image.png)

# Bottom Layer

------

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/808b1eea-3e37-4c74-93df-05e04986074e/image.png)

# 프로젝트 진행하면서 어려웠던 점

------

[문제점]

Common Mode Rejection Ratio와 Right Leg Driver를 활용한 노이즈 저감 이론

[해결방안]

의용계측 수업 및 의용계측공학 서적을 통해 CMRR 이론 학습

[문제점]

1코어 동작 시 UART 전송 동안 Heart Rate Sampling이 늦어지는 문제

[해결방안]

UART Baud Rate를 높히고, ECG 측정 후 임시 버퍼에 저장하여 데이터 유실을 줄임

# 프로젝트 진행하면서 깨달은 점

------

[깨달은점]

Half-Cell Potential에 의한 R-R interval 시 Threshold를 동적으로 반응 해야함