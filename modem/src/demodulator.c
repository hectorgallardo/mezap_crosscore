/*
 * demodulator.c
 *
 *  Created on: 2016 abe. 10
 *      Author: ibair
 */
#include <filter.h>
#include "modem.h"
#include "demodulator.h";
#include <filter.h>
#include <fract2float_conv.h>

float recieved_signal[NUM_SAMPLES];
float received_real[NUM_SAMPLES];
float received_imag[NUM_SAMPLES];

fract32 received_fr_real[NUM_SAMPLES];
fract32 received_fr_imag[NUM_SAMPLES];

fract32 filtered_fr_real[NUM_SAMPLES];
fract32 filtered_fr_imag[NUM_SAMPLES];

fir_state_fr32 state_real;
fir_state_fr32 state_imag;

#pragma section("L1_data_b")
fract32 delay_real[NUM_COEFFS];
#pragma section("L1_data_b")
fract32 delay_imag[NUM_COEFFS];

float received_filtered_real[NUM_SAMPLES];
float received_filtered_imag[NUM_SAMPLES];

#define AFTER_DOWN NUM_SAMPLES/OVERSAMPLING

float received_symbol_real[AFTER_DOWN];
float received_symbol_imag[AFTER_DOWN];

int received_symbols[AFTER_DOWN];

/*
 * This function is used to i
 */
void demodulator(){
	demodulate();
	filter_demodulator();
	dowmsample();
	demapper();
}

/*
 * Demodulates the received signal
 */
void demodulate(){
	for (int i = 0; i < NUM_SAMPLES; ++i) {
		received_real[i] = recieved_signal[i]*cos_modulator[i%8];
		received_imag[i] = -recieved_signal[i]*sin_modulator[i%8];
	}
}

/*
 * Filters the demodulated signal (square raised cosine filter)
 */
void filter_demodulator(){
	received_fr_imag = float_to_fr32(received_fr_imag);
	received_fr_real = float_to_fr32(received_fr_real);

	for (int i = 0; i < NUM_COEFFS; ++i) {
		delay_real[i] = 0;
		delay_imag[i] = 0;
	}

	fir_init(state_real,filter_coefficients,delay_real,NUM_COEFFS,0);
	fir_init(state_imag,filter_coefficients,delay_imag,NUM_COEFFS,0);

	fir_fr32(received_fr_real,filtered_fr_real,NUM_SAMPLES,&state_real);
	fir_fr32(received_fr_imag,filtered_fr_imag,NUM_SAMPLES,&state_imag);

	received_filtered_real = fr32_to_float(received_filtered_real);
	received_filtered_imag = fr32_to_float(received_filtered_imag);
}

/*
 * downsamples the signal
 */
void dowmsample(){
	for (int i = 0; i < AFTER_DOWN; ++i) {
		received_symbol_imag[i]=received_filtered_imag[i*8];
		received_symbol_real[i]=received_filtered_real[i*8];
	}
}

/*
 * Detects the received symbols
 */
void demapper(){
	double distance;
	double min_distance;



	for (int received_symbol_index = 0; received_symbol_index < AFTER_DOWN; ++received_symbol_index) {
		min_distance = 100000;
		for (int symbol_index = 0; symbol_index < NUMBER_OF_SYMBOLS; ++symbol_index) {
				distance = sqrt(pow(constelation_real[symbol_index]-received_filtered_real[received_symbol_index],2)+pow(constelation_real[symbol_index]-received_filtered_real[received_symbol_index],2));

				if(distance < min_distance){
					min_distance = distance;
					received_symbols[received_symbol_index] = symbol_index;
				}
		}
	}
}