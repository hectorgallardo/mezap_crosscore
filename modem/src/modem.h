/*****************************************************************************
 * modem.h
 *****************************************************************************/

#ifndef __MODEM_H__
#define __MODEM_H__

/* Add your custom header content here */
#define SAMPLING_FREQ	48000	//Hz
#define CARRIER_FREQ	6000	//Hz

#define OVERSAMPLING					SAMPLING_FREQ/CARRIER_FREQ
#define FRAME							263
#define NUMBER_OF_SYMBOLS 				FRAME*2
#define NUMBER_OF_SYMBOLS_OVERSAMPLED	NUMBER_OF_SYMBOLS*8
#define NUM_COEFFS						49
#define NUM_SAMPLES						NUMBER_OF_SYMBOLS_OVERSAMPLED

int constelation_imag[] = {3, 1, -3, -1, 3, 1, -3, -1, 3, 1, -3, -1, 3, 1, -3, -1};
int constelation_real[] = {-3, -3, -3, -3, -1, -1 , -1, -1, 1, 1, 1, 1, 3, 3, 3, 3};

float sin_modulator[] = {0,	0.7071, 1,	0.7071,		0,	-0.7071,	-1,	-0.7071};
float cos_modulator[] = {1,	0.7071,	0,	-0.7071,	-1,	-0.7071,	0,	0.7071};

#endif /* __MODEM_H__ */
