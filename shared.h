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

/*
 * ------------------------------------------------------------- defines --
 */
 
/* Shared Memory Key: 1000 * <uid> + 0 - JPW: 42548 */
#define KEY_SEMAPHORE_W 42548000
/* Write Semaphore Key: 1000 * <uid> + 1 - PW: 42548 */
#define KEY_SEMAPHORE_R 42548001
/* Read Semaphore Key: 1000 * <uid> + 2 - PW: 42548 */
#define KEY_SHAREDMEM   42548002


/* Mode for linking shared memory - ACHTUNG - NOCH ÜBERARBEITEN */
#define SHM_MODE_READ 1 /* Use shared memory in read mode */
#define SHM_MODE_RW   0 /* Use shared memory in read/write mode */

/* Type of Semaphore*/
#define WRITE_SEM 1 /* write  */
#define READ_SEM 0 /* read */


#endif


