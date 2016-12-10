#include "filter_coeffs.h"
#include <filter.h>
#include "modulator.h"
#include "modem.h"



char frame[FRAME];
fract32 frame_symbols_real[NUMBER_OF_SYMBOLS];
fract32 frame_symbols_imag[NUMBER_OF_SYMBOLS];

fract32 frame_symbols_real_upsample[NUMBER_OF_SYMBOLS_OVERSAMPLED];
fract32 frame_symbols_imag_upsample[NUMBER_OF_SYMBOLS_OVERSAMPLED];

/*
 * For filter use
 */

fract32 filtered_real_symbols[NUM_SAMPLES];
fract32 filtered_imag_symbols[NUM_SAMPLES];

fir_state_fr32 state_real;
fir_state_fr32 state_imag;

#pragma section("L1_data_b")
fract32 delay_real[NUM_COEFFS];
#pragma section("L1_data_b")
fract32 delay_imag[NUM_COEFFS];


/*
 * For the modulator
 */

fract32 modulated_signal[NUM_SAMPLES];

/*
 * This function is used in order to prepare the signal to send from the dac
 */
void modulator(){
	mapper();
	upsample();
	filter();
	modulate();
}

/*
 * The function used to parse the signal from binary to symbols
 */
void mapper(){
	int i=0;
	int numDecimal = 0;
	for (i = 0; i < NUMBER_OF_SYMBOLS; i++) {
		if(i%2 != 0) numDecimal = frame[i/2] & 0xF; //pares numeros de la derecha
		else numDecimal = ((frame[i/2] & 0xF0)>>4); //impares numeros de la izquierda

		frame_symbols_real[i] = constelation_real[numDecimal];
		frame_symbols_imag[i] = constelation_imag[numDecimal];
	}
}

/*
 * The function used to upsample the signal
 */
void upsample(){
	int i=0;

	for (i = 0; i < NUMBER_OF_SYMBOLS_OVERSAMPLED; i++) {
		if(i%8 == 0){
			frame_symbols_imag_upsample[i] = frame_symbols_imag[i/8];
			frame_symbols_real_upsample[i] = frame_symbols_real[i/8];
		}else{
			frame_symbols_imag_upsample[i] = 0;
			frame_symbols_real_upsample[i] = 0;
		}
	}
}

/*
 * The function used to filter the real and imaginary symbols out
 */
void filter(){
	int i=0;

	//Initializates the filter delay
	for (i = 0; i < NUM_COEFFS; i++)
	{
		delay_real[i] = 0;
		delay_imag[i] = 0;
	}

	//Initializates the filter
	fir_init(state_real, filter_coefficients, delay_real, NUM_COEFFS, 0);
	fir_init(state_imag, filter_coefficients, delay_imag, NUM_COEFFS, 0);

	//Filters the signal
	fir_fr32(frame_symbols_real_upsample, filtered_real_symbols, NUM_SAMPLES, &state_real);
	fir_fr32(frame_symbols_imag_upsample, filtered_imag_symbols, NUM_SAMPLES, &state_imag);
}

/*
 * The function used to modulate the symbols
 */
void modulate(){
	int i = 0;

	for (int i = 0; i < NUM_SAMPLES; ++i) {
		modulated_signal[i] = filtered_real_symbols[i]*cos_modulator[i%8] - filtered_imag_symbols[i]*sin_modulator[i%8];
	}
}



