#include <filter.h>
#include "modem.h"
#include "demodulator.h"
#include <fract2float_conv.h>
#include <math.h>

segment ("sdram0") fract32 received_signal[NUM_SAMPLES_TX];
segment ("sdram0") fract32 received_real[NUM_SAMPLES_RX];
segment ("sdram0") fract32 received_imag[NUM_SAMPLES_RX];

segment ("sdram0") fract32 received_fr_real[NUM_SAMPLES_TX];
segment ("sdram0") fract32 received_fr_imag[NUM_SAMPLES_TX];

segment ("sdram0") fract32 filtered_fr_real[NUM_SAMPLES_TX];
segment ("sdram0") fract32 filtered_fr_imag[NUM_SAMPLES_TX];

segment ("sdram0") fract32 received_filtered_real[NUM_SAMPLES_RX];
segment ("sdram0") fract32 received_filtered_imag[NUM_SAMPLES_RX];

#define AFTER_DOWN NUM_SAMPLES_TX/OVERSAMPLING

segment ("sdram0") fract32 received_symbol_real[NUMBER_OF_SYMBOLS];
segment ("sdram0") fract32 received_symbol_imag[NUMBER_OF_SYMBOLS];

segment ("sdram0") char detected_bits[NUMBER_OF_SYMBOLS*BITS_PER_SYMBOL];

void init_demodulator(){
    for (int i = 0;  i < NUM_SAMPLES_TX; i++) {
        received_signal[i] = modulated_signal[i];
    }

}
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

    for (int i = 0; i < NUM_SAMPLES_TX; i++) {
        received_real[i] =(received_signal[i]*cos_modulator_6KHz[i%8])*1.4142;
        received_imag[i] =(-received_signal[i]*sin_modulator_6KHz[i%8])*1.4142;
    }
}

/*
 * Filters the demodulated signal (square raised cosine filter)
 */
void filter_demodulator(){

    for (int i = 0; i < NUM_COEFFS; i++) {
        delay_real[i] = 0;
        delay_imag[i] = 0;
        received_real[NUM_SAMPLES_TX+i]=0;
        received_imag[NUM_SAMPLES_TX+i]=0;
    }

    fir_init(state_real,filter_coefficients,delay_real,NUM_COEFFS,0);
    fir_init(state_imag,filter_coefficients,delay_imag,NUM_COEFFS,0);

    fir_fr32(received_real,filtered_fr_real,NUM_SAMPLES_RX,&state_real);
    fir_fr32(received_imag,filtered_fr_imag,NUM_SAMPLES_RX,&state_imag);

}

/*
 * downsamples the signal
 */

//coge los symbolos del real y del imaginario
void dowmsample(){
    for (int i = 0; i < NUMBER_OF_SYMBOLS; i++) {
            received_symbol_imag[i]=filtered_fr_imag[2*FILTER_DELAY+i*8];
            received_symbol_real[i]=filtered_fr_real[2*FILTER_DELAY+i*8];
    }
}

/*
 * Detects the received symbols
 */
void demapper(){
    double distance;
    double min_distance;

    for (int received_symbol_index = 0; received_symbol_index < NUMBER_OF_SYMBOLS; received_symbol_index++) {
        min_distance = 100000;
        for (int symbol_index = 0; symbol_index < SYMBOLS_16QAM; symbol_index++) {
            distance = sqrt(pow(constelation_real[symbol_index]-received_symbol_real[received_symbol_index],2)+pow(constelation_imag[symbol_index]-received_symbol_imag[received_symbol_index],2));

            if(distance < min_distance){
                min_distance = distance;
                detected_bits[received_symbol_index] = symbol_index;
            }
        }
    }
}
