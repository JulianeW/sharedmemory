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


/* The writer reads blocks of data from standard input and copies (“writes”) them
into a shared memory segment. */

#include "shared.h"

int main (int argc, char *argv[]) {


	/* Create a set containing the two semaphores that are used by the writer and
reader program to ensure that they alternate in accessing the shared memory
segment q. The semaphores are initialized so that the writer has first access to
the shared memory segment. Since the writer creates the semaphore set, it
must be started before the reader. */ 

	int sem_id;

	sem_id = semgrab

	semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);
	if (semid == -1)
		errExit("semget");
	if (initSemAvailable(semid, WRITE_SEM) == -1)
		errExit("initSemAvailable");
	if (initSemInUse(semid, READ_SEM) == -1)
		errExit("initSemInUse");




































int main (int argc, char *argv[]) {
	/* Holds the maximum amount of elements in the shared memory */
	int liBufElems = 0;
	/* The Input Char as int */
	int iInput = -1;

	/* Check and Convert Command Line Parameters */
	if ((liBufElems = CheckAndPrepareParameter(argc, argv)) == RET_ERR) return EXIT_FAILURE;

	/* init the resources */
	if (InitResources(MY_SENDER, liBufElems) == RET_ERR) return EXIT_FAILURE;

	/* Process the read of the input */
	do {
		/* Read a char from stdin */
		iInput = fgetc(stdin);
		
		/* Wait for the write semaphore */
		if (WaitForSemaphore() == RET_ERR) return EXIT_FAILURE;

		/* Write to shared memory */
		WriteToSharedMemory(iInput);

		/* Signal to read Semaphore */
		if (SignalToSemaphore() == RET_ERR) return EXIT_FAILURE;

	} while (iInput != EOF);

	/* Check for Input error or EOF */
	if (ferror(stdin)) {
		HANDLEERRORERRNO("Error Reading from stdin!");
		return EXIT_FAILURE;
	}

	/* Program finished, do Cleanup and return exit state */
	if (Cleanup() == RET_ERR) return EXIT_FAILURE;
	else 							return EXIT_SUCCESS;
}
