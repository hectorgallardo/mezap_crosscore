#include "filter_coeffs.h"
#include "filter.h"
#include "math.h"
#include "fract_typedef.h"
#include "fract2float_conv.h"


#define BITS 8
#define BITS_PER_SYMBOL 4
#define DATA_LENGTH 260
#define CHECKSUM_LENGTH 2
#define FRAME 263
#define NUMBER_OF_SYMBOLS FRAME*2
#define NUMBER_OF_SYMBOLS_OVERSAMPLED NUMBER_OF_SYMBOLS*8

char frame_init = 0xAA;
char data[DATA_LENGTH];
char checksum[CHECKSUM_LENGTH];
char frame[FRAME];
fract32 frame_symbols_real[NUMBER_OF_SYMBOLS];
fract32 frame_symbols_imag[NUMBER_OF_SYMBOLS];

fract32 frame_symbols_real_upsample[NUMBER_OF_SYMBOLS_OVERSAMPLED];
fract32 frame_symbols_imag_upsample[NUMBER_OF_SYMBOLS_OVERSAMPLED];


#define NUM_COEFFS 49
fir_state_fr32 state_real;
fir_state_fr32 state_imag;
#pragma section("L1_data_b")
fract32 delay_real[NUM_COEFFS+2];
#pragma section("L1_data_b")
fract32 delay_imag[NUM_COEFFS+2];

segment ("sdram0")  fract32 guardado_Real[BUFFER_SIZE/8];






int constelation_imag[] = {3, 1, -3, -1, 3, 1, -3, -1, 3, 1, -3, -1, 3, 1, -3, -1};
int constelation_real[] = {-3, -3, -3, -3, -1, -1 , -1, -1, 1, 1, 1, 1, 3, 3, 3, 3};

void encapsulation(){






}



void calculateChecksum(){
	int i=0;
	int total=4; //frame init tiene 4 bits a 1
	int aux;
	int j;

	for (i = 0; i < DATA_LENGTH; ++i) {
		for(j = 0; j < 8; j++){

			aux = data[i] & (1<<j);
			if(aux > 0)
				total++;
		}


	}

	checksum[0] = total & 0xF;
	checksum[1] = (total & 0xF0) >> 4;

}

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

void filter(){
	int i=0;

	/* Filter initializations */
	// MIKEL: Insert ere any filter initialization you might need
	for (i = 0; i < NUM_COEFFS; i++) /* clear the delay line */
	{
		delay_real[i] = 0;
		delay_imag[i] = 0;
	}

	fir_init(state_real, filter_coefficients, delay_real, NUM_COEFFS, 0);
	fir_init(state_imag, filter_coefficients, delay_imag, NUM_COEFFS, 0);

	fir_fr32(guardado1, salida1, NUM_SAMPLES, &state1);

	fir_fr32(guardado2, salida2, NUM_SAMPLES, &state2);
}





