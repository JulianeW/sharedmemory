/**
 * @file shared.c
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

/*
 * ------------------------------------------------------------- global variables --
 */

/* holds file name */
char * file_name = NULL;
/* holds ID of read semaphore */
int read_sem_id = -1;
/* holds ID of write semaphore */
int write_sem_id = -1;
/* holds ID of shared memory */
int shared_mem_id = 0;
/* pointer to shared memory */
int * shared_mem = NULL;
/* type of sender, receiver */
int file_type = -1;
/* position in shared memory */
int mem_pos = 0;
/* maximum elements of shared memory */
int max_elements_mem = -1;

/**
 *
 * \brief Function to check input parameters and return converted value for size of shared memory.
 * Set programme name to a global variable for error handling.
 *
 * \param argc   the argument count
 * \param argv   the pointer to the argument vector
 *
 * \return -1 if there was an error
 * \return any other return value which is a valid integer
 *
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
					print_errno("Error converting size of buffer:");
					usage();
					return -1;
				}
				/* check conversion and for values greater than 0 and make sure it is smaller than INT_MAX */
				if (optarg == '\0' || *end_pointer != '\0' || convert <= 0 || convert > INT_MAX)
				{
					print_errno("Invalid size of buffer!");
					usage();
					return -1;
				}
			break;

			default:
				print_errno("Unknown option!");
				usage();
				return -1;
		}
	}

	if (optind < argc)
	{
		print_errno("Unknown option!");
		usage();
		return -1;
	}

	return convert;
}

/**
 *
 * \brief Function to initialise all resources (semaphores, shared memory) and set global variables
 *
 * \param binary_type   type to initialise
 * \param mem_elements  size of shared memory
 *
 * \return -1 if there was an error
 * \return 0 if successful
 *
 */
extern int initialise_resource(const int binary_type, const int mem_elements)
{
	int bin_mode;

	/* set global variables accordingly */
	file_type = binary_type;
	max_elements_mem = mem_elements;


	if (binary_type == MY_RECEIVER)
	{
		bin_mode = SH_MEM_READ;
	}
	else
	{
		bin_mode = SH_MEM_RW;
	}

	/* create semaphores */
	if (create_sem(WRITE_SEM, max_elements_mem) == -1)
		return -1;
	if (create_sem(READ_SEM, 0) == -1)
		return -1;
	/* create shared memory */
	if (create_shared_mem(max_elements_mem) == -1)
		return -1;
	/* link memory */
	if (link_shared_mem(bin_mode) == -1)
		return -1;

	return 0;
}

/**
 *
 * \brief Function to let semaphore wait
 *
 *
 * \return -1 if there was an error
 * \return 0 if successful
 *
 */
extern int semaphore_wait(void)
{
	/* get semaphore depending on type */
	int my_sem;

	if (file_type == MY_SENDER)
		my_sem = write_sem_id;
	else
		my_sem = read_sem_id;

	errno = 0;

	while (P(my_sem) == -1)
	{
		if (errno != EINTR)
		{
			print_errno("Error waiting for semaphore: ");
			cleanup();
		}
		errno = 0;

	}
	return 0;
}

/**
 *
 * \brief Function to send signal to semaphore
 *
 * \return -1 if there was an error
 * \return 0 if successful
 *
 */
extern int signal_sem(void)
{
	/* get semaphore depending on type */
		int my_sem;

		if (file_type == MY_SENDER)
		{
			my_sem = write_sem_id;
		}
		else
		{
			my_sem = read_sem_id;
		}

		errno = 0;

		if (V(my_sem) == -1)
		{
			print_errno("Error Signaling Semaphore: ");
			cleanup();
			return -1;
		}
	return 0;
}

/**
 *
 * \brief Function write to shared memory
 *
 * \param input   what is written to the shared memory
 *
 */
extern void write_to_memory(int input)
{
	/* write to the offset position within shared memory */
	*(shared_mem + mem_pos) = input;
	/* when maximum number of elements in shared memory is reached, reset the position to 0 as it is a circular buffer */
	if (++mem_pos >= max_elements_mem)
		mem_pos = 0;

}

/**
 *
 * \brief Function to read from the shared memory
 * 
 * \return what is to be read from offset position
 *
 */
extern int read_from_memory(void)
{
	int position;
	position = mem_pos;

	/* when maximum number of elements in shared memory is reached, reset the position to 0 as it is a circular buffer */
	if (++mem_pos >= max_elements_mem)
		mem_pos = 0;

	/* read from offset position */
	return *(shared_mem+position);
}

/**
 *
 * \brief Function to create a semaphore depending on the type
 *
 * \param type         semaphore type (read or write)
 * \param init_value   value for initialisation of semaphore
 *
 * \return -1 if there was an error
 * \return 0 if everything is ok
 *
 */
extern int create_sem(const int type, const int init_value)
{
	int sem_key;
	int sem_id;
	errno = 0;

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

			/* grab existing semaphore*/
			if ((sem_id = semgrab(sem_key)) == -1)
			{
				print_errno("Error grabbing semaphore.");
				cleanup();
				return -1;
			}
		}
		else /* error creating the semaphore */
		{
			print_errno("Error creating semaphore.");
			cleanup();
			return -1;
		}
	}

	/* save semaphore ID in the correct variable */
	if (type == WRITE_SEM)
	{
		write_sem_id = sem_id;
	}
	else
	{
		read_sem_id = sem_id;
	}
	return 0;
}

