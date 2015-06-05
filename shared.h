/**
 * @file shared.h
 * Betriebssysteme Headerfile sender.c, empfaenger.c und myshared.c
 *
 * @author Claudia Baierl ic14b003 <claudia.baierl@technikum-wien.at>
 * @author Rene Elsner ic14b016 <rene.elsner@technikum-wien.at>
 * @author Juliane Piñeiro-Winkler ic14b059 <juliane.winkler@technikum-wien-at>
 * @date 2015/06/05
 *
 * @version 1 
 *
 */

#ifndef SHARED_H

#define SHARED_H

/*
 * -------------------------------------------------------------- includes --
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <error.h>
#include <limits.h>
#include "sem182.h"
#include <ctype.h>
#include <libgen.h>
#include <stdarg.h>

/*
 * ------------------------------------------------------------- defines --
 */
 


/* Semaphore write key: 1000 * <uid> + 0 */
#define KEY_SEMAPHORE_W 42548000
/* Semaphore read key: 1000 * <uid> + 1 */
#define KEY_SEMAPHORE_R 42548001
/* Shared memory key: 1000 * <uid> + 2 */
#define KEY_SHAREDMEM 42548002


/* Mode for linking shared memory */
#define SH_MEM_READ 1 /* Use shared memory in read mode */
#define SH_MEM_RW 0 /* Use shared memory in read/write mode */

/* Type of Semaphore*/
#define WRITE_SEM 1 /* write */
#define READ_SEM 0 /* read */

/* Type of process */
#define MY_SENDER   0 /* Initialise resources as sender */
#define MY_RECEIVER 1 /* Initialise resources as receiver */


/*
 * ------------------------------------------------------------- prototypes --
 */

extern int check_get_parameters(const int argc, char * argv[]);
extern int initialise_resource(const int binary_type, const int mem_elements);
extern int semaphore_wait(void);
extern int signal_sem(void);
extern void write_to_memory(int input);
extern int read_from_memory(void);
extern int create_sem(const int type, const int init_value);
extern int create_shared_mem(const int buffersize);
extern int link_shared_mem(const int mode);
extern int unlink_shared_mem(void);
extern int remove_sem(const int type);
extern int remove_shared_mem(void);
extern int cleanup(void);
extern void usage(void);
extern void print_errno(char * message);
void printf_handling(char * format, ...);


#endif
