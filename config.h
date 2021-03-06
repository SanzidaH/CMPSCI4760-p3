/*libraries*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<getopt.h>
#include<sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/sem.h>

/* macros */
#define MAXPROC 20
#define SHM_KEY 0x12345 //shared memory key
#define LEN 150

union semun {
    int val;                /* value for SETVAL */
    struct semid_ds *buf;   /* buffer for IPC_STAT & IPC_SET */
    unsigned short *array;  /* array for GETALL & SETALL */
    struct seminfo *__buf;  /* buffer for IPC_INFO */
};

/* sem variables */
key_t key;
int semid;
union semun arg;
char logfname[256];

/* functions */ 
void sem_init();
void sem_remove();
void shm_remove();
void sem_grab();
void sem_wait();
void sem_signal();
 
/* variable for time */
time_t curtime;
struct tm *loc_time;


/* shm variables */
int cstest_id;
FILE *cstest = NULL, *logfile = NULL;


/* signalhandlers */
void signal_timer(int signal);
void signal_abort();
void signal_handler();

/* iterators */
int i, j, c; 