/**
 *
 * \brief Function to create a shared memory
 *
 * \param buffersize     the size of the shared memory
 *
 * \return -1 if there was an error
 * \return 0 if everything is ok
 *
 */
extern int create_shared_mem(const int buffersize)
{
	errno = 0;

	if ((shared_mem_id = shmget(KEY_SHAREDMEM, sizeof(int) * buffersize, 0660|IPC_CREAT|IPC_EXCL)) == -1)
	{
		print_errno("Error creating shared memory.");
		cleanup();
		return -1;
	}

	return 0;
}

/**
 *
 * \brief Function to link the shared memory
 *
 * \param mode     mode to link the shared memory (SH_MEM_READ and SH_MEM_RW)
 *
 * \return -1 if there was an error
 * \return 0 if everything is ok
 *
 */
extern int link_shared_mem(const int mode)
{
	int sh_mode;
	errno = 0;

	if (mode == SH_MEM_READ)
	{
		sh_mode = SHM_RDONLY;
	}
	else
	{
		sh_mode = 0;
	}

	if ((shared_mem = shmat(shared_mem_id, NULL, sh_mode)) == (int *)-1)
	{
		print_errno("Could not link memory.");
		cleanup();
		return -1;
	}

	return 0;
}

/**
 *
 * \brief Function to unlink the shared memory
 *
 * \return -1 if there was an error
 * \return 0 if everything is ok
 *
 */
extern int unlink_shared_mem(void)
{
	errno = 0;

	if (shmdt(shared_mem) == -1)
	{
		print_errno("Could not unlink shared memory.");
		shared_mem = NULL;
		return -1;
	}

	shared_mem = NULL;
	return 0;
}

/**
 *
 * \brief Function removes semphores
 *
 * \param type    which type of semaphore to remove
 *
 * \return -1 if there was an error
 * \return 0 if everything is ok
 *
 */
extern int remove_sem(const int type)
{
	int sem_type;
	errno = 0;

	if (type == WRITE_SEM)
	{
		sem_type = write_sem_id;
	}
	else
	{
		sem_type = read_sem_id;
	}

	if (semrm(sem_type) == -1)
	{
		print_errno("Could not remove semaphore.");

		/* reset globals */
		if (type == WRITE_SEM)
		{
			write_sem_id = -1;
		}
		else
		{
			read_sem_id = -1;
		}
		return -1;
	}

	if (type == WRITE_SEM)
	{
		write_sem_id = -1;
	}
	else
	{
		read_sem_id = -1;
	}

	return 0;
}

/**
 *
 * \brief Function to remove shared memory
 *
 * \return -1 if there was an error
 * \return 0 if everything is ok
 *
 */
extern int remove_shared_mem(void)
{
	errno = 0;

	if (shmctl(shared_mem_id, IPC_RMID, NULL) == -1)
	{
		print_errno("Could not remove shared memory.");
		shared_mem_id = -1;
		return -1;
	}

	shared_mem_id = -1;
	return 0;
}

/**
 *
 * \brief Function for cleanup of shared memory and semaphore
 *
 * \return error status (which is -1 on failure and 0 on success)
 *
 */
extern int cleanup(void)
{
	int error_status = 0;

	/* unlink shared memory if it has been linked already */
	if (shared_mem != NULL)
	{
		if (unlink_shared_mem() == -1)
			error_status = -1;
	}

	/* only remove everything if we are in receiver process or in case of an error */
	if (file_type == MY_RECEIVER)
	{
		/* remove shared memory if it has been created */
		if (shared_mem_id != -1)
		{
			if (remove_shared_mem() == -1)
				error_status = -1;
		}
		/* remove read and write semaphore if they have been created already */
		if (write_sem_id != -1)
		{
			if (remove_sem(READ_SEM) == -1)
				error_status = -1;
		}
		if (read_sem_id != -1)
		{
			if (remove_sem(WRITE_SEM) == -1)
				error_status = -1;
		}

	}
	/* reset variables */
	file_type = -1;
	mem_pos = 0;
	max_elements_mem = -1;

	return error_status;
}

/**
 *
 * \ brief Function that prints out the usage message for the programme
 *
 * \return void
 *
 */
extern void usage(void)
{
	printf_handling("Usage: %s -m <buffer elements>\n", file_name);
}


/**
 *
 * \brief Function for printing error messages
 *
 * \param message    error message
 *
 */
extern void print_errno(char * message)
{
	if (errno != 0)
	{
		fprintf(stderr, "%s: %s - %s\n", file_name, message, strerror(errno));
	}
	else
	{
		fprintf(stderr, "%s: %s\n", file_name, message);
	}
}

/**
 *
 * \brief Function error handling of printf
 *
 * \param format
 * \param ...
 *
 */

void printf_handling(char * format, ...)
{
	va_list args;

	va_start(args, format);

	if (vprintf(format, args) < 0) 
		error(1, 1, "%d", errno);
	va_end(args);
}
