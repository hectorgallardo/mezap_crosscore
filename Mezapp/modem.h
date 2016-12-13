/*****************************************************************************
 * modem.h
 *****************************************************************************/

#ifndef __MODEM_H__
#define __MODEM_H__

/* Add your custom header content here */
#define SAMPLING_FREQ	48000	//Hz
#define CARRIER_FREQ	6000	//Hz

#define OVERSAMPLING					SAMPLING_FREQ/CARRIER_FREQ
#define FRAME							2
#define NUMBER_OF_SYMBOLS 				FRAME*2
#define NUMBER_OF_SYMBOLS_OVERSAMPLED	NUMBER_OF_SYMBOLS*8
#define NUM_COEFFS						49
#define NUM_SAMPLES						NUMBER_OF_SYMBOLS_OVERSAMPLED

#include <filter.h>
#include "modulator.h"
#include "demodulator.h"

extern char frame[];
extern int constelation_imag[];
extern int constelation_real[];

extern float sin_modulator[];
extern float cos_modulator[];

extern segment ("sdram0") fract32 delay_real[NUM_COEFFS];
extern segment ("sdram0") fract32 delay_imag[NUM_COEFFS];

extern fir_state_fr32 state_real;
extern fir_state_fr32 state_imag;

extern segment ("sdram0") fract32 filter_coefficients[];

#endif /* __MODEM_H__ */
