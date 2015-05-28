/**
 * @file sender.c
 * Betriebssysteme Shared Memory
 * Beispiel 3
 *
 * @author Claudia Baierl ic14b003 <claudia.baierl@technikum-wien.at>
 * @author Rene Elsner ic14b016 <rene.elsner@technikum-wien.at>
 * @author Juliane Piñeiro-Winkler ic14b059 <juliane.winkler@technikum-wien-at>
 * @date 2015/06/05
 *
 * @version 1
 *
 */
 
/*
 * -------------------------------------------------------------- includes --
 */

#include "shared.h"

int main (int argc, char *argv[]) {
/* Holds the maximum amount of elements in the shared memory */
	int liBufElems = 0;
/* The Input Char as int */
	int iInput = -1;

/* Check and Convert Command Line Parameters */
	if ((liBufElems = CheckAndPrepareParameter(argc, argv)) == MY_RETURN_ERR) return EXIT_FAILURE;

/* init the resources */
	if (InitResources(MY_RECEIVER, liBufElems) == MY_RETURN_ERR) return EXIT_FAILURE;

/* Process the read of shared memory */
	do {
/* Wait for the read semaphore */
		if (WaitForSemaphore() == MY_RETURN_ERR) return EXIT_FAILURE;

/* Write to shared memory */
		iInput = ReadFromSharedMemory();

/* Signal to write Semaphore */
		if (SignalToSemaphore() == MY_RETURN_ERR) return EXIT_FAILURE;

/* Output every int which stands for a char, except EOF */
		if (iInput != EOF) {

/* Do ouput and check for Output error */
			if (fputc(iInput, stdout) == EOF) {
				HANDLEERRORERRNO("Error Writing to stdout!");
				return EXIT_FAILURE;
			}
		}
	} while (iInput != EOF);

/* stdout is buffered, flush it */	
	if (fflush(stdout) == EOF) {
		HANDLEERRORERRNO("Error Writing to stdout!");
		return EXIT_FAILURE;
	}

/* Program finished, do Cleanup and return exit state */
	if (Cleanup() == MY_RETURN_ERR) return EXIT_FAILURE;
	else 							return EXIT_SUCCESS;
}
