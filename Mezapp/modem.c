/*****************************************************************************
 * modem.c
 *****************************************************************************/

#include <sys/platform.h>
#include "adi_initialize.h"
#include "modem.h"
#include "peripherals.h"


/**
 * If you want to use command program arguments, then place them in the following string.
 */
char __argv_string[] = "";
#include <stdio.h>

int main(int argc, char *argv[])
{
	/**
	 * Initialize managed drivers and/or services that have been added to
	 * the project.
	 * @return zero on success
	 */
	//adi_initComponents();

	frame[0] = 0xAF;
	frame[1] = 0xAB;
	char* fff = "ib";
	memcpy(send_through_uart,fff,strlen(fff));
	initializate_peripherals();
	while(1){
	sendThroughUART();
	printf("%s", receive_from_uart);
	}

//	salirPorUART();
//	init_modulator();
//	modulator();
//
//	init_demodulator();
//	demodulator();
//
	/* Begin adding your custom code here */

	return 0;
}

