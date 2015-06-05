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

/**
 *
 * \brief Program that handles the sender
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

	/* The Input Char as int */
	int input = -1;

	/* Check and Convert Command Line Parameters */
	if ((max_elements = check_get_parameters(argc, argv)) == -1)
		return EXIT_FAILURE;

	/* initialise the resources */
	if (initialise_resource(MY_SENDER, max_elements) == -1)
		return EXIT_FAILURE;

	/* Process the read of the input */
	do {
		/* Read a char from stdin */
		input = fgetc(stdin);
		
		/* Wait for the write semaphore */
		if (semaphore_wait() == -1)
			return EXIT_FAILURE;

		/* Write to shared memory */
		write_to_memory(input);

		/* Signal Semaphore */
		if (signal_sem() == -1)
			return EXIT_FAILURE;

	} while (input != EOF);

	/* Check for Input error or EOF */
	if (ferror(stdin)) 
	{
		print_errno("Error Reading from stdin!");
		cleanup();
		return EXIT_FAILURE;
	}

	/* Program finished, do Cleanup and return exit state */
	if (cleanup() == -1)
	{
		return EXIT_FAILURE;
	}
	else
	{
		return EXIT_SUCCESS;
	}
}

