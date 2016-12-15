/*
 * uart.c
 *
 *  Created on: 15/12/2016
 *      Author: Hector
 */

#include "uart.h"
#include "modem.h"



#define BAUD_RATE           9600u

/* External input clock frequency in Hz */
#define 	PROC_CLOCK_IN       		25000000
/* Maximum core clock frequency in Hz */
#define 	PROC_MAX_CORE_CLOCK 		600000000
/* Maximum system clock frequency in Hz */
#define 	PROC_MAX_SYS_CLOCK  		133333333
/* Minimum VCO clock frequency in Hz */
#define 	PROC_MIN_VCO_CLOCK  		25000000
/* Required core clock frequency in Hz */
#define 	PROC_REQ_CORE_CLOCK 		400000000
/* Required system clock frequency in Hz */
#define 	PROC_REQ_SYS_CLOCK  		100000000

/* Rx and Tx buffers */
char BufferTx1[FRAME];
char BufferTx2[FRAME];
char BufferRx1[FRAME];
char BufferRx2[FRAME];

unsigned char *UBufferBidali,*UBufferJaso;
bool enviar;

/* UART Handle */
static ADI_UART_HANDLE hDevice;

static ADI_UART_RESULT respuestaTx;
static ADI_UART_RESULT respuestaRx;

uint32_t	Result;

//Variables conversores
fract32 *ptr_fr32;

#define BUFFER_SIZE_CONV (65536*FRAME)

#define CARACTERES_PRUEBA 8

section ("sdram0") fract32 captura_prueba[BUFFER_SIZE_CONV/4];

section ("sdram0") int indice_guardado = 0;

//Variables UART
section ("sdram0") unsigned char trama_entrada_mod[CARACTERES_PRUEBA + FRAME];// = {"actualizar"};
section ("sdram0") unsigned char trama_salida_demod[FRAME];
section ("sdram0") unsigned char entrada_test[FRAME];



/* Memory required for operating UART in dma mode */
static uint8_t gUARTMemory[ADI_UART_BIDIR_DMA_MEMORY_SIZE];

static bool bError;

static void CheckResult(ADI_UART_RESULT result) {
	if (result != ADI_UART_SUCCESS) {
		printf("UART failure\n");
		bError = true;
	}
	/* IF (Debug info enabled) */
#if defined(ENABLE_DEBUG_INFO)
#define DEBUG_MSG1(message)     printf(message)
#define DEBUG_MSG2(message, result) \
		do { \
			printf(message); \
			if(result) \
			{ \
				printf(", Error Code: 0x%08X", result); \
				printf("\n"); \
			} \
		} while (0)
	/* ELSE (Debug info disabled) */
#else

#define DEBUG_MSG1(message)
#define DEBUG_MSG2(message, result)

#endif

}

void initializations(){

	unsigned int i;
	ADI_UART_RESULT eResult;

	/* Initialize managed drivers and/or services */
	adi_initComponents();



	/* Initialize power service */
	Result = (uint32_t) adi_pwr_Init (PROC_CLOCK_IN, PROC_MAX_CORE_CLOCK, PROC_MAX_SYS_CLOCK, PROC_MIN_VCO_CLOCK);

	/* IF (Failure) */
	if (Result)
	{
		DEBUG_MSG1 ("Failed to initialize Power service\n");
	}

	/* IF (Success) */
	if (Result == 0)
	{
		/* Set the required core clock and system clock */
		Result = (uint32_t) adi_pwr_SetFreq(PROC_REQ_CORE_CLOCK, PROC_REQ_SYS_CLOCK);

		/* IF (Failure) */
		if (Result)
		{
			DEBUG_MSG1 ("Failed to initialize Power service\n");
		}
	}

	//Definiciones UART
	/* UART Device Number connected to RS232 socket on the BF537 EZ-KIT Lite */
#define UART_DEVICE_NUM     0u

	//UART initialization
	/* Open UART driver */
	eResult = adi_uart_Open(UART_DEVICE_NUM, ADI_UART_DIR_BIDIRECTION,
			gUARTMemory, ADI_UART_BIDIR_DMA_MEMORY_SIZE, &hDevice);
	CheckResult(eResult);

	/* Set UART Baud Rate */
	eResult = adi_uart_SetBaudRate(hDevice, BAUD_RATE);
	CheckResult(eResult);

	/* Configure  UART device with NO-PARITY, ONE STOP BIT and 8bit word length. */
	eResult = adi_uart_SetConfiguration(hDevice, ADI_UART_NO_PARITY,
			ADI_UART_ONE_STOPBIT, ADI_UART_WORDLEN_8BITS);
	CheckResult(eResult);

	/* Enable the DMA associated with UART if UART is expeced to work with DMA mode */
	eResult = adi_uart_EnableDMAMode(hDevice, true);
	CheckResult(eResult);

	printf("Setup terminal on PC as described in Readme file. \n\n");
	printf("Type characters in the terminal program and notice the characters being echoed.\n\n");
	printf("Press the return key to stop the program.\n");

	adi_uart_SubmitTxBuffer (hDevice, BufferTx1, FRAME);
	adi_uart_SubmitRxBuffer (hDevice, BufferTx2, FRAME);
	adi_uart_SubmitTxBuffer (hDevice, BufferRx1, FRAME);
	adi_uart_SubmitRxBuffer (hDevice, BufferRx2, FRAME);

	eResult= adi_uart_EnableTx(hDevice,true);
	eResult= adi_uart_EnableRx(hDevice,true);


}




void salirPorUART(){

	enviar = 0;

	// Se bloquea hasta que haya buffer (en condiciones normales siempre debería haber)
	while(!enviar)
	{
		respuestaTx=adi_uart_IsTxBufferAvailable (hDevice, &enviar);
	}

	// Cpgemos puntero a buffer de tx libre
	respuestaTx = adi_uart_GetTxBuffer (hDevice, &UBufferBidali);

	// Copiamos lo que quermoes enviar
	memcpy(UBufferBidali, entrada_test, FRAME);

	//Enviamos el buffer
	respuestaTx = adi_uart_SubmitTxBuffer (hDevice, UBufferBidali, FRAME); //envia
}

void comprobarEntradaUART(){

	/* Read a character */
	respuestaRx=adi_uart_IsRxBufferAvailable (hDevice, &enviar);


	if(enviar){
		respuestaRx = adi_uart_GetRxBuffer (hDevice, &UBufferJaso);//recibe

		// Aquí lo copio al buffer de tx y lo envio
		memcpy(entrada_test,UBufferJaso, FRAME);	//void *s1, const void *s2, size_t n
		salirPorUART();

		// Acordarse de volver a enviar el buffer
		respuestaRx=adi_uart_SubmitRxBuffer (hDevice, UBufferJaso, FRAME);
	}
	//

}


