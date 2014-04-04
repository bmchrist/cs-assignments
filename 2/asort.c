#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>


int main ( int argc, char *argv[])
{
  if (argc < 2) {
    write(2, "Not enough arguments\n", 21);
    return 1;
  }

  /* Create the SORTED directory if it does not exist */
  DIR *pDir = opendir("SORTED");
  if ( pDir == NULL){ /* Could not open director */
    int status = mkdir("SORTED", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
  else{
    closedir(pDir);
  }

  /* Track pid => filename */
  int pids[argc-1];
  char *names[argc-1];

  /* Sort each file */
  int i;
  char *passed_env[] = {NULL};
  for(i=1; i<argc; i++){
    pid_t pid;
    pid = fork();
    if( pid < 0 ){
      write(2, "Fork failed\n", 12);
      return 1;
    }
    if( pid == 0){ /* This is the child */

      /* Generate the output file name */
      int len = strlen(argv[i]) + 7;
      char *outfile = malloc(len); /* SORTED/filename */
      if( outfile == NULL){
        write(2, "Failed to allocate memory\n", 26);
      }
      strncpy(outfile, "SORTED/", len);
      strncat(outfile, argv[i], len - strlen(outfile));
      /*printf("Str: %s \n", outfile);*/

      char *passed_args[] = {"sort", "-o", outfile, argv[i], NULL};
      execve("/usr/bin/sort", passed_args, passed_env);

      /* No need to free outfile, since execve overwrites data segment */
      write(1, "Failed to exec sort\n", 20);
      return 1;
    }
    /* Track the pid -> name */
    pids[i-1] = pid;
    names[i-1] = argv[i];
  }

  int status;
  pid_t wpid;
  for(i=1; i<argc; i++){
    wpid = wait(&status);

    /* Find the filename */
    int j=0;
    while( pids[j] != wpid){
      j++;
    }

    /* Print status information */
    char *buff;
    int size;
    if( status == 0){
      size = asprintf(&buff, "%s : success(%d)\n", names[j], status);
    }
    else{
      size = asprintf(&buff, "%s : failure(%d)\n", names[j], status);
    }
    write(1, buff, size);
    free(buff);
  }
}
