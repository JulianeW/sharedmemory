/**
 * @file shared.h
 * Betriebssysteme Headerfile sender.c und empfaenger.c
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
#include <sys/types.h>
#include "sem182.h"

/*
 * ------------------------------------------------------------- defines --
 */
 


/* Semaphore write key: 1000 * <uid> + 0 */
#define KEY_SEMAPHORE_W 42548000
/* Semaphore read key: 1000 * <uid> + 1 */
#define KEY_SEMAPHORE_R 42548001
/* Shared memory key: 1000 * <uid> + 2 */
#define KEY_SHAREDMEM 42548002


/* Mode for linking shared memory - ACHTUNG - NOCH ÜBERARBEITEN */
#define SHM_MODE_READ 1 /* Use shared memory in read mode */
#define SHM_MODE_RW 0 /* Use shared memory in read/write mode */

/* Type of Semaphore*/
#define WRITE_SEM 1 /* write  */
#define READ_SEM 0 /* read */

/*
 * ------------------------------------------------------------- prototypes --
 */

extern void printf_handling(char * format, ...);
extern int check_get_parameters(const int argc, char * argv[]);
extern int create_sem(const int type, const int init_value);
extern int create_shared_mem(const int buffersize);
extern int link_shared_mem(const int mode);
extern int unlink_shared_mem(void);
extern int remove_sem(const int type);
extern int remove_shared_mem(void);
extern int cleanup(const int clean_mode);
extern void usage(void);


/*
 * ------------------------------------------------------------- variables --
 */

/* holds file name */
extern char * file_name = NULL;
/* holds ID of read semaphore */
extern int read_sem_id = -1;
/* holds ID of write semaphore */
extern int write_sem_id = -1;
/* holds ID of shared memory */
extern int shared_mem_id = -1;
/* pointer to shared memory */
extern int * shared_mem;
/* type of sender, receiver */
extern int file_type = -1;
/* position in shared memory */
extern mem_pos = 0;
/* maximum elements of shared memory */
extern int max_elements_mem = 0;

#endif

