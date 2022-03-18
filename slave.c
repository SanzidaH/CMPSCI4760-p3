/**
* @Author: Sanzida Hoque
* @Course: CMP_SCI 4760 Operating Systems 
*
*/

#include "config.h"

int main(int argc, char *argv[])
{
    printf("slave is processessing...\n");
 
    /* Parsing Command Line argument */  
    int procid = atoi(argv[1]); 
    int nprocs = atoi(argv[2]); 
    printf("%d %d\n", procid, nprocs);
   
   
    char logfname[256];
    sprintf(logfname, "logfile.%d", procid);

    time_t curtime;
    struct tm *loc_time;
    char buf[LEN];
    int random_t = 0;


    for ( i = 0; i < 5; i++){
        srand(time(NULL) + procid + i); // seed for random sleep time
        int thread = procid - 1;
        /* acquire the semaphore */
        //TODO add code for acquire the semaphore
        logfile = fopen(logfname, "a");
        cstest = fopen("cstest", "a");
        if (cstest == NULL || logfile == NULL){
            perror("slave: Error: file open failed");
        }
        /* Logging */       
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
        fprintf(cstest, "%s Queue %d File modified by process number %d\n", buf, tickets[procid - 1], procid);
       
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
    }

    exit(EXIT_SUCCESS);
}


