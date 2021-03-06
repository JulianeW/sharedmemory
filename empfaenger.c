/**
 * @file empfaenger.c
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

/**
 *
 * \brief Program that handles the receiver
 *
 * This is the entry point of normal C programs.
 *
 * \param argc the number of arguments
 * \param argv the arguments (including the program name in argv[0])
 *
 * \return EXIT_SUCCESS if successful
 * \return EXIT_FAILURE if soemthing goes wrong
 *
 */

int main (int argc, char *argv[])
{
	/* Holds the maximum amount of elements in the shared memory */
	int max_elements = 0;
	/* input character as integer variable */
	int input = -1;

	/* check command line parameters and convert them */
	if ((max_elements = check_get_parameters(argc, argv)) == -1)
		return EXIT_FAILURE;

	/* initialise resources */
	if (initialise_resource(MY_RECEIVER, max_elements) == -1)
		return EXIT_FAILURE;

	/* Process the read of shared memory */
	do {
		/* Wait for the read semaphore */
		if (semaphore_wait() == -1)
			return EXIT_FAILURE;

		/* read from shared memory */
		input = read_from_memory();

		/* send signal to read Semaphore */
		if (signal_sem() == -1)
			return EXIT_FAILURE;

		/* output everything except for EOF */
		if (input != EOF) {

			/* Output and check for output error */
			if (fputc(input, stdout) == EOF)
			{
				print_errno("Error Writing to stdout!");
				cleanup();
				return EXIT_FAILURE;
			}
		}
	} while (input != EOF);

	/* flush stdout as it is buffered */
	if (fflush(stdout) == EOF)
	{
		print_errno("Error Writing to stdout!");
		cleanup();
		return EXIT_FAILURE;
	}

	/* programme finished - clean up and return success if clean up works */
	if (cleanup() == -1)
	{
		return EXIT_FAILURE;
	}
	else
	{
		return EXIT_SUCCESS;
	}
}
