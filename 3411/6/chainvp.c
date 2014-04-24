#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int chainvp(int num, char **list){
  int i, idx, start; /* Used in some loops */
  int pipes[num+1][2]; /* All of the pipes */
  char ch; /* Used for printing */

  pipe(pipes[0]);
  /*printf("open pipe[%d]: %d,%d\n",0,pipes[0][0],pipes[0][1]);*/

  if( list[0] != NULL){ /* Take input from file */
    int in = open(list[0], O_RDONLY);
    if( in < 0){
      write(2,"Error opening file\n",19);
      return -1;
    }
    while( read(in, &ch, 1) ){
      write(pipes[0][1],&ch,1); /* Write the file to the pipe */
    }
  }
  close(pipes[0][1]);
  
  idx=2;
  for(i=0;i<num;i++){ /* For each program in the chain */
    start = idx;
    while( list[idx] != NULL){ /* Find the end of the args list */
      idx++;
    }
    idx++; /* the next prog start point */
    pipe(pipes[i+1]); /* open up pipe to next proc */
    if(fork() == 0){
      /* If the first process does not have file input it should read from stdin 
       * otherwise processes should read from the pipe */
      if( i != 0 || list[0] != NULL){ 
        close(0);
        dup(pipes[i][0]);
      }

      /* We don't need to write to the prev proc */
      close(pipes[i][1]);
      /* We don't need to read from the next proc */
      close(pipes[i+1][0]);

      /* Write to next proc */
      close(1);
      dup(pipes[i+1][1]);

      execvp(list[start], &list[start]);
      fprintf(stderr,"name: %s\n",list[start]);
      write(2,"exec error\n",11);
      return -1;
    }
    /* We don't write to the next proc */
    close(pipes[i+1][1]);
  }

  int status; /* return status of children */
  int err = 0;  /* Return status of this program */
  for(i=0;i<num;i++){
    wait(&status);
    if( err == 0 ){
      err = WEXITSTATUS(status);
    }
  }

  int writedest = 1;
  if( list[1] != NULL){ /* Take input from file */
    writedest = open(list[1], O_WRONLY | O_CREAT, 0600);
    if( writedest < 0){
      write(2,"Error opening file. Using stdout\n",19);
      writedest = 1;
    }
  }
  while( read(pipes[num][0], &ch, 1)){
    write(writedest, &ch, 1);
  }
  close(pipes[num][0]);

  return err;
}
