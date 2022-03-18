/**
* @Author: Sanzida Hoque
* @Course: CMP_SCI 4760 Operating Systems 
* @Sources: https://www.prodevelopertutorial.com/system-v-semaphores-in-c-using-semget-semctl-semop-system-v-system-calls-in-linux/
*           
*/


#include "config.h"

int opt, nprocs = 20, terminate_time = 100, errno, pid = 0,  *parents = NULL, *children = NULL;
time_t startTime = 0;

int main(int argc, char *argv[]){

    printf("master is processessing...\n");
    /* Interrupt signal handling */
    signal(SIGALRM, signal_timer);//Abort for end of termination time 
    signal(SIGINT, signal_abort);// Abort for Ctrl+C     
  
    /* Parsing Command Line argument */
    while ((opt = getopt(argc, argv, "t:n:h:")) != -1) {

        switch (opt) {
        case 'h':
            printf("Help:\n");
            printf("Run using master -t [ss] [n]\n");
            printf("number of process > 20 will be automatically set for 20.");
        case 't':
            terminate_time = atoi(optarg);
            if(terminate_time < 1){
 		perror("master: Termination time is not valid");
            }
            nprocs = atoi(argv[3]);
            if(nprocs > MAXPROC){
            	perror("master: Warning: Numbers of Processes can not exceed 20");
            	nprocs = 20;
            }
            break;
        case 'n':
            nprocs = atoi(optarg);
            if(nprocs > MAXPROC){
            	perror("master: Warning: Numbers of Processes can not exceed 20");
            	nprocs = 20;
            }
            break;
        case '?':
            if (opt == 'c')
                perror("master: Error: Option requires an argument");
            else
                perror("master: Error: Unknown option character");
            return 1;

        }

    }

    fprintf(stderr, "terminate_time: %d\n", terminate_time);
    fprintf(stderr, "nprocs: %d\n", nprocs);
    parents = malloc(sizeof(int) * nprocs);
    children = malloc(sizeof(int) * nprocs); 
    

    /* initialization of shared resource for critical section */
    cstest_id = shmget(ftok("Makefile", '5'), sizeof(FILE) * nprocs, IPC_CREAT | 0666);

    if (cstest_id == -1){
        perror("master: Error: Shared memory allocation failed");
    }
    cstest = (FILE *)shmat(cstest_id, NULL, 0);
        
    if (cstest == (void *) -1 ) {
       perror("master: Error: Shared memory attachment failed");
   }
    

    curtime = time (NULL); //Getting current time of system  	
    loc_time = localtime (&curtime);// Converting current time to local time
    fprintf(stderr, "master: Initializing Semaphore > %s\n", asctime (loc_time));     
    sem_init(); // check  from cmd :  ipcs -s
    
      
    /* spawning child processes and waiting to finish */
    startTime = time(NULL);
    alarm(terminate_time);
    for (int i = 0; i < nprocs; i++){
        
        pid = fork();//spawn fan of child processes
        //printf("pid %d\n",pid);
	
	/* Checking fork */
	if (pid == -1){ // Something went wrong
	    perror("master: Error: Failed to create a child process");
	    exit(EXIT_FAILURE);
	}

        if (pid == 0){// Child processes
            char procid[256];
            sprintf(procid, "%d", i+1);
            char numprocs[256];
            sprintf(numprocs, "%d", nprocs);
            printf("child pid %d from the"
                   " parent pid %d, master set pid = %d\n",
                getpid(), getppid(),i+1); 
            children[i] = pid; 
            errno = execl("./slave", "./slave" , procid, numprocs, NULL); // Processing slaves
            if(errno == -1){
                perror("master: Error: Can't process slave!");
            }
            exit(EXIT_SUCCESS);
        }
        else{ // Parent processes
            parents[i] = pid; 
        }

    }

    for (j = 0; j < nprocs; j++)
         wait(NULL);
   
    curtime = time (NULL); //Getting current time of system  	
    loc_time = localtime (&curtime);// Converting current time to local time
    fprintf(stderr, "master: master is terminating successfully > %s\n", asctime (loc_time));
    
    sem_remove(); 
    shm_remove();
    exit(EXIT_SUCCESS);
}


void sem_init(){

    if ((key = ftok("Makefile", '4')) == -1){   
        perror("master: sem_init(): ftok failed");
        exit(EXIT_FAILURE); 
    }
    
/* create a semaphore set with 1 semaphore: */
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1){
        perror("master: sem_init(): semget failed");
        exit(EXIT_FAILURE); 
    }
/* initialize semaphore #0 to 1: */ 
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1){
        perror("master: sem_init(): semctl failed");
        exit(EXIT_FAILURE); 
    }

}

void sem_remove(){
    
    curtime = time (NULL); //Getting current time of system  	
    loc_time = localtime (&curtime);// Converting current time to local time
    fprintf(stderr, "master: Removing Semaphore > %s\n", asctime (loc_time)); 
    /* grab the semaphore set created by sem_init(): */ 
    if ((semid = semget(key, 1, 0)) == -1) {
        perror("master: sem_remove(): semget failed");
        exit(EXIT_FAILURE); 
    } 
    
    /* removing semaphore */
    if (semctl(semid, 0, IPC_RMID, arg) == -1) {
        perror("master: sem_remove(): semctl failed");
        exit(EXIT_FAILURE); 
    }
    curtime = time (NULL); //Getting current time of system  	
    loc_time = localtime (&curtime);// Converting current time to local time
}

void shm_remove(){

  if (shmdt(cstest) == -1) {
      perror("master: Error: shmdt failed to detach memory");
  }
  if (shmctl(cstest_id, IPC_RMID, 0) == -1 ) {
      perror("master: Error: shmctl failed to delete shared memory");
  }  
}

/* signal handle for time out */
void signal_timer(int signal){
        curtime = time (NULL); //Getting current time of system  	
        loc_time = localtime (&curtime);// Converting current time to local time
	if(difftime(time(NULL),startTime) >= terminate_time){
	   fprintf(stderr, "master: Warning: timer ends > %s\n", asctime (loc_time));
	   killpg((*parents), SIGTERM);
	   killpg((*children), SIGTERM);
	    for(int i = 0; i < nprocs; i++){
		wait(NULL);
	    }
	   sem_remove();
	   shm_remove();
	   //abort();
	   exit(EXIT_SUCCESS);
	}
}


/* signal handle for receiving CTRL + C */
void signal_abort(){ 
    curtime = time (NULL); //Getting current time of system  	
    loc_time = localtime (&curtime);// Converting current time to local time
    fprintf(stderr, "master: Warning: CTRL + C received, master is terminating > %s\n", asctime (loc_time));
    killpg((*parents), SIGTERM);
    killpg((*children), SIGTERM);
    /*for(int i = 0; i < nprocs; i++){
		wait(NULL);
	    }*/
    sem_remove();
    shm_remove();
    //abort();
    exit(EXIT_SUCCESS);
}





















