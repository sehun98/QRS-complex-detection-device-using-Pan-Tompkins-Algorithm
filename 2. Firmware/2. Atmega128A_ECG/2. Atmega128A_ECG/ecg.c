#include "ecg.h"

ecg_t ecg = { 0, 0, 0, 0, 0, 0 };
moving_average_t moving_average = { 0, 0, {0} };

volatile bool ECG_Flag = false;

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
double highPass_filter(double raw_data)
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
 * 기울기 = y축 변화율 / x축 변화율
 * 여기서 주의할 점은 데이터 타입이 변환된다는 점
 */
double differentiator(double highPass_filtered_data)
{
	static double prev_data = 0.0;
	double delta = 0.0009606147934678194;

	double slope = (highPass_filtered_data - prev_data) / delta;
	prev_data = highPass_filtered_data;

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

uint16_t sub_sampling(double moving_average_data)
{
	static uint16_t count = 0;
	static uint16_t subSampling_data = 0;
	

	const uint8_t factor = 5;   // ← 다운샘플링 배수 (1041/5 ≈ 208Hz)

	if (++count >= factor)
	{
		count = 0;
		subSampling_data = (uint16_t)moving_average_data;  // 새로운 값 갱신
	}

	return subSampling_data;   // 매번 반환은 하되, factor 주기마다 업데이트됨
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