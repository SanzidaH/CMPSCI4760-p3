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
 
/* variable for time */
time_t curtime;
struct tm *loc_time;


/* shm variables */
bool *choosing;
int *tickets;
int choose_id, ticket_id, cstest_id;
FILE *cstest = NULL, *logfile = NULL, *masterfile;

/* functions */   
void deallocate_shm();
int find_maximum(int *, int);

/* signalhandlers */
void signal_timer(int signal);
void signal_abort();

/* iterators */
int i, j, c; 
