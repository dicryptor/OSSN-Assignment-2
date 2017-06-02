//Modified from 
//http://docs.linux.cz/programming/c/unix_examples/semab.html

#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

#define KEY 87654 //Unique semaphore key

typedef char * string;

int main(){

  string henry[10];
  henry[0] = "There's a hole in the bucket, dear Liza, dear Liza,\n"
        "There's a hole in the bucket, dear Liza, a hole.";
  henry[1] = "With what shall I fix it, dear Liza, dear Liza?\n"
        "With what shall I fix it, dear Liza, with what?";
  henry[2] = "The straw is too long, dear Liza, dear Liza,\n"
        "The straw is too long, dear Liza, too long.";
  henry[3] = "With what shall I cut it, dear Liza, dear Liza?\n"
        "With what shall I cut it, dear Liza, with what?";
  henry[4] = "The axe is too dull, dear Liza, dear Liza,\n"
        "The axe is too dull, dear Liza, too dull.";
  henry[5] = "With what shall I sharpen it, dear Liza, dear Liza?\n"
        "With what shall I sharpen it, dear Liza, with what?";
  henry[6] = "The stone is too dry, dear Liza, dear Liza,\n"
        "The stone is too dry, dear Liza, too dry.";
  henry[7] = "With what shall I wet it, dear Liza, dear Liza?\n"
        "With what shall I wet it, dear Liza, with what?";
  henry[8] = "In what shall I carry it, dear Liza, dear Liza?\n"
        "In what shall I carry it, dear Liza, in what?";
  henry[9] = "But there's a hole in my bucket, dear Liza, dear Liza,\n"
        "There's a hole in my bucket, dear Liza, a hole.";

  string liza[9];
  liza[0] = "Then fix it, dear Henry, dear Henry, dear Henry,\n"
        "Then fix it, dear Henry, dear Henry, fix it.";
  liza[1] = "With straw, dear Henry, dear Henry, dear Henry,\n"
        "With straw, dear Henry, dear Henry, with straw.";
  liza[2] = "Then cut it, dear Henry, dear Henry, dear Henry,\n"
        "Then cut it, dear Henry, dear Henry, cut it.";
  liza[3] = "With an axe, dear Henry, dear Henry, dear Henry,\n"
        "With an axe, dear Henry, dear Henry, an axe.";
  liza[4] = "Then sharpen it, dear Henry, dear Henry, dear Henry,\n"
        "Then sharpen it, dear Henry, dear Henry, sharpen it.";
  liza[5] = "With a stone, dear Henry, dear Henry, dear Henry,\n"
        "With a stone, dear Henry, dear Henry, a stone.";
  liza[6] = "Then wet it, dear Henry, dear Henry, dear Henry,\n"
        "Then wet it, dear Henry, dear Henry, wet it.";
  liza[7] = "With water, dear Henry, dear Henry, dear Henry,\n"
        "With water, dear Henry, dear Henry, with water.";
  liza[8] = "In a bucket, dear Henry, dear Henry, dear Henry,\n"
        "In a bucket, dear Henry, dear Henry, in a bucket.";


  int id; /* Number by which the semaphore is known within a program */
  
  /* The next thing is an argument to the semctl() function. Semctl() 
     does various things to the semaphore depending on which arguments
     are passed. We will use it to make sure that the value of the 
     semaphore is initially 0. */
  
  union semun {
    int val;
    struct semid_ds *buf;
    ushort * array;
  } argument;
  
  argument.val = 1;
  
  /* Create the semaphore with external key KEY if it doesn't already 
     exists. Give permissions to the world. */
  
  id = semget(KEY, 1, 0666 | IPC_CREAT);
  
  /* Always check system returns. */
  
  if(id < 0)
    {
      fprintf(stderr, "Unable to obtain semaphore.\n");
      exit(0);
    }
  
  /* What we actually get is an array of semaphores. The second 
     argument to semget() was the array dimension - in our case
     1. */
  
  /* Set the value of the number 0 semaphore in semaphore array
     # id to the value 0. */
  
  if( semctl(id, 0, SETVAL, argument) < 0)
    {
      fprintf( stderr, "Cannot set semaphore value.\n");
    }
  else
    {
      fprintf(stderr, "Semaphore %d initialized.\n", KEY);
    }
  
  int pid=fork();
  int p1 = 0;
  int p2 = 0;
  
  if(pid){
    struct sembuf operations[1];
    int retval; /* Return value from semop() */

    /* Get the index for the semaphore with external name KEY. */
    id = semget(KEY, 1, 0666);

    if(id < 0){
      /* Semaphore does not exist. */
      
      fprintf(stderr, "Program sema cannot find semaphore, exiting.\n");
      exit(0);
    }
    operations[0].sem_num = 0;
    /* Which operation? Subtract 1 from semaphore value : */
    operations[0].sem_op = -1;
    /* Set the flag so we will wait : */   
    operations[0].sem_flg = 0;
    

    
    while(1){
      //Process 1
      if (p1 > 9)
        p1 = 0;

      //wait
      operations[0].sem_op = -1;
      retval = semop(id, operations, 1);


      //critical section
      // printf("In critical section P1 ... iteration no %d\n", p1);
      printf("%s\n", henry[p1]);
      fflush(stdout);
      int stime=2;
      // printf("Sleeping for %d secs\n",stime);
      sleep(stime);


      // printf("Ending critical section P1 ... \n");
      p1++;
      fflush(stdout);

      operations[0].sem_op = 1;
      //signal
      retval = semop(id, operations, 1);

    }
  }else{
    //Process 2
    struct sembuf operations[1];
    int retval; /* Return value from semop() */
    /* Get the index for the semaphore with external name KEY. */
    id = semget(KEY, 1, 0666);
    if(id < 0){
      /* Semaphore does not exist. */
      
      fprintf(stderr, "Program sema cannot find semaphore, exiting.\n");
      exit(0);
    }
    operations[0].sem_num = 0;
    /* Which operation? Subtract 1 from semaphore value : */
    operations[0].sem_op = -1;
    /* Set the flag so we will wait : */   
    operations[0].sem_flg = 0;
    
    while(1){

      if (p2 > 8)
        p2 = 0;
      
      //wait
      operations[0].sem_op = -1;
      retval = semop(id, operations, 1);
      
      //critical section
      // printf("In critical section P2 ... iteration no %d\n", p2);
      printf("%s\n", liza[p2]);
      fflush(stdout);
      int stime=2;
      // printf("Sleeping for %d secs\n",stime);
      sleep(stime);

      // printf("Ending critical section P2 ... \n");
      p2++;
      fflush(stdout);

      //signal
      operations[0].sem_op = 1;
      retval = semop(id, operations, 1);

    }
    
  }
  
}