/**
* @Author: Sanzida Hoque
* @Course: CMP_SCI 4760 Operating Systems 
* @Sources: https://www.prodevelopertutorial.com/system-v-semaphores-in-c-using-semget-semctl-semop-system-v-system-calls-in-linux/
*/

#include "config.h"

int main(int argc, char *argv[]){
    printf("slave is processessing...\n");
    signal(SIGALRM, signal_handler);//Abort for end of termination time 
    signal(SIGINT, signal_handler);// Abort for Ctrl+C 
    /* Parsing Command Line argument */  
    int procid = atoi(argv[1]); 
    int nprocs = atoi(argv[2]); 
    printf("process id %d, no of process %d\n", procid, nprocs);
   
   /* Ensuring that IPC_CREAT created the segment in master */
    cstest_id = shmget(ftok("Makefile", '5'), sizeof(FILE) * nprocs, IPC_EXCL);
    if (cstest_id == -1){
        perror("slave: Error: Shared memory allocation failed");
    }
    cstest = (FILE *)shmat(cstest_id, NULL, 0);
    
    if (cstest == (void *) -1 ){
      perror("slave: Error: Shared memory attachment failed");
    }

    char logfname[256];
    sprintf(logfname, "logfile.%d", procid);

    time_t curtime;
    struct tm *loc_time;
    char buf[LEN];
    int random_t = 0;

   /* Grabbing semophore */
    sem_grab();
    
    for ( i = 0; i < 5; i++){
        srand(time(NULL) + procid + i); // seed for random sleep time
        /* acquire the semaphore */
        //TODO add code for acquire the semaphore
        sem_wait();
        /* Logging */   
        logfile = fopen(logfname, "a");
        cstest = fopen("cstest", "a");
        if (cstest == NULL || logfile == NULL){
            perror("slave: Error: file open failed");
        }             
        curtime = time (NULL); //Getting current time of system  	
   	loc_time = localtime (&curtime);// Converting current time to local time
        fprintf(logfile, "Process id %d enters critical section %s\n", procid, asctime (loc_time));
  	/* Critical Section */      
        /* wait for a random number of seconds before writing */
        random_t = (rand() % 5) + 1; // random numbers in range [1, 5]
        sleep(random_t);
 	curtime = time (NULL); //Getting current time of system  	
   	loc_time = localtime (&curtime);// Converting current time to local time
        fprintf(logfile, "Process id %d writes on shared resource, cstest %s\n", procid, asctime (loc_time));
        strftime (buf, LEN, "%H:%M:%S", loc_time);// formatting time
        fprintf(cstest, "%s Queue %d File modified by process number %d\n", buf, i+1 , procid);
       
	/* wait for a random number of seconds after writing */
        random_t = (rand() % 5) + 1; // random numbers in range [1, 5]
        sleep(random_t);
	curtime = time (NULL); //Getting current time of system  	
   	loc_time = localtime (&curtime);// Converting current time to local time
        fprintf(logfile, "Process id: %d exits critical section %s\n", procid, asctime (loc_time));
       
       /* closing files */
          fclose(logfile);
          fclose(cstest);
       
        //TODO add code for release semaphore
        sem_signal();
    }
    exit(EXIT_SUCCESS);
}


void sem_wait(){
    struct sembuf sb = {0, -1, 0}; //allocate resource
    if (semop(semid, &sb, 1) == -1) {
        perror("slave: semop failed");
        exit(EXIT_FAILURE); 
    }
}

void sem_signal(){
    struct sembuf sb = {0, 1, 0}; //free resource
    if (semop(semid, &sb, 1) == -1) {
        perror("slave: semop failed");
        exit(EXIT_FAILURE); 
    }
}

void sem_grab(){

    if ((key = ftok("Makefile", '4')) == -1) {   
        perror("slave: sem_grab(): ftok failed");
        exit(EXIT_FAILURE); 
    }
        
     if ((semid = semget(key, 1, 0)) == -1) {
        perror("slave: sem_grab(): semget failed");
        exit(EXIT_FAILURE); 
    }

}

void sem_remove(){
   
    /* grabing semaphore set created by sem_init() in master */ 
    if ((semid = semget(key, 1, 0)) == -1){
        perror("slave: sem_remove(): semget failed");
        exit(EXIT_FAILURE); 
    }
    
    /* removing semaphore */
    if (semctl(semid, 0, IPC_RMID, arg) == -1) {
        perror("slave: sem_remove(): semctl failed");
        exit(EXIT_FAILURE); 
    }

}


void signal_handler(){

	exit(EXIT_SUCCESS);
}


