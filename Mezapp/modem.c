/*****************************************************************************
 * modem.c
 *****************************************************************************/

#include <sys/platform.h>
#include "adi_initialize.h"
#include "modem.h"

/**
 * If you want to use command program arguments, then place them in the following string.
 */
char __argv_string[] = "";

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

	modulator();

	/* Begin adding your custom code here */

	return 0;
}

