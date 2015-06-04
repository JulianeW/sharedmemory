/**
 * @file myshared.c
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

int main(int argc, char * argv[])
{






}
/**
 *
 * \brief Function: Check the parameters given to the binary and returns the converted
 * value for the amount of shared memory slots. Sets also the binaryname
 * to a global variable for error output
 *
 * \param argc the argument count
 * \param argv the pointer to the argument vector
 *
 * \return -1 if there was an error
 * \return any other return value which is a valid integer
 */
extern int check_get_parameters(const int argc, char * argv[])
{
	long int convert = -1;
	char * end_pointer = NULL;
	int options = -1;

	errno = 0;

	/* set global filename */
	file_name = argv[0];

	/* check parameters */
	while ((options = getopt(argc, argv, "m:")) != -1)
	{
		switch(options)
		{
			case 'm':
				convert = strtol(optarg, &end_pointer, 10);
				if (errno != 0)
				{
					fprintf(stderr "Error converting size of buffer: %s ", file_name);
					usage();
					return -1;
				}
				/* check conversion and for values greater than 0 and make sure it is smaller than INT_MAX */
				if (optarg == '\0' || end_pointer != '\0' || convert <= 0 || convert > INT_MAX)
				{
					fprintf(stderr "%s: Invalid size of buffer!", file_name);
					usage();
					return -1;
				}
			break;

			default:
				fprintf(stderr "Unknown option!");
				usage();
				return -1;
		}
	}

	if (optind < arg)
	{
		fprintf(stderr "Unknown option!");
		usage();
		return -1;
	}

	return convert;

}

/**
 *
 * \brief Function:
 *
 * \param type semaphore type (read or write)
 * \param init_value value for initialisation of semaphore
 *
 * \return -1 if there was an error
 * \return 0 if everything is ok
 */
extern int create_sem(const int type, const int init_value)
{
	int sem_id = -1;
	errno = 0;
	int sem_key = 0;

	/* check for type before creating semaphore */
	if (type == WRITE_SEM)
		sem_key = KEY_SEMAPHORE_W;
	else
		sem_key = KEY_SEMAPHORE_R;

	/* initialise semaphore */
	if ((sem_id = seminit(sem_key, 0660, init_value)) == -1)
	{
		if (errno == EEXIST)
		{
			errno = 0;
			/* semaphore exits - grab it */
			if ((sem_id = samgrab(type)) == -1);
			{
				fprintf(stderr "Error grabbing semaphore: %s", file_name);
				/* ACHTUNG: Hier noch CleanUp machen!!! */
				return -1;
			}
		}
		else /* error creating the semaphore */
		{
			fprintf(stderr "Error creating semaphore: %s", file_name);
			/* ACHTUNG: Hier noch CleanUp machen!!! */
			return -1;
		}
	}

	/* save semaphore id in the right variable */
	if (type == WRITE_SEM)
		write_sem_id = sem_id;
	else
		read_sem_id = sem_id;
	return 0;

}

extern int create_shared_mem(const int buffersize)
{
	errno = 0;
	if ((shared_mem_id = shmget(KEY_SHAREDMEM, sizeof(int) * buffersize, 0660|IPC_CREAT)) == -1)
	{
		fprintf(stderr "Error creating semaphore: %s", file_name);
		/* ACHTUNG: Hier noch CleanUp machen!!! */
		return -1;
	}

	return 0;
}

extern int link_shared_mem(const int mode)
{


}

extern int unlink_shared_mem(void)
{


}

extern int remove_sem(const int type)
{


}

extern int remove_shared_mem(void)
{


}

extern int cleanup(const int clean_mode)
{


}

/**
 *
 * \ brief Function prints out the usage message for the programme
 *
 * \return void
 */
extern void usage(void)
{
	printf_handling("Usage: %s -m <buffer elements>\n", file_name);
}

/**
 *
 * \brief Function error handling of printf
 *
 * \param format
 * \param ...
 *
 */
extern void printf_handling(char * format, ...)
{
	va_list args;

	va_start(args, format);

	if (vprintf(format, args) < 0)
		error(1, 1, "%d", errno);

	va_end(args);
}
