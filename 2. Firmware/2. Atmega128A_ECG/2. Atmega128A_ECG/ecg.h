#ifndef ECG_H_
#define ECG_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define MOVING_AVERAGE_SIZE 8
#define MOVING_AVERAGE_MASK (MOVING_AVERAGE_SIZE - 1)

#define ADC_VREF_TYPE 0x40
#define INITIAL_THRESHOLD 2000000

#define TOTAL_SAMPLES 1024

typedef struct
{
	uint16_t raw_data;
	double highPassFiltered_data;
	double differented_data;
	double squared_data;
	double moving_average_filtered_data;
	uint16_t sub_sample_data;
	uint16_t bpm;
}ecg_t;

typedef struct
{
	uint16_t index;
	double sum;
	double buffer[MOVING_AVERAGE_SIZE];
}moving_average_t;

extern volatile bool ECG_Flag;
extern ecg_t ecg;
extern moving_average_t moving_average;

uint16_t read_adc(unsigned char adc_input);
double highPass_filter(double raw_data);
double differentiator(double lowPassFiltered_data);
double moving_average_filter(double squared_data);
uint16_t peak_detection(double filtered_data);
uint16_t sub_sampling(double moving_average_data);

#endif /* ECG_H_ */