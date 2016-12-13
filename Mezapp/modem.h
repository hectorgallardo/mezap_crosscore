/*****************************************************************************
 * modem.h
 *****************************************************************************/

#ifndef __MODEM_H__
#define __MODEM_H__

/* Add your custom header content here */
#define SAMPLING_FREQ	48000	//Hz
#define CARRIER_FREQ	12000	//Hz
#define SYMBOL_FREQ		6000	//Hz

#define OVERSAMPLING					SAMPLING_FREQ/SYMBOL_FREQ
#define FRAME							2
#define NUMBER_OF_SYMBOLS 				FRAME*2
#define NUMBER_OF_SYMBOLS_OVERSAMPLED	NUMBER_OF_SYMBOLS*8
#define NUM_COEFFS						49
#define NUM_SAMPLES						NUMBER_OF_SYMBOLS_OVERSAMPLED + NUM_COEFFS

#include <filter.h>
#include "modulator.h"
#include "demodulator.h"

extern char frame[];
extern fract32 constelation_imag[];
extern fract32 constelation_real[];

extern float sin_modulator[];
extern float cos_modulator[];

extern segment ("sdram0") fract32 delay_real[NUM_COEFFS];
extern segment ("sdram0") fract32 delay_imag[NUM_COEFFS];

extern fir_state_fr32 state_real;
extern fir_state_fr32 state_imag;

extern segment ("sdram0") fract32 filter_coefficients[];
extern fract32 modulated_signal[NUM_SAMPLES];

#endif /* __MODEM_H__ */
